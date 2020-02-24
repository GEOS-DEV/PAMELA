cmake_minimum_required (VERSION 3.1)

project("PAMELA - PArallel MEsh LibrAry" CXX C)

################################
# BLT
################################
if (NOT BLT_LOADED)
  if (DEFINED BLT_SOURCE_DIR)
      # Support having a shared BLT outside of the repository if given a BLT_SOURCE_DIR

      if (NOT EXISTS ${BLT_SOURCE_DIR}/SetupBLT.cmake)
          message(FATAL_ERROR "Given BLT_SOURCE_DIR does not contain SetupBLT.cmake")
      endif()
  else()
      # Use internal 'blt' submodule path if BLT_SOURCE_DIR not provided
      set(BLT_SOURCE_DIR "${PROJECT_SOURCE_DIR}/cmake/blt" CACHE PATH "")
      if (NOT EXISTS ${BLT_SOURCE_DIR}/SetupBLT.cmake)
          message(FATAL_ERROR
              "Cannot locate BLT. "
              "Either run the following two commands in your git repository: \n"
              "    git submodule init\n"
              "    git submodule update\n"
              "Or add -DBLT_SOURCE_DIR=/path/to/blt to your CMake command." )
      endif()
  endif()

  if (NOT BLT_CXX_STD)
    set(BLT_CXX_STD "c++14")
  endif()

  include(${BLT_SOURCE_DIR}/SetupBLT.cmake)
endif()

# check if PAMELA is build as a submodule or a separate project
get_directory_property(parent_dir PARENT_DIRECTORY)
if(parent_dir)
    set(is_submodule ON)
else()
    set(is_submodule OFF)
endif()

if(NOT is_submodule)

  #Path
  set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake ${CMAKE_MODULE_PATH})

  include(cmake/PAMELA.cmake)
  include(cmake/tpls.cmake)

  #Options
  option(PAMELA_WITH_TESTS "Compile test" OFF)
  option(PAMELA_WITH_EXAMPLES "Compile Examples" OFF)
  option(PAMELA_WITH_VTK "Enable VTK" OFF)

  # Add source
  add_subdirectory(PAMELA)

  #Expose paths
  set(PAMELA_PATH ${CMAKE_SOURCE_DIR} CACHE INTERNAL "PAMELA_PATH" FORCE)

  # Examples
  if(PAMELA_WITH_EXAMPLES)
    add_subdirectory(examples)
  endif(PAMELA_WITH_EXAMPLES)

  #Tests
  if(PAMELA_WITH_TESTS)
    enable_testing()
    add_subdirectory(tests)
  endif(PAMELA_WITH_TESTS)
endif()
if(is_submodule)
  add_subdirectory(PAMELA)
endif()
