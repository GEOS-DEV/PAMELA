# Try to find PAMELA
# Once done this will define
#
#  PAMELA_FOUND        - system has PAMELA
#  PAMELA_INCLUDE_PATH - include directories for PAMELA (use in include_directories())
#  PAMELA_LIBRARIES    - libraries for PAMELA (use in target_link_libraries())
#
# Variables used by this module. They can change the default behavior and
# need to be set before calling find_package:
#
#  PAMELA_INCLUDE_PATH - Include directory of the PAMELA installation
#                       (set only if different from ${PAMELA_DIR}/include)
#  PAMELA_LIBRARY_PATH - Library directory of the PAMELA installation or build tree
#                       (set only if different from ${PAMELA_DIR}/lib)

find_path(PAMELA_INCLUDE_PATH PAMELA.hpp
  HINTS "${PAMELA_INCLUDE_PATH}" ENV PAMELA_INCLUDE_DIR ENV PAMELA_DIR
  PATH_SUFFIXES include
  DOC "Directory where the PAMELA header files are located"
)

find_library(PAMELA_LIBRARY
  NAMES PAMELA 
  HINTS "${PAMELA_LIBRARY_PATH}" ENV PAMELA_LIB_DIR ENV PAMELA_DIR
  PATH_SUFFIXES lib ${CMAKE_CONFIGURATION_TYPES}
  DOC "Full path to PAMELA library"
)

# Try compiling and running test program
if(PAMELA_INCLUDE_PATH AND PAMELA_LIBRARY)

  # Set flags for building test program
  set(CMAKE_REQUIRED_INCLUDES "${PAMELA_INCLUDE_PATH}")
  set(CMAKE_REQUIRED_LIBRARIES "${PAMELA_LIBRARY}")

  # Rerun the test if it was unsuccessful before in case the user has changed values
  if(NOT PAMELA_TEST_RUNS)
    unset(PAMELA_TEST_RUNS CACHE)
  endif()
  
  # Build and run test program
  include(CheckCSourceRuns)
  check_c_source_runs("
#define PAMELA_EXPORT
#include \"PAMELA.hpp\"
int main(int argc, char* argv[])
{
  // FIXME: Find a simple but sensible test for PAMELA
  return 0;
}
" PAMELA_TEST_RUNS)

  unset(CMAKE_REQUIRED_INCLUDES)
  unset(CMAKE_REQUIRED_LIBRARIES)
endif()

# Standard package handling
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(PAMELA REQUIRED_VARS PAMELA_LIBRARY PAMELA_INCLUDE_PATH PAMELA_TEST_RUNS)

if(PAMELA_FOUND)
  get_filename_component(PAMELA_LIBRARY_DIR "${PAMELA_LIBRARY}" PATH)
  get_filename_component(PAMELA_LIBRARY_NAME "${PAMELA_LIBRARY}" NAME)
  get_filename_component(PAMELA_LIBRARY_NAME_WE "${PAMELA_LIBRARY}" NAME_WE)
  get_filename_component(PAMELA_LIBRARY_EXT "${PAMELA_LIBRARY}" EXT)
  set(PAMELA_LIBRARY_PATH "${PAMELA_LIBRARY_DIR}" CACHE STRING "Path to directory containing PAMELA libraries")
  set(PAMELA_LIBRARIES "${PAMELA_LIBRARY}")
  if(MSVC)
    foreach(CONFIG ${CMAKE_CONFIGURATION_TYPES})
      if("${PAMELA_LIBRARY_DIR}" MATCHES "${CONFIG}/?\$")
        get_filename_component(PAMELA_LIBRARY_DIR "${PAMELA_LIBRARY_DIR}" PATH)
        set(PAMELA_LIBRARY_DIR "${PAMELA_LIBRARY_DIR}/$<CONFIG>")
        break()
      endif()
    endforeach()
    set(PAMELA_LIBRARIES "${PAMELA_LIBRARY_DIR}/${PAMELA_LIBRARY_NAME}")
	set(PAMELA_LIBRARIES "${PAMELA_LIBRARY_DIR}/${PAMELA_LIBRARY_NAME_WE}$<$<CONFIG:Debug>:d>${PAMELA_LIBRARY_EXT}")
  endif()
endif()

set(PAMELA_LIBRARY_PATH "" CACHE STRING "Path to directory containing Mesh_Library libraries")

mark_as_advanced(PAMELA_LIBRARY)
