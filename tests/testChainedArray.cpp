#include <cstdlib>

#include "catch2/catch.hpp"

#include <kpl/chainedarray.h>

#include "testextras.h"

TEST_CASE( "ChainedArray Tests", "[Tests]" )
{
    std::size_t stackSize = 10;
    kpl::ChainedArray<NotDefaultConstructable, 10, 25, 10> chainedArray;
    
    REQUIRE(chainedArray.size() == 0);
    
    chainedArray.emplaceBack(5, 'b');
    //REQUIRE(chainedArray.size() == 1);
    
    //REQUIRE( chainedArray.cT(0).getA() == 5 );
    //REQUIRE( chainedArray.cT(0).getB() == 'b' );
    
    
    for(size_t i = chainedArray.size(); i < stackSize + 5; i++)
    {
        NotDefaultConstructable data(i, 'k');
        chainedArray.pushBack(data);
        
        //REQUIRE( chainedArray.cT(i).getA() == i );
    }
    
    //REQUIRE( chainedArray.size() ==  stackSize + 5);
    
    NotDefaultConstructable data(100, 'k');
    chainedArray.insertAt(data, 0);
    
    //REQUIRE( chainedArray.t(0).getA() == 100 );
}
