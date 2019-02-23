#include "catch2/catch.hpp"

#include <cstdint>
#include <cstdio>

#include <kpl/linkedbufferarray.h>

TEST_CASE( "LinkedBufferArray Tests", "[Tests]" )
{
    const uint8_t startSize = 15;
    const uint16_t stackPreallocatedSize = 10;
    const uint16_t heapBufferSize = 25;
    const uint16_t maxNumHeapBuffers = 2;
    const uint16_t totalMaxSize = stackPreallocatedSize + (heapBufferSize * maxNumHeapBuffers);
    
    constexpr std::size_t memNeeded = sizeof(kpl::ChainedArray<uint8_t, stackPreallocatedSize, heapBufferSize, maxNumHeapBuffers>);
    uint8_t LinkedBufferArrayAllocation[ memNeeded ];
    
    std::printf("Placement in Tests = %p\n", LinkedBufferArrayAllocation);
    
    kpl::LinkedBufferArray<uint8_t> testLBA (
        std::move(  kpl::LinkedBufferArray<uint8_t>::make<  stackPreallocatedSize, 
                                                            heapBufferSize, 
                                                            maxNumHeapBuffers>  ( LinkedBufferArrayAllocation ) ) );
    
    std::cout << "Base.. -> " << memNeeded << std::endl;
    std::cout << "Size of.. -> " << sizeof(kpl::LinkedBufferArray<uint8_t>) << std::endl;
    
    REQUIRE(testLBA.size() == 0);
    
    /*
    for(uint8_t i = 0; i < startSize; i++)
    {
        testLBA.pushBack(i);
        
        REQUIRE( testLBA.t(i) == i );
        REQUIRE( testLBA.tRef(i) == i );
        REQUIRE( testLBA.cTRef(i) == i );
    }
    */
}
