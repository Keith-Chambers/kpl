#include <cstdlib>
#include <iostream>

#include "catch2/catch.hpp"

#define CHAINED_ARRAY_DEBUG

#include <kpl/chainedarray.h>
#include "testextras.h"

TEST_CASE( "ChainedArray Tests", "[Tests]" )
{
    /*  BASIC SETUP 
        
        Methods Tested: 
            size()
            cT(uint16_t)
            t(uint16_t)
            tRef(uint16_t)
            cTRef(uint16_t) 
            
        Implementation Details:
            Data that expands stack & heap buffer
            
            */
    
    
    std::function<void (const NotDefaultConstructable)> printNotDefaultConstructable = 
    [](const NotDefaultConstructable t)
    {
        std::cout << "A -> " << t.getA();
        std::cout << "\nB -> " << t.getB() << "\n" << std::endl;
    };
    
    const uint8_t startSize = 15;
    const uint16_t stackPreallocatedSize = 10;
    const uint16_t heapBufferSize = 25;
    const uint16_t maxNumHeapBuffers = 2;
    //const uint16_t totalMaxSize = stackPreallocatedSize + (heapBufferSize * maxNumHeapBuffers);
    
    kpl::ChainedArray<NotDefaultConstructable, stackPreallocatedSize, heapBufferSize, maxNumHeapBuffers> chainedArray;
    
    REQUIRE(chainedArray.size() == 0);
    
    for(uint16_t i = 0; i < startSize; i++)
    {
        NotDefaultConstructable data(i, 'k');
        chainedArray.pushBack(data);
        
        REQUIRE( chainedArray.t(i).getA() == i );
        REQUIRE( chainedArray.tRef(i).getA() == i );
        REQUIRE( chainedArray.cTRef(i).getA() == i );
        
    }
    
    REQUIRE(chainedArray.size() == startSize);
    
    /* Test EmplaceBack(const T& t) */
    SECTION( "Emplace Back" )
    {
        const uint16_t testVal = 111;
        chainedArray.emplaceBack(testVal, 'b');
        
        REQUIRE( chainedArray.t( startSize ).getA() == testVal );
        REQUIRE( chainedArray.tRef( startSize ).getA() == testVal );
        REQUIRE( chainedArray.cTRef( startSize ).getA() == testVal );
        
        REQUIRE(chainedArray.size() == startSize + 1);
    }
    
    SECTION( "Push Back" ) 
    {
        const uint16_t testVal = 222;
        NotDefaultConstructable data(testVal, 'k');
        
        chainedArray.pushBack(data);
        
        REQUIRE( chainedArray.t( startSize ).getA() == testVal );
        REQUIRE( chainedArray.tRef( startSize ).getA() == testVal );
        REQUIRE( chainedArray.cTRef( startSize ).getA() == testVal );
        
        REQUIRE(chainedArray.size() == startSize + 1);
    }
    
    
    SECTION( "Insert At" )
    {
        const uint16_t numToInsert = 10;
        const uint16_t insertionIndex = 8;
        const uint16_t testVal = 333;
        
        // Cross buffer boundry
        for(uint16_t i = insertionIndex; i < (insertionIndex + numToInsert); i++)
        {
            NotDefaultConstructable data(testVal + i - insertionIndex, 'k');
            chainedArray.insertAt(data, i);
            
            REQUIRE( chainedArray.cTRef( i ).getA() == testVal + i - insertionIndex );
            REQUIRE( chainedArray.size() == startSize + (i - insertionIndex) + 1);
        }
            
        // Check the last value is correct 
        REQUIRE( chainedArray.cTRef( chainedArray.size() - 1 ).getA() == startSize - 1);
    }
    
    SECTION( "Remove At" )
    {
        // Remove last
        REQUIRE( chainedArray.cTRef(chainedArray.size() - 1).getA() == startSize - 1);
        
        REQUIRE( chainedArray.remove( chainedArray.size() - 1 ) );
        REQUIRE( chainedArray.size() == startSize - 1);
        REQUIRE( chainedArray.cTRef(chainedArray.size() - 1).getA() == startSize - 2);
        
        // Remove middle heap 
        
        REQUIRE( chainedArray.cTRef(stackPreallocatedSize + 1).getA() == stackPreallocatedSize + 1); 
        
        chainedArray.remove(stackPreallocatedSize + 1);
        REQUIRE( chainedArray.size() == startSize - 2);
        // Check value at deleted index is now set to the next element to right
        REQUIRE( chainedArray.cTRef(stackPreallocatedSize + 1).getA() == stackPreallocatedSize + 2);
        
        // Remove middle stack 
        REQUIRE( chainedArray.remove(stackPreallocatedSize - 2) );
        REQUIRE( chainedArray.size() == startSize - 3);
        REQUIRE( chainedArray.cTRef(stackPreallocatedSize - 2).getA() == stackPreallocatedSize - 1);
        
        // Remove first 
        chainedArray.remove(0);
        REQUIRE( chainedArray.size() == startSize - 4);
        
        // Start value is now the second element 
        REQUIRE( chainedArray.cTRef(0).getA() == 1);
    }
    
    SECTION( "Remove End" )
    {
        for(uint16_t i = 0; i < chainedArray.size(); i++)
        {
            REQUIRE( chainedArray.removeEnd() );
            REQUIRE( chainedArray.size() == startSize - i - 1);
            REQUIRE( chainedArray.cTRef(chainedArray.size() - 1).getA() == startSize - i - 2);
        }
    }

    /* Testing the asArray Interface */
    kpl::array<NotDefaultConstructable> * arrayInterface = chainedArray.asArray();

    SECTION( "Push Back" )
    {
        const uint16_t testVal = 222;
        NotDefaultConstructable data(testVal, 'k');

        arrayInterface->pushBack(data);

        REQUIRE( arrayInterface->t( startSize ).getA() == testVal );
        REQUIRE( arrayInterface->tRef( startSize ).getA() == testVal );
        REQUIRE( arrayInterface->cTRef( startSize ).getA() == testVal );

        REQUIRE(arrayInterface->size() == startSize + 1);
    }


    SECTION( "Insert At" )
    {
        const uint16_t numToInsert = 10;
        const uint16_t insertionIndex = 8;
        const uint16_t testVal = 333;

        // Cross buffer boundry
        for(uint16_t i = insertionIndex; i < (insertionIndex + numToInsert); i++)
        {
            NotDefaultConstructable data(testVal + i - insertionIndex, 'k');
            arrayInterface->insertAt(data, i);

            REQUIRE( arrayInterface->cTRef( i ).getA() == testVal + i - insertionIndex );
            REQUIRE( arrayInterface->size() == startSize + (i - insertionIndex) + 1);
        }

        // Check the last value is correct
        REQUIRE( arrayInterface->cTRef( arrayInterface->size() - 1 ).getA() == startSize - 1);
    }

    SECTION( "Remove At" )
    {
        // Remove last
        REQUIRE( arrayInterface->cTRef(arrayInterface->size() - 1).getA() == startSize - 1);

        REQUIRE( arrayInterface->remove( arrayInterface->size() - 1 ) );
        REQUIRE( arrayInterface->size() == startSize - 1);
        REQUIRE( arrayInterface->cTRef(arrayInterface->size() - 1).getA() == startSize - 2);

        // Remove middle heap

        REQUIRE( arrayInterface->cTRef(stackPreallocatedSize + 1).getA() == stackPreallocatedSize + 1);

        arrayInterface->remove(stackPreallocatedSize + 1);
        REQUIRE( arrayInterface->size() == startSize - 2);
        // Check value at deleted index is now set to the next element to right
        REQUIRE( arrayInterface->cTRef(stackPreallocatedSize + 1).getA() == stackPreallocatedSize + 2);

        // Remove middle stack
        REQUIRE( arrayInterface->remove(stackPreallocatedSize - 2) );
        REQUIRE( arrayInterface->size() == startSize - 3);
        REQUIRE( arrayInterface->cTRef(stackPreallocatedSize - 2).getA() == stackPreallocatedSize - 1);

        // Remove first
        arrayInterface->remove(0);
        REQUIRE( arrayInterface->size() == startSize - 4);

        // Start value is now the second element
        REQUIRE( arrayInterface->cTRef(0).getA() == 1);
    }

    SECTION( "Remove End" )
    {
        for(uint16_t i = 0; i < arrayInterface->size(); i++)
        {
            REQUIRE( arrayInterface->removeEnd() );
            REQUIRE( arrayInterface->size() == startSize - i - 1);
            REQUIRE( arrayInterface->cTRef(arrayInterface->size() - 1).getA() == startSize - i - 2);
        }
    }

    SECTION ( "MAX SIZE" )
    {
        for(uint16_t i = startSize; i < (stackPreallocatedSize + (heapBufferSize * maxNumHeapBuffers)); i++)
        {
            NotDefaultConstructable data(0, 'k');
            REQUIRE( arrayInterface->insertAt(data, i - startSize) );
        }
    }
}
