cmake_minimum_required(VERSION 3.8)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(kpl)

set(KPL_USE_QT true)
unset(KPL_USE_QT)

if(KPL_USE_QT)
    add_compile_definitions(KPL_USE_QT=1)
endif()

set(TESTS_PATH tests) # enable tests

set(SOURCE_PATH ${CMAKE_SOURCE_DIR}/source)
set(INCLUDE_PATH ${CMAKE_SOURCE_DIR}/include)
set(LIB_NAME ${PROJECT_NAME})
set(STATIC_LIB_NAME ${PROJECT_NAME}_static)
set(EXTERNAL_PATH ${CMAKE_SOURCE_DIR}/external)
set(BENCHMARK_PROJECT benchmarks)

set(LIB_PATH ${CMAKE_SOURCE_DIR}/lib)
set(BIN_PATH ${CMAKE_SOURCE_DIR}/bin)

set(REFLECTION_SOURCE_PATH ${SOURCE_PATH}/reflection)
set(RELECTION_LIB_NAME ${PROJECT_NAME}-reflection)

set(REFLECTION_SOURCES
        ${REFLECTION_SOURCE_PATH}/datatype.cpp
        ${REFLECTION_SOURCE_PATH}/function.cpp
        ${REFLECTION_SOURCE_PATH}/logicaldatatype.cpp
        ${REFLECTION_SOURCE_PATH}/class.cpp
        ${REFLECTION_SOURCE_PATH}/memberfunction.cpp
        ${REFLECTION_SOURCE_PATH}/membervariable.cpp
        ${REFLECTION_SOURCE_PATH}/accessmodifier.cpp
        ${REFLECTION_SOURCE_PATH}/variable.cpp
)

set(SOURCES
    ${SOURCE_PATH}/core.cpp
    ${SOURCE_PATH}/eventsystem.cpp
    ${SOURCE_PATH}/database.cpp
    ${SOURCE_PATH}/kfile.cpp
    ${SOURCE_PATH}/kjson.cpp
    ${SOURCE_PATH}/logger.cpp
    ${SOURCE_PATH}/util.cpp
    ${SOURCE_PATH}/idgenerator.cpp
#    ${SOURCE_PATH}/filesystem-qt.cpp
)

if(KPL_USE_QT)
    set(SOURCES ${SOURCES} ${SOURCE_PATH}/filesystem-qt.cpp)
endif()

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${LIB_PATH})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${LIB_PATH})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${BIN_PATH})

if(KPL_USE_QT)
    message("kpl configured to use Qt")
    add_compile_options(-fPIC)
endif()

# Core shared object lib
add_library(${LIB_NAME} SHARED ${SOURCES})

target_include_directories(${LIB_NAME} PRIVATE ${INCLUDE_PATH})
target_include_directories(${LIB_NAME} PRIVATE ${EXTERNAL_PATH})

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${BUILD_PATH})

# Core static lib
add_library(${STATIC_LIB_NAME} STATIC ${SOURCES})

target_include_directories(${STATIC_LIB_NAME} PRIVATE ${INCLUDE_PATH})
target_include_directories(${STATIC_LIB_NAME} PRIVATE ${EXTERNAL_PATH})
            
# reflection module
add_library(${RELECTION_LIB_NAME} STATIC ${REFLECTION_SOURCES})

target_include_directories(${RELECTION_LIB_NAME} PRIVATE ${INCLUDE_PATH})
target_include_directories(${RELECTION_LIB_NAME} PRIVATE ${EXTERNAL_PATH})

if(KPL_USE_QT)
    find_package(Qt5 COMPONENTS Core Quick CONFIG REQUIRED)

    target_include_directories(${STATIC_LIB_NAME} PRIVATE ${Qt5Core_INCLUDE_DIRS})
    target_include_directories(${LIB_NAME} PRIVATE ${Qt5Core_INCLUDE_DIRS})
endif()

if(TESTS_PATH)
    set(TEST_PROJECT tests)

    add_executable( ${TEST_PROJECT}
                    ${TESTS_PATH}/main.cpp
                    ${TESTS_PATH}/testChainedArray.cpp
                    ${TESTS_PATH}/testPair.cpp
                    ${TESTS_PATH}/testFunction.cpp
                    ${TESTS_PATH}/testIDGenerator.cpp
    )

    target_include_directories(${TEST_PROJECT} PRIVATE ${INCLUDE_PATH})
    target_include_directories(${TEST_PROJECT} PRIVATE ${EXTERNAL_PATH})

    target_link_libraries(${TEST_PROJECT} ${LIB_PATH}/lib${STATIC_LIB_NAME}.a)
    target_link_libraries(${TEST_PROJECT} ${LIB_PATH}/lib${RELECTION_LIB_NAME}.a)

    target_link_libraries(${TEST_PROJECT} ${EXTERNAL_PATH}/libfmt.a)

    add_executable( ${BENCHMARK_PROJECT}
                    ${TESTS_PATH}/benchmark_chainedarray.cpp
    )

    target_include_directories(${BENCHMARK_PROJECT} PRIVATE ${INCLUDE_PATH})
    target_include_directories(${BENCHMARK_PROJECT} PRIVATE ${EXTERNAL_PATH})

    target_link_libraries(${BENCHMARK_PROJECT} ${LIB_PATH}/lib${STATIC_LIB_NAME}.a)
    target_link_libraries(${BENCHMARK_PROJECT} ${EXTERNAL_PATH}/libfmt.a)
    target_link_libraries(${BENCHMARK_PROJECT} ${EXTERNAL_PATH}/libbenchmark.a)
    target_link_libraries(${BENCHMARK_PROJECT} pthread)

endif(TESTS_PATH)

# Probably have to do more linking
#target_link_libraries(${PROJECT_NAME} "-lsqlite3")

