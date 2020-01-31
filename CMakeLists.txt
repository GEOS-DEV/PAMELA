cmake_minimum_required (VERSION 3.1)


# check if PAMELA is build as a submodule or a separate project
get_directory_property(parent_dir PARENT_DIRECTORY)
if(parent_dir)
    set(is_submodule ON)
else()
    set(is_submodule OFF)
endif()

if(NOT is_submodule)
  project("PAMELA - PArallel MEsh LibrAry" CXX C)

  # Check if BLT is available
  if (NOT EXISTS ${PROJECT_SOURCE_DIR}/cmake/blt/SetupBLT.cmake)
    message(FATAL_ERROR
      "The BLT submodule is not present. "
      "Run the following two commands in your git repository: \n"
      "    git submodule init \n "
      "    git submodule update" )
  endif()

  #Path
  set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake ${CMAKE_MODULE_PATH})

  set(BLT_CXX_STD "c++14")
  include(cmake/blt/SetupBLT.cmake)
  include(cmake/PAMELA.cmake)

  #Options
  option(PAMELA_WITH_TESTS "Compile test" OFF)
  option(PAMELA_WITH_EXAMPLES "Compile Examples" OFF)
  option(PAMELA_WITH_VTK "Enable VTK" OFF)

  include(cmake/blt/SetupBLT.cmake)
  include(cmake/PAMELA.cmake)

  include(cmake/tpls.cmake)
  
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
