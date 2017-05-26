# Try to find METIS
# Once done this will define
#
#  METIS_FOUND        - system has METIS
#  METIS_INCLUDE_PATH - include directories for METIS (use in include_directories())
#  METIS_LIBRARIES    - libraries for METIS (use in target_link_libraries())
#
# Variables used by this module. They can change the default behavior and
# need to be set before calling find_package:
#
#  METIS_INCLUDE_PATH - Include directory of the METIS installation
#                       (set only if different from ${METIS_DIR}/include)
#  METIS_LIBRARY_PATH - Library directory of the METIS installation or build tree
#                       (set only if different from ${METIS_DIR}/lib)

find_path(METIS_INCLUDE_PATH metis.h
  HINTS "${METIS_INCLUDE_PATH}" ENV METIS_INCLUDE_DIR ENV METIS_DIR
  PATH_SUFFIXES include
  DOC "Directory where the METIS header files are located"
)

find_library(METIS_LIBRARY
  NAMES metis
  HINTS "${METIS_LIBRARY_PATH}" ENV METIS_LIB_DIR ENV METIS_DIR
  PATH_SUFFIXES lib ${CMAKE_CONFIGURATION_TYPES}
  DOC "Full path to METIS library"
)

# Try compiling and running test program
if(METIS_INCLUDE_PATH AND METIS_LIBRARY)

  # Set flags for building test program
  set(CMAKE_REQUIRED_INCLUDES "${METIS_INCLUDE_PATH}")
  set(CMAKE_REQUIRED_LIBRARIES "${METIS_LIBRARY}")

  # Rerun the test if it was unsuccessful before in case the user has changed values
  if(NOT METIS_TEST_RUNS)
    unset(METIS_TEST_RUNS CACHE)
  endif()
  
  # Build and run test program
  include(CheckCSourceRuns)
  check_c_source_runs("
#define METIS_EXPORT
#include \"metis.h\"
int main(int argc, char* argv[])
{
  // FIXME: Find a simple but sensible test for METIS
  return 0;
}
" METIS_TEST_RUNS)

  unset(CMAKE_REQUIRED_INCLUDES)
  unset(CMAKE_REQUIRED_LIBRARIES)
endif()

# Standard package handling
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(METIS REQUIRED_VARS METIS_LIBRARY METIS_INCLUDE_PATH METIS_TEST_RUNS)

if(METIS_FOUND)
  get_filename_component(METIS_LIBRARY_DIR "${METIS_LIBRARY}" PATH)
  get_filename_component(METIS_LIBRARY_NAME "${METIS_LIBRARY}" NAME)
  get_filename_component(METIS_LIBRARY_NAME_WE "${METIS_LIBRARY}" NAME_WE)
  get_filename_component(METIS_LIBRARY_EXT "${METIS_LIBRARY}" EXT)
  set(METIS_LIBRARY_PATH "${METIS_LIBRARY_DIR}" CACHE STRING "Path to directory containing METIS libraries")
  set(METIS_LIBRARIES "${METIS_LIBRARY}")
  if(MSVC)
    foreach(CONFIG ${CMAKE_CONFIGURATION_TYPES})
      if("${METIS_LIBRARY_DIR}" MATCHES "${CONFIG}/?\$")
        get_filename_component(METIS_LIBRARY_DIR "${METIS_LIBRARY_DIR}" PATH)
        set(METIS_LIBRARY_DIR "${METIS_LIBRARY_DIR}/$<CONFIG>")
        break()
      endif()
    endforeach()
	set(METIS_LIBRARIES "${METIS_LIBRARY_DIR}/${METIS_LIBRARY_NAME_WE}$<$<CONFIG:Debug>:d>${METIS_LIBRARY_EXT}")
  endif()
endif()

set(METIS_LIBRARY_PATH "" CACHE STRING "Path to directory containing Mesh_Library libraries")

mark_as_advanced(METIS_LIBRARY)
