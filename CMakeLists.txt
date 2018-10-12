#Definitions
cmake_minimum_required (VERSION 3.1)

project("PAMELA - PArallel MEsh LibrAry" CXX C)

include(cmake/blt/SetupBLT.cmake)

include(cmake/PAMELA.cmake)

#Path
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake ${CMAKE_MODULE_PATH})

#Options
option(COMPILE_TESTS "Compile test" OFF)
option(COMPILE_EXAMPLES "Compile Examples" OFF)

#Expose important paths
set(PAMELA_PATH ${CMAKE_SOURCE_DIR} CACHE INTERNAL "PAMELA_PATH" FORCE)

#PAMELA
add_subdirectory(PAMELA)

# Examples
if(COMPILE_EXAMPLES)
   add_subdirectory(examples)
endif(COMPILE_EXAMPLES)

#Tests
if(COMPILE_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif(COMPILE_TESTS)
