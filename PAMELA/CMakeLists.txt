cmake_minimum_required (VERSION 3.1)

set(PROJECT_NAME "PAMELA")

project(${PROJECT_NAME} LANGUAGES CXX C)

# C++11 enforced
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Options
set(USE_MPI OFF CACHE BOOL "Enable MPI")
set(USE_VTK OFF CACHE BOOL "Enable VTK")

#Main directory
get_filename_component(PROJECT_DIR "./" ABSOLUTE)
set(PROJECT_BUILD_DIR ${CMAKE_BINARY_DIR}/${PROJECT_NAME})
set(PROJECT_BIN_DIR ${PROJECT_BUILD_DIR}/bin)
set(PROJECT_LIB_DIR ${PROJECT_BUILD_DIR}/lib)
set(PROJECT_SRC_DIR ${PROJECT_DIR}/source)
set(PROJECT_INC_DIR ${PROJECT_DIR}/source)
set(INSTALL_DIR "${PROJECT_DIR}/install")


#Find files
file(GLOB_RECURSE PROJECT_SRC ${PROJECT_SRC_DIR}/*.cpp)
file(GLOB_RECURSE PROJECT_HDR ${PROJECT_SRC_DIR}/*.hpp)
file(GLOB_RECURSE PROJECT_TPP ${PROJECT_SRC_DIR}/*.tpp)
file(GLOB_RECURSE PROJECT_HDRINC ${PROJECT_INC_DIR}/*.hpp)
file(GLOB_RECURSE PROJECT_TPPINC ${PROJECT_INC_DIR}/*.tpp)

#Setup
add_library(${PROJECT_NAME} STATIC ${PROJECT_SRC} ${PROJECT_HDR} ${PROJECT_TPP})
set_target_properties(${PROJECT_NAME} PROPERTIES DEBUG_POSTFIX d)
include_directories(${PROJECT_INC_DIR})

set_target_properties( ${PROJECT_NAME}
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY_DEBUG "${PROJECT_BIN_DIR}"
    LIBRARY_OUTPUT_DIRECTORY_DEBUG "${PROJECT_BIN_DIR}"
    RUNTIME_OUTPUT_DIRECTORY_DEBUG "${PROJECT_BIN_DIR}"
)

# Install
set(CMAKE_INSTALL_PREFIX "INSTALL"  CACHE STRING "Install directory" FORCE)
install(TARGETS ${PROJECT_NAME} EXPORT ${PROJECT_NAME}_Config
  RUNTIME DESTINATION bin
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib)  
install(DIRECTORY ${PROJECT_SRC_DIR}/ DESTINATION include FILES_MATCHING PATTERN "*.hpp" PATTERN "*.h")



#-------------------------------------------- Visual Studio filters
foreach(src IN LISTS PROJECT_HDR PROJECT_TPP PROJECT_SRC)
    file(RELATIVE_PATH rel_scr ${PROJECT_DIR} ${src})
	get_filename_component(fname "${rel_scr}" NAME)
	string(REPLACE "src/" "" src_grp "${rel_scr}")
	string(REPLACE "${fname}" "" src_grp "${src_grp}")
	string(REPLACE "/" "\\" src_grp "${src_grp}")
	source_group("${src_grp}" FILES "${rel_scr}")
endforeach()

if(USE_MPI)
  find_package(MPI)
  if(MPI_CXX_FOUND)
   add_definitions(-DWITH_MPI)
   include_directories("${MPI_CXX_INCLUDE_PATH}")
   target_link_libraries(${PROJECT_NAME} "${MPI_CXX_LIBRARIES}")
   set_target_properties(${PROJECT_NAME} PROPERTIES COMPILE_FLAGS "${MPI_CXX_COMPILE_FLAGS}")
   set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "${MPI_CXX_LINK_FLAGS}")
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
   target_link_libraries(${PROJECT_NAME} ${METIS_LIBRARIES})
   mark_as_advanced(METIS_INCLUDE_PATH METIS_LIBRARY_PATH)
   message(STATUS "Found METIS")
  else()
    message(SEND_ERROR "METIS not found. Set METIS_INCLUDE_PATH and METIS_LIBRARY_PATH")  
  endif()
endif()

#VTK
if(USE_VTK)
  find_package(VTK REQUIRED)
  if(VTK_FOUND)
    add_definitions(-DWITH_VTK)
    #add_definitions(${VTK_DEFINITIONS})
    include(${VTK_USE_FILE})
    #include_directories(${VTK_INCLUDE_DIRS})
    target_link_libraries(${PROJECT_NAME} ${VTK_LIBRARIES})
    message(STATUS "Found VTK")
   else()
     message(SEND_ERROR "VTK not found. Set VTK_INCLUDE_PATH and VTK_LIBRARY_PATH")  
   endif()
endif()

# Expose includes
set(${PROJECT_NAME}_INCLUDE_DIRS ${PROJECT_INC_DIR}
    CACHE INTERNAL "${PROJECT_NAME}: Include Directories" FORCE)





