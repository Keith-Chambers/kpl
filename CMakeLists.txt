cmake_minimum_required(VERSION 3.8)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCE_PATH src)
set(INCLUDE_PATH include)

project(kpl)

add_library(${PROJECT_NAME} STATIC 
            ${SOURCE_PATH}/eventsystem.cpp
            ${SOURCE_PATH}/function.cpp
            ${SOURCE_PATH}/kdatabase.cpp
            ${SOURCE_PATH}/kfile.cpp
            ${SOURCE_PATH}/kjson.cpp
            ${SOURCE_PATH}/logger.cpp
            ${SOURCE_PATH}/logicaldatatype.cpp
            ${SOURCE_PATH}/util.cpp
            ${SOURCE_PATH}/variable.cpp)
            
target_include_directories(${PROJECT_NAME} PRIVATE ${INCLUDE_PATH})
