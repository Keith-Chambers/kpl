#include "catch2/catch.hpp"

#include <string>
#include <iostream>

#include <kpl/datatype.h>
#include <kpl/function.h>
#include <kpl/variable.h>

TEST_CASE( "kpl::Function Tests", "[Something]" )
{
    kpl::reflection::Function func("testFunc", kpl::reflection::DataType("testType", 16));
    
    std::cout << func.name() << std::endl;
}
