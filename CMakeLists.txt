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
option(COMPILE_EXAMPLES "Compile examples" OFF)
option(COMPILE_TESTS "Compile test" OFF)

#PAMELA
add_subdirectory (PAMELA)

#Visualization
#if(COMPILE_VISUALIZATION)
#   add_subdirectory (visualization)
#endif(COMPILE_VISUALIZATION)


# #Examples
if(COMPILE_EXAMPLES)
   add_subdirectory(examples)
endif(COMPILE_EXAMPLES)

# #Tests
# if(COMPILE_TESTS)
   # include(CTest)
   # add_subdirectory(tests)
# endif(COMPILE_TESTS)



