#include "catch2/catch.hpp"

#include <kpl/pair.h>

#include "testextras.h"

TEST_CASE( "Pair Tests", "[Something]" )
{
    int firstInt = 5;
    int secondInt = 10;
    
    /* Simple test of getters */
    kpl::Pair<int, int> intPair(firstInt, secondInt);
    
    REQUIRE(*intPair.cPtr1() == firstInt);
    REQUIRE(*intPair.ptr1() == firstInt);
    REQUIRE(intPair.cT1() == firstInt);
    REQUIRE(intPair.t1() == firstInt);
    
    REQUIRE(*intPair.cPtr2() == secondInt);
    REQUIRE(*intPair.ptr2() == secondInt);
    REQUIRE(intPair.cT2() == secondInt);
    REQUIRE(intPair.t2() == secondInt);
    
    /* Test non constructed pair is valid */
    kpl::Pair<int, int> defaultConstructedPair;
    
    REQUIRE(defaultConstructedPair.isT1Set() == false);
    REQUIRE(defaultConstructedPair.isT2Set() == false);
    
    defaultConstructedPair.setT1WithCopy(firstInt);
    
    REQUIRE(defaultConstructedPair.isT1Set() == true);
    REQUIRE(*intPair.cPtr1() == firstInt);
    REQUIRE(*intPair.ptr1() == firstInt);
    REQUIRE(intPair.cT1() == firstInt);
    REQUIRE(intPair.t1() == firstInt);
    
    defaultConstructedPair.setT2WithCopy(secondInt);
    REQUIRE(defaultConstructedPair.isT2Set() == true);
    
    /* Test pair with a non default constructable type */
    kpl::Pair<int, NotDefaultConstructable> complexPair;
    complexPair.emplaceT2(5, 'b');
    
    REQUIRE( complexPair.t2().getA() == 5 );
    REQUIRE( complexPair.t2().getB() == 'b' );
}
