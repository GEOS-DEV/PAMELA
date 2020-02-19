# set IDE filters
set(PROJECT_SRC_DIR ${CMAKE_CURRENT_LIST_DIR}/source)
set(PROJECT_INC_DIR ${CMAKE_CURRENT_LIST_DIR}/source)

file(GLOB_RECURSE PROJECT_SRC ${PROJECT_SRC_DIR}/*.cpp)
file(GLOB_RECURSE PROJECT_HDR ${PROJECT_SRC_DIR}/*.hpp)

foreach(src IN LISTS PROJECT_HDR PROJECT_SRC)
    file(RELATIVE_PATH rel_scr ${CMAKE_CURRENT_LIST_DIR} ${src})
	get_filename_component(fname "${rel_scr}" NAME)
	string(REPLACE "src/" "" src_grp "${rel_scr}")
	string(REPLACE "${fname}" "" src_grp "${src_grp}")
	string(REPLACE "/" "\\" src_grp "${src_grp}")
	source_group("${src_grp}" FILES "${rel_scr}")
endforeach()

# Expose includes
set(${PROJECT_NAME}_INCLUDE_DIRS ${PROJECT_INC_DIR}
    CACHE INTERNAL "${PROJECT_NAME}: Include Directories" FORCE)

if(${ENABLE_MPI})
  set(PAMELA_dependencies_list ${PAMELA_dependencies_list} MPI)
  set(PAMELA_dependencies_list ${PAMELA_dependencies_list} METIS)
  set(PAMELA_definitions_list ${PAMELA_definitions_list} "-DWITH_MPI")
  set(PAMELA_definitions_list ${PAMELA_definitions_list} "-DWITH_METIS")
endif(${ENABLE_MPI})

if(${PAMELA_WITH_VTK})
  set(PAMELA_dependencies_list ${PAMELA_dependencies_list} VTK)
   set(PAMELA_definitions_list ${PAMELA_definitions_list} "-DWITH_VTK")
 endif(${PAMELA_WITH_VTK})

if(NOT ${GEOSX_TPL_DIR} STREQUAL "" )
  message(STATUS "PAMELA is configured with GEOSX !")
  if(ENABLE_METIS)
    message(STATUS "Configuring PAMELA with metis of GEOSX tpls")
    set(PAMELA_dependencies_list ${PAMELA_dependencies_list} metis)
    set(PAMELA_definitions_list ${PAMELA_definitions_list} "-DWITH_METIS")
  endif(ENABLE_METIS)
  if(ENABLE_MPI)
    message(STATUS "Configuring PAMELA with MPI")
    set(PAMELA_dependencies_list ${PAMELA_dependencies_list} mpi)
    set(PAMELA_definitions_list ${PAMELA_definitions_list} "-DWITH_MPI")
  endif(ENABLE_MPI)
endif(NOT ${GEOSX_TPL_DIR} STREQUAL "")

set(PAMELA_dependencies ${pamela_dependencies_list})
set(PAMELA_target PAMELA)
set(PAMELA_headers ${PROJECT_HDR})
set(PAMELA_sources ${PROJECT_SRC})
set(PAMELA_dependencies ${PAMELA_dependencies_list})
set(PAMELA_definitions ${PAMELA_definitions_list})

message(STATUS "adding PAMELA_Library")
message(STATUS "PAMELA with following definitions : ${PAMELA_definitions}")
blt_add_library(NAME                  ${PAMELA_target}
                SOURCES               ${PAMELA_sources}
                HEADERS               ${PAMELA_headers}
                HEADERS_OUTPUT_SUBDIR PAMELA
                DEFINES               ${PAMELA_definitions}
                DEPENDS_ON             ${PAMELA_dependencies})

				
target_include_directories(${PAMELA_target} PUBLIC ${CMAKE_CURRENT_LIST_DIR} ${PROJECT_SRC_DIR}) 
	


