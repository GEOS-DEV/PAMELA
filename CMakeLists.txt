#Definitions
cmake_minimum_required (VERSION 3.1)


set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)
set(CMAKE_DISABLE_SOURCE_CHANGES  ON)
set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_COLOR_MAKEFILE   ON)

project("PAMELA - PArallel MEsh LibrAry" LANGUAGES CXX C)

#Path
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake ${CMAKE_MODULE_PATH})

#Options
option(COMPILE_TESTS "Compile test" OFF)
option(COMPILE_EXAMPLES "Compile Examples" OFF)
option(COMPILE_EXECUTABLE "Compile executable" ON)

#PAMELA
add_subdirectory (PAMELA)

# Examples
if(COMPILE_EXAMPLES)
   add_subdirectory(examples)
endif(COMPILE_EXAMPLES)

#Tests
if(COMPILE_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif(COMPILE_TESTS)

