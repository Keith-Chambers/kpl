#include "catch2/catch.hpp"

#include <kpl/pair.h>

#include "testextras.h"

TEST_CASE( "Pair Tests", "[Something]" )
{
    int firstInt = 5;
    int secondInt = 10;
    
    /* Simple test of getters */
    kpl::Pair<int, int> intPair(firstInt, secondInt);
    
    REQUIRE(intPair.getT1CRef() == firstInt);
    REQUIRE(intPair.getT1Ref() == firstInt);
    REQUIRE(intPair.getT1() == firstInt);
    
    REQUIRE(intPair.getT2CRef() == secondInt);
    REQUIRE(intPair.getT2Ref() == secondInt);
    REQUIRE(intPair.getT2() == secondInt);
    
    /* Test non constructed pair is valid */
    kpl::Pair<int, int> defaultConstructedPair;
    
    REQUIRE(defaultConstructedPair.isT1Set() == false);
    REQUIRE(defaultConstructedPair.isT2Set() == false);
    
    defaultConstructedPair.setT1(firstInt);
    
    REQUIRE(defaultConstructedPair.isT1Set() == true);
    REQUIRE(defaultConstructedPair.getT1CRef() == firstInt);
    REQUIRE(defaultConstructedPair.getT1Ref() == firstInt);
    REQUIRE(defaultConstructedPair.getT1() == firstInt);
    
    defaultConstructedPair.setT2(secondInt);
    REQUIRE(defaultConstructedPair.isT2Set() == true);
    
    /* Test pair with a non default constructable type */
    kpl::Pair<int, NotDefaultConstructable> complexPair;
    complexPair.emplaceT2(5, 'b');
    
    REQUIRE( complexPair.getT2().getA() == 5 );
    REQUIRE( complexPair.getT2().getB() == 'b' );
}
