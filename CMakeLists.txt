cmake_minimum_required(VERSION 3.8)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(kpl)

set(SOURCE_PATH src)
set(INCLUDE_PATH include)
set(LIB_NAME ${PROJECT_NAME})
set(STATIC_LIB_NAME ${PROJECT_NAME}Static)

set(SOURCES ${SOURCE_PATH}/eventsystem.cpp
            ${SOURCE_PATH}/datatype.cpp
            ${SOURCE_PATH}/function.cpp
            ${SOURCE_PATH}/kdatabase.cpp
            ${SOURCE_PATH}/kfile.cpp
            ${SOURCE_PATH}/kjson.cpp
            ${SOURCE_PATH}/logger.cpp
            ${SOURCE_PATH}/logicaldatatype.cpp
            ${SOURCE_PATH}/util.cpp
            ${SOURCE_PATH}/variable.cpp)

add_library(${LIB_NAME} SHARED ${SOURCES})
add_library(${STATIC_LIB_NAME} STATIC ${SOURCES})
            
target_include_directories(${LIB_NAME} PRIVATE ${INCLUDE_PATH})
target_include_directories(${STATIC_LIB_NAME} PRIVATE ${INCLUDE_PATH})

# Probably have to do more linking
target_link_libraries(${PROJECT_NAME} "-lsqlite3")

