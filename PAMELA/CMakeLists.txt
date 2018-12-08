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

if(${USE_MPI})
  set(PAMELA_dependencies_list ${PAMELA_dependencies_list} MPI)
  set(PAMELA_dependencies_list ${PAMELA_dependencies_list} METIS)
  set(PAMELA_definitions_list ${PAMELA_definitions_list} -DWITH_METIS)
  set(PAMELA_definitions_list ${PAMELA_definitions_list} -DWITH_MPI)
endif(${USE_MPI})

if(${USE_VTK})
  set(PAMELA_dependencies_list ${PAMELA_dependencies_list} VTK)
    set(PAMELA_definitions_list ${PAMELA_definitions_list} -DWITH_VTK)
endif(${USE_VTK})

set(PAMELA_target PAMELA)

set(PAMELA_headers ${PROJECT_HDR})
set(PAMELA_sources ${PROJECT_SRC})
set(PAMELA_dependencies ${PAMELA_dependencies_list})
set(PAMELA_definitions ${PAMELA_dependencies_list})

message("adding PAMELA_Library")
blt_add_library(NAME                  ${PAMELA_target}
                SOURCES               ${PAMELA_sources}
                HEADERS               ${PAMELA_headers}
                HEADERS_OUTPUT_SUBDIR PAMELA
                DEPENDS_ON             ${PAMELA_dependencies})

				
blt_add_target_definitions(TO ${PAMELA_target} TARGET_DEFINITIONS ${PAMELA_definitions})				
				
target_include_directories(${PAMELA_target} PUBLIC ${CMAKE_CURRENT_LIST_DIR} ${PROJECT_SRC_DIR}) 
	


