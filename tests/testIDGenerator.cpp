#include "catch2/catch.hpp"

#include <kpl/idgenerator.h>

TEST_CASE( "IDGenerator Tests", "[Test]" )
{
    kpl::IDGenerator idGen;

    std::string first = "first";
    std::string second = "second";
    std::string third = "third";

    std::string alsoSecond = "second";

    REQUIRE( idGen.getID( first ) == 1 );
    REQUIRE( idGen.getID( second ) == 2 );
    REQUIRE( idGen.getID( third ) == 3 );
    REQUIRE( idGen.getID( alsoSecond ) == 2 );
    REQUIRE( idGen.getID( third ) == 3 );
}
