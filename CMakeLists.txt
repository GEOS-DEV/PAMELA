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

  include(cmake/blt/SetupBLT.cmake)
  include(cmake/PAMELA.cmake)

  #Options
  option(COMPILE_TESTS "Compile test" OFF)
  option(COMPILE_EXAMPLES "Compile Examples" OFF)

  set(USE_MPI OFF CACHE BOOL "Enable MPI")
  set(USE_VTK OFF CACHE BOOL "Enable VTK")

  include(cmake/tpls.cmake)

  add_subdirectory(PAMELA)

  #Expose important paths
  set(PAMELA_PATH ${CMAKE_SOURCE_DIR} CACHE INTERNAL "PAMELA_PATH" FORCE)

  # Examples
  if(COMPILE_EXAMPLES)
    add_subdirectory(examples)
  endif(COMPILE_EXAMPLES)

  #Tests
  if(COMPILE_TESTS)
    enable_testing()
    add_subdirectory(tests)
  endif(COMPILE_TESTS)
elseif(NOT is_submodule)
  add_subdirectory(PAMELA)
endif(NOT is_submodule)
