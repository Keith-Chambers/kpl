#ifndef CHAINEDARRAY_H
#define CHAINEDARRAY_H

//#define CHAINED_ARRAY_DEBUG

#ifdef CHAINED_ARRAY_DEBUG

#include <fmt/format.h>
#include <assert.h>
#include <cstdio>

#endif

#include <cstdlib>
#include <cstdint>
#include <cstring>

#include <kpl/util.h>
#include <kpl/pair.h>

/*  Use case: When you are reasonably sure that you want to store a small amount of elements
 *  but need to ability to store far more to be safe */
namespace kpl{

    template <typename T, std::size_t stackPreAlloc, std::size_t dynaBufferSize, std::size_t numDynaBuffers>
    class ChainedArray
    {

        uint16_t bufferIndexFor(uint16_t index) const
        {
            if(index < stackPreAlloc)
                return index;
            
            uint16_t result = 1;
            index -= stackPreAlloc;
            
            while(index >= dynaBufferSize)
            {
                result++;
                index -= dynaBufferSize;
            }
            
            return result;
        }

        const uint8_t * addrForElement(uint16_t index) const
        {
            // Make sure requested index is in range
            if( stackPreAlloc + (dynaBufferSize * numDynaBuffers) < index )
            {
                #ifdef CHAINED_ARRAY_DEBUG
                fmt::print(stderr, "Error: Requested address exceeds max allocated memory in ChainedArray\n"
                           "Requested Index: {}\n"
                           "Preallocated Stack Memory: {}\n"
                           "Dynamic Memory Block Size: {}\n"
                           "Max Number Dyanamic Memory Blocks: {}\n", index, stackPreAlloc, dynaBufferSize, numDynaBuffers);

                exit(EXIT_FAILURE);
                #endif

                return nullptr;
            }

            
            kpl::Pair<uint16_t, uint16_t> absoluteIndexPair = translateIndex(index);
            
        #ifdef CHAINED_ARRAY_DEBUG
            fmt::print("Absolute Index Pair for Raw Index of {} -> {}:{}\n", index, absoluteIndexPair.cT1(), absoluteIndexPair.cT2());
        #endif

            // Get address from stack memory
            if(absoluteIndexPair.cT1() == 0)
                return reinterpret_cast<const uint8_t *>(& (mStackBuffer[index * sizeof(T)]));

            // Since index has been localised to a memory block, it should not exceed the size of one
            if(absoluteIndexPair.cT2() > dynaBufferSize)
            {
                #ifdef CHAINED_ARRAY_DEBUG

                fmt::print(stderr, "Error: index({}) > dynaBufferSize({}) assert failed in ChainedArray::addrForElement({})\n", absoluteIndexPair.cT2(), dynaBufferSize, absoluteIndexPair.cT2());
                assert(absoluteIndexPair.cT2() <= dynaBufferSize);

                #endif

                return nullptr;
            }
            /*
            // Allocate another memory block if needed
            if(mDynaMemBuffer[bufferIndex] == nullptr)
                mDynaMemBuffer[bufferIndex] = nullptr; //reinterpret_cast<uint8_t*>( malloc( sizeof(T) * dynaBufferSize ) ); */

            // Get beginning of correct memory block and move index * sizeof(T) bytes forward
            return reinterpret_cast<const uint8_t *>(mDynaMemBuffer[absoluteIndexPair.cT1()] + (absoluteIndexPair.cT2() * sizeof(T)));
        }

        /* BufferIndex, RelativeIndex */
        kpl::Pair<uint16_t, uint16_t> translateIndex(uint16_t index) const
        {
            if(index < stackPreAlloc)
                return kpl::Pair<uint16_t, uint16_t>(0, index);
            
            uint16_t result = 1;
            index -= stackPreAlloc;
            
            while(index >= dynaBufferSize)
            {
                result++;
                index -= dynaBufferSize;
            }
            
            return kpl::Pair<uint16_t, uint16_t>(result, index);
        }

        bool allocateNextBuffer()
        {
            uint16_t nextBufferIndex = bufferIndexFor(mNumElements) + 1;
            if(nextBufferIndex == numDynaBuffers)
            {
                #ifdef CHAINED_ARRAY_DEBUG

                fmt::print(stderr, "Error: Reached max number of dyna buffers to allocate\n");
                assert(nextBufferIndex != numDynaBuffers);

                #endif
            }

            if(mDynaMemBuffer[nextBufferIndex] == nullptr)
                mDynaMemBuffer[nextBufferIndex] = reinterpret_cast<uint8_t*>( malloc( sizeof(T) * dynaBufferSize ) );

            return true;
        }

        T* dataPtrAt(kpl::Pair<uint16_t, uint16_t> indexPair)
        {
            if(indexPair.cT1() == 0)
            {
                return reinterpret_cast<T*>(& mStackBuffer[indexPair.cT2() * sizeof(T)]);
            } else
            {
                return reinterpret_cast<T*>(mDynaMemBuffer[indexPair.cT1() - 1] + (indexPair.cT2() * sizeof(T)));
            }
        }
        
