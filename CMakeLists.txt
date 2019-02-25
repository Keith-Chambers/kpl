cmake_minimum_required(VERSION 3.8)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(kpl)

set(SOURCE_PATH src)
set(TESTS_PATH tests)

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
            ${SOURCE_PATH}/variable.cpp
            ${SOURCE_PATH}/pointertype.cpp
            ${SOURCE_PATH}/referencetype.cpp
            )

add_library(${LIB_NAME} SHARED ${SOURCES})
add_library(${STATIC_LIB_NAME} STATIC ${SOURCES})
            
target_include_directories(${LIB_NAME} PRIVATE ${INCLUDE_PATH})
target_include_directories(${STATIC_LIB_NAME} PRIVATE ${INCLUDE_PATH})

if(TESTS_PATH)
    set(TEST_PROJECT generated_tests)

    add_executable( ${TEST_PROJECT}
                    ${TESTS_PATH}/main.cpp
                    ${TESTS_PATH}/testChainedArray.cpp
                    ${TESTS_PATH}/testPair.cpp
                    ${TESTS_PATH}/testFunction.cpp
                    )

    target_link_libraries(${TEST_PROJECT} ${STATIC_LIB_NAME})

    target_link_libraries(${TEST_PROJECT} "-lfmt")

    target_include_directories(${TEST_PROJECT} PRIVATE ${INCLUDE_PATH})
endif(TESTS_PATH)

# Probably have to do more linking
target_link_libraries(${PROJECT_NAME} "-lsqlite3")

