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


#VTK
if(USE_MPI)
  find_package(MPI)
  if(MPI_CXX_FOUND)
   add_definitions(-DWITH_MPI)
   include_directories(${MPI_CXX_INCLUDE_PATH})
   set(EXTRA_LIBS ${EXTRA_LIBS} ${MPI_CXX_LIBRARIES})
   set(EXTRA_COMPILE_FLAGS ${EXTRA_COMPILE_FLAGS} ${MPI_CXX_COMPILE_FLAGS})
   set(EXTRA_LINK_FLAGS ${EXTRA_COMPILE_FLAGS} ${MPI_CXX_LINK_FLAGS})
   mark_as_advanced(FORCE MPI_LIBRARY MPI_EXTRA_LIBRARY MPI_CXX_INCLUDE_PATH MPI_CXX_LIBRARIES)
   message(STATUS "Found MPI")
  else()
    message(SEND_ERROR "MPI not found. Disable USE_MPI or set MPI_CXX_INCLUDE_PATH and MPI_CXX_LIBRARIES")
    set(MPI_CXX_INCLUDE_PATH "${MPI_CXX_INCLUDE_PATH}" CACHE STRING "Path to MPI include directory")
    set(MPI_CXX_LIBRARIES "${MPI_CXX_LIBRARIES}" CACHE STRING "Path to MPI libraries")
  endif()
endif()

if(USE_MPI)
  find_package(METIS)
  if(METIS_FOUND)
   add_definitions(-DWITH_METIS)
   include_directories(${METIS_INCLUDE_PATH})
   set(EXTRA_LIBS ${EXTRA_LIBS} ${METIS_LIBRARIES})
   mark_as_advanced(METIS_INCLUDE_PATH METIS_LIBRARY_PATH)
   message(STATUS "Found METIS")
  else()
    message(SEND_ERROR "METIS not found. Set METIS_INCLUDE_PATH and METIS_LIBRARY_PATH")  
  endif()
endif()
if(USE_VTK)
  find_package(VTK REQUIRED)
  if(VTK_FOUND)
    set(EXTRA_LIBS ${EXTRA_LIBS} ${VTK_LIBRARIES})
   include_directories(${VTK_INCLUDE_DIRS})
    message(STATUS "Found VTK")
   else()
     message(SEND_ERROR "VTK not found. Set VTK_INCLUDE_PATH and VTK_LIBRARY_PATH")  
   endif()
endif()

#PAMELA
add_subdirectory (PAMELA)


# #Examples
if(COMPILE_EXAMPLES)
   add_subdirectory(examples)
endif(COMPILE_EXAMPLES)

# #Tests
# if(COMPILE_TESTS)
   # include(CTest)
   # add_subdirectory(tests)
# endif(COMPILE_TESTS)