    #ifdef CHAINED_ARRAY_DEBUG
        void dumpState()
        {
            fmt::print("Dumping chained buffer state\n\n");
            
            fmt::print("Stack allocated space: {}\n", stackPreAlloc);
            fmt::print("Dynamic buffer size: {}\n", dynaBufferSize);
            fmt::print("Number of Dynamic Buffers: {}\n", numDynaBuffers);
            fmt::print("Size: {}\n", mNumElements);
            fmt::print("Tail buffer index: {}\n\n", bufferIndexFor(mNumElements));
        }
    #endif

        /* TODO: Make more efficient */
        void rightShiftData(uint16_t indexFrom, uint16_t shiftSize)
        {
            std::size_t currentBufferIndex = bufferIndexFor(mNumElements);
            int16_t spaceMissing;
            
        #ifdef CHAINED_ARRAY_DEBUG
            fmt::print("Right data shift required in chainedarray. indexFrom -> {}, shiftSize -> {}\n", indexFrom, shiftSize);
            fmt::print("Data extends to buffer # {}\n", currentBufferIndex);
            dumpState();
        #endif

            // Calculate additional space required for operation
            if(currentBufferIndex == 0)
            {
                spaceMissing = (mNumElements + shiftSize) - stackPreAlloc;
            } else
            {
                uint16_t elementsInBuffer = mNumElements - stackPreAlloc;

                while(elementsInBuffer > dynaBufferSize)
                    elementsInBuffer -= dynaBufferSize;

                spaceMissing = (elementsInBuffer + shiftSize) - dynaBufferSize;
                spaceMissing = (spaceMissing < 0) ? 0 : spaceMissing;
            }
            
        #ifdef CHAINED_ARRAY_DEBUG
            fmt::print("Additional space required from end buffer: {}\n", spaceMissing);
        #endif

            // Allocate enough buffers to store the shifted data
            while(spaceMissing > 0)
            {
            #ifdef CHAINED_ARRAY_DEBUG
                fmt::print("Allocating extra buffer..");
            #endif
                allocateNextBuffer();
                spaceMissing -= dynaBufferSize;
            }

            // Start at end of array and work towards in shift point
            uint16_t shiftPointIndex = mNumElements;
        #ifdef CHAINED_ARRAY_DEBUG
            kpl::Pair<uint16_t, uint16_t> testIndexPair = translateIndex(shiftPointIndex);
            fmt::print("Shift point: raw -> {}, bufferIndex -> {}, localIndex -> {}\n", shiftPointIndex, testIndexPair.cT1(), testIndexPair.cT2());
        #endif
            
            while( true )
            {
                kpl::Pair<uint16_t, uint16_t> srcIndexPair = translateIndex(shiftPointIndex);
                kpl::Pair<uint16_t, uint16_t> dstIndexPair = translateIndex(shiftPointIndex + shiftSize);
                
            #ifdef CHAINED_ARRAY_DEBUG
                fmt::print("Moving data from index {}:{} to {}:{}\n", srcIndexPair.cT1(), srcIndexPair.cT2(), dstIndexPair.cT1(), dstIndexPair.cT2());
            #endif

                *dataPtrAt(dstIndexPair) = *dataPtrAt(srcIndexPair);
                
                if(shiftPointIndex-- == 0)
                    break;
            }
            
            #ifdef CHAINED_ARRAY_DEBUG
                fmt::print("Data shift completed\n");
            #endif
        }

    public:
        ChainedArray()
            : mNumElements { 0 },
              mDynaAllocatedMemoryBlocks {0}
        {
            for(std::size_t i = 0; i < numDynaBuffers; i++)
                mDynaMemBuffer[i] = nullptr;
        }

        /* Getters */
        const T * cTPtr(uint16_t index) const
        {
           const uint8_t * location = addrForElement(index);

           fmt::print("Hey");

           return reinterpret_cast<const T*>(location);
        }

        T * tPtr(uint16_t index)
        {
            uint8_t * location = reinterpret_cast<uint8_t*>(addrForElement(index));
            return reinterpret_cast<T*>(location);
        }

        T t(uint16_t index) const
        {
            uint8_t * location = const_cast<uint8_t*>(addrForElement(index));
            return *(reinterpret_cast<T*>(location));
        }

        const T cT(uint16_t index) const
        {
            const uint8_t * location = addrForElement(index);
            return *(reinterpret_cast<const T*>(location));
        }

        const T * operator[](uint16_t index)
        {
            return cTPtr(index);
        }

        std::size_t size() const
        {
            return mNumElements;
        }

