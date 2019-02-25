#include <iostream>

#include "catch2/catch.hpp"

#include <kpl/fixedstringbuffer.h>

TEST_CASE( "FixedStringBuffer Tests", "[Tests]" )
{
    kpl::FixedStringBuffer<100> string;
    
    REQUIRE( string.bufSize() == 100 );
    REQUIRE( string.dataSize == 0 );
    
    std::memcpy( string.data, "Hello", 6);
    string.setSize( strlen("Hello") );
    
    REQUIRE( string.dataSize == 5 );
}
