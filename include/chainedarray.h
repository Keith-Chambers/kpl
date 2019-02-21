#ifndef CHAINEDARRAY_H
#define CHAINEDARRAY_H

#define CHAINED_ARRAY_DEBUG

#ifdef CHAINED_ARRAY_DEBUG

#include <fmt/format.h>
#include <assert.h>
#include <cstdio>

#endif

#include <cstdlib>
#include <cstdint>
#include <cstring>

#include <util.h>
#include <pair.h>

/*  Use case: When you are resonably sure that you want to store a small amount of elements
 *  but need to ability to store far more to be safe */
namespace kpl{

    template <typename T, std::size_t stackPreAlloc, std::size_t dynaBufferSize, std::size_t numDynaBuffers>
    class ChainedArray
    {

        uint16_t bufferIndexFor(uint16_t index) const
        {
            return ( index <= stackPreAlloc ) ? 0 : (index - stackPreAlloc) / dynaBufferSize;
        }

        uint8_t * addrForElement(uint16_t index)
        {
            // Make sure requested index is in range
            if( stackPreAlloc + (dynaBufferSize * numDynaBuffers) <= index )
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

            // Get the index of the buffer it will belong to
            uint16_t bufferIndex = bufferIndexFor(index);

            // Get address from stack memory
            if(bufferIndex == 0)
                return & (mStackBuffer[index * sizeof(T)]);

            // Ignore Stack memory
            bufferIndex--;

            // Convert index to local index inside it's respective dynamically allocated memory block
            index -= stackPreAlloc + (bufferIndex * dynaBufferSize);

            // Since index has been localised to a memory block, it should not exceed the size of one
            if(index > dynaBufferSize)
            {
                #ifdef CHAINED_ARRAY_DEBUG

                fmt::print(stderr, "Error: index({}) > dynaBufferSize({}) assert failed in ChainedArray::addrForElement({})\n", index, dynaBufferSize, index);
                assert(index <= dynaBufferSize);

                #endif

                return nullptr;
            }

            // Allocate another memory block if needed
            if(mDynaMemBuffer[bufferIndex] == nullptr)
                mDynaMemBuffer[bufferIndex] = reinterpret_cast<uint8_t*>( malloc( sizeof(T) * dynaBufferSize ) );

            // Get beginning of correct memory block and move index * sizeof(T) bytes forward
            return mDynaMemBuffer[bufferIndex] + (index * sizeof(T));
        }

        /* BufferIndex, RelativeIndex */
        kpl::Pair<uint16_t, uint16_t> translateIndex(uint16_t index)
        {
            if(index < stackPreAlloc)
                return kpl::Pair<uint16_t, uint16_t>(0, index);

            uint16_t bufferIndex = (index - stackPreAlloc + 1) / dynaBufferSize;
            index -= stackPreAlloc + (bufferIndex * dynaBufferSize);

            return kpl::Pair<uint16_t, uint16_t>(bufferIndex, index);
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

        inline T* dataPtrAt(kpl::Pair<uint16_t, uint16_t> indexPair)
        {
            if(indexPair.cT1() == 0)
            {
                return reinterpret_cast<T*>(& mStackBuffer[indexPair.cT2()]);
            } else
            {
                return reinterpret_cast<T*>(mDynaMemBuffer[indexPair.cT1()] + (indexPair.cT2() * sizeof(T)));
            }
        }

        /* TODO: Make more efficient */
        void rightShiftData(uint16_t indexFrom, uint16_t shiftSize)
        {
            std::size_t currentBufferIndex = bufferIndexFor(mNumElements);
            int16_t spaceMissing;

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

            // Allocate enough buffers to store the shifted data
            while(spaceMissing > 0)
            {
                allocateNextBuffer();
                spaceMissing -= dynaBufferSize;
            }

            // Start at end of array and work towards in shift point
            uint16_t shiftPointIndex = mNumElements;
            while( shiftPointIndex >= indexFrom )
            {
                kpl::Pair<uint16_t, uint16_t> srcIndexPair = translateIndex(shiftPointIndex);
                kpl::Pair<uint16_t, uint16_t> dstIndexPair = translateIndex(shiftPointIndex + shiftSize);

                *dataPtrAt(dstIndexPair) = dataPtrAt(srcIndexPair);
                shiftPointIndex--;
            }
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
           uint8_t * location = addrForElement(index);

           fmt::print("Hey");

           return reinterpret_cast<const T*>(location);
        }

        T * tPtr(uint16_t index)
        {
            uint8_t * location = addrForElement(index);
            return reinterpret_cast<T*>(location);
        }

        T t(uint16_t index)
        {
            uint8_t * location = addrForElement(index);
            return *(reinterpret_cast<T*>(location));
        }

        const T cT(uint16_t index) const
        {
            uint8_t * location = addrForElement(index);
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
            uint8_t * placement = addrForElement(mNumElements);

            if(placement == nullptr)
                return false;

            new (  reinterpret_cast<void*>(placement) ) T( t );
            mNumElements++;

            return true;
        }

        bool insertAt(const T& t, uint16_t index)
        {
            uint8_t * desiredPlacement = addrForElement(index);

            // Make space for new element
            rightShiftData(index, 1);

            // Place new element in the cleared space
            new (  desiredPlacement ) T( t );

            return true;
        }

        /*  Contructs a type T at the memory address calculate to placement using T's constructor values
         *  E.G Instead of push_back( t ), you do emplace_back( a, b, c ) where type T has a constructor of T(a, b, c */
        template <typename... Ts>
        bool emplaceBack(Ts&&... args)
        {
            uint8_t * placement = addrForElement(mNumElements);

            if(placement == nullptr)
                return false;

            new (  reinterpret_cast<void*>(placement) ) T( static_cast<Ts&&>(args)... );
            mNumElements++;

            return true;
        }

        template <typename... Ts>
        bool emplaceAt(Ts&&... args, uint16_t index)
        {
            uint8_t * placement = addrForElement(index);
            // TODO: Make space for element

            if(placement == nullptr)
                return false;

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