        /* Modifiers */
        bool pushBack(T t)
        {
            uint8_t * placement;
            
            // Fast path, stack only
            if(mNumElements < stackPreAlloc)
            {
                placement = &mStackBuffer[ mNumElements * sizeof(T) ];
                new (  reinterpret_cast<void*>( placement ) ) T( t );
                
                mNumElements++;
                
            #ifdef CHAINED_ARRAY_DEBUG
                fmt::print("Element added. Size -> {}\n", mNumElements);
            #endif
                
                return true;
            }
            
            /* Slow path that needs to take dynamic buffers into account */
            
            // T1: bufferIndex, T2: Relative index
            kpl::Pair<uint16_t, uint16_t> fullIndex = translateIndex(mNumElements);
            
            if(! ensureBufferAllocated(fullIndex.cT1() - 1))
            {
            #ifdef CHAINED_ARRAY_DEBUG
                fmt::print("Error: Failed to allocate DYNAMIC buffer # {} for pushBack op\n", fullIndex.cT1() - 1);
                dumpState();
                assert(false);
            #endif
                
                return false;
            }
            
            placement = mDynaMemBuffer[fullIndex.cT1() - 1] + ( sizeof(T) * fullIndex.cT2() );

            new (  reinterpret_cast<void*>(placement) ) T( t );
            mNumElements++;
            
        #ifdef CHAINED_ARRAY_DEBUG
            fmt::print("Element added. Size -> {}\n", mNumElements);
        #endif

            return true;
        }

        bool insertAt(const T& t, uint16_t index)
        {
            uint8_t * placement = const_cast<uint8_t*>(addrForElement(index));
            
            if(placement == nullptr)
            {
            #ifdef CHAINED_ARRAY_DEBUG
                fmt::print("Error: Failed to get placement address for insertAt op\n");
                dumpState();
                assert(false);
            #endif
                return false;
            }

            // Make space for new element
            rightShiftData(index, 1);

            // Place new element in the cleared space
            new (  placement ) T( t );

            return true;
        }

        /*  Contructs a type T at the memory address calculate to placement using T's constructor values
         *  E.G Instead of push_back( t ), you do emplace_back( a, b, c ) where type T has a constructor of T(a, b, c */
        template <typename... Ts>
        bool emplaceBack(Ts&&... args)
        {
            uint8_t * placement = const_cast<uint8_t *>(addrForElement(mNumElements));
            
            if(placement == nullptr)
            {
            #ifdef CHAINED_ARRAY_DEBUG
                fmt::print("Error: Failed to get placement address for emplaceBack op\n");
                dumpState();
                assert(false);
            #endif
                return false;
            }

            if(placement == nullptr)
                return false;

            new (  reinterpret_cast<void*>(placement) ) T( static_cast<Ts&&>(args)... );
            mNumElements++;
            
        #ifdef CHAINED_ARRAY_DEBUG
            fmt::print("Element added. Size -> {}\n", mNumElements);
        #endif

            return true;
        }
        
        bool ensureBufferAllocated(uint16_t dynaBufferIndex)
        {
            if(dynaBufferIndex >= numDynaBuffers)
                return false;
            
            if(mDynaMemBuffer[dynaBufferIndex] == nullptr)
                mDynaMemBuffer[dynaBufferIndex] = reinterpret_cast<uint8_t*>( malloc( sizeof(T) * dynaBufferSize ) );
            
            assert( mDynaMemBuffer[dynaBufferIndex] );
            
            return true;
        }

        template <typename... Ts>
        bool emplaceAt(Ts&&... args, uint16_t index)
        {
            // Fast path, stack only
            uint8_t * placement;
            
            if(index < stackPreAlloc)
            {
                placement = &mStackBuffer[ index * sizeof(T) ];
                new (  reinterpret_cast<void*>( placement ) ) T( static_cast<Ts&&>(args)... );
                
                mNumElements++;
                
                return true;
            }
            
            /* Slow path that needs to take dynamic buffers into account */
            
            // T1: bufferIndex, T2: Relative index
            kpl::Pair<uint16_t, uint16_t> fullIndex = translateIndex(index);
            
            if(! ensureBufferAllocated(fullIndex.cT1() - 1))
            {
            #ifdef CHAINED_ARRAY_DEBUG
                fmt::print("Error: Failed to allocate DYNAMIC buffer # {} for emplaceBack op\n", fullIndex.cT1() - 1);
                dumpState();
                assert(false);
            #endif
                
                return false;
            }
            
            placement = mDynaMemBuffer[fullIndex.cT1() - 1] + ( sizeof(T) * fullIndex.cT2() );
            
            // TODO: Make space for element

            new (  reinterpret_cast<void*>(placement) ) T( static_cast<Ts&&>(args)... );
            mNumElements++;

            return true;
        }

    private:

        uint8_t mStackBuffer[sizeof(T) * stackPreAlloc];
        uint8_t * mDynaMemBuffer[numDynaBuffers];

        uint16_t mNumElements;
        uint8_t mDynaAllocatedMemoryBlocks;
    };
}

#endif // CHAINEDARRAY_H
