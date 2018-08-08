cmake_minimum_required (VERSION 3.1)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(PROJECT_NAME "Test")
project(${PROJECT_NAME} C CXX)


set(TEST_DATA_INPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/data")
set(TEST_DIRECTORY ${CMAKE_SOURCE_DIR}/tests)
set(TEST_DATA_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/output")
set(CTEST_OUTPUT_ON_FAILURE ON)
file(MAKE_DIRECTORY ${TEST_DATA_OUTPUT_DIRECTORY})

set(test_config_file_in ${TEST_DIRECTORY}/tests_config.h.in)
set(test_config_file ${PROJECT_BINARY_DIR}/tests_config.h)
configure_file(${test_config_file_in} ${test_config_file})
	
add_executable("TEST_small" small.cpp)
target_link_libraries("TEST_small" PAMELA)
target_compile_definitions("TEST_small" PUBLIC "${PAMELA_DEFINITIONS}")
target_include_directories("TEST_small" PUBLIC "${PROJECT_BINARY_DIR}")
target_include_directories("TEST_small" PUBLIC "${PAMELA_INCLUDE_DIRS}")
add_test(NAME small COMMAND small)

add_executable("TEST_medium" medium.cpp)
target_link_libraries("TEST_medium" PAMELA)
target_compile_definitions("TEST_medium" PUBLIC "${PAMELA_DEFINITIONS}")
target_include_directories("TEST_medium" PUBLIC "${PROJECT_BINARY_DIR}")
target_include_directories("TEST_medium" PUBLIC "${PAMELA_INCLUDE_DIRS}")
add_test(NAME medium COMMAND medium)

add_executable("TEST_big" big.cpp)
target_link_libraries("TEST_big" PAMELA)
target_compile_definitions("TEST_big" PUBLIC "${PAMELA_DEFINITIONS}")
target_include_directories("TEST_big" PUBLIC "${PROJECT_BINARY_DIR}")
target_include_directories("TEST_big" PUBLIC "${PAMELA_INCLUDE_DIRS}")
add_test(NAME big COMMAND big)


