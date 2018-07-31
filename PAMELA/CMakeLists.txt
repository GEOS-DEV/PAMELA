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
target_link_libraries(${PROJECT_NAME} ${EXTRA_LIBS})
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


# Expose includes
set(${PROJECT_NAME}_INCLUDE_DIRS ${PROJECT_INC_DIR}
    CACHE INTERNAL "${PROJECT_NAME}: Include Directories" FORCE)





