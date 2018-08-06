cmake_minimum_required (VERSION 3.1)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(PROJECT_NAME "Test")
project(${PROJECT_NAME} C CXX)
message(STATUS "ALLOOOO")


set(TEST_DATA_INPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/data")
set(TEST_DIRECTORY ${CMAKE_SOURCE_DIR}/tests)
set(TEST_DATA_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/output")
set(CTEST_OUTPUT_ON_FAILURE ON)
file(MAKE_DIRECTORY ${TEST_DATA_OUTPUT_DIRECTORY})

set(test_config_file_in ${TEST_DIRECTORY}/tests_config.h.in)
set(test_config_file ${PROJECT_BINARY_DIR}/tests_config.h)
configure_file(${test_config_file_in} ${test_config_file})
	
add_executable("small" small.cpp)
target_link_libraries("small" PAMELA)

add_test(NAME small COMMAND small)

if(${USE_VTK})
    add_definitions(-DWITH_VTK)
endif(${USE_VTK})
include_directories(${PAMELA_INCLUDE_DIRS})
include_directories(${PROJECT_BINARY_DIR})

