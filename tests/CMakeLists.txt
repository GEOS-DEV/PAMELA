#Macro to get subdirectories
MACRO(SUBDIRLIST result curdir)
  FILE(GLOB children RELATIVE ${curdir} ${curdir}/*)
  SET(dirlist "")
  FOREACH(child ${children})
    IF(IS_DIRECTORY ${curdir}/${child})
      LIST(APPEND dirlist ${child})
    ENDIF()
  ENDFOREACH()
  SET(${result} ${dirlist})
ENDMACRO()

cmake_minimum_required (VERSION 2.8)
project(Test)
set(SOURCE main.cpp)

add_executable(Test ${SOURCE})
target_link_libraries(Test Parallel_Mesh_Library)

get_filename_component(CURRENT_DIR "./" ABSOLUTE)

#Force verbosity for failure
set(CTEST_OUTPUT_ON_FAILURE "TRUE" CACHE STRING "ss")

#Find .mesh
file(GLOB_RECURSE LIST_MESH_FILES ${CURRENT_DIR}/*.mesh)

#List of subdirectories = tests
SUBDIRLIST(SUBDIRS ${CURRENT_DIR})


#Create tests
 set(i 0)
 set(TEST_DIR ${CMAKE_CURRENT_LIST_DIR})
 foreach(filename ${LIST_MESH_FILES})
     LIST(GET SUBDIRS ${i} CURRENT_TEST_DIR)
	 if (NOT CURRENT_TEST_DIR STREQUAL "logs")
		message (STATUS "Test_${i} based on ${filename} created" )
		SET(REF_PATH ${CURRENT_DIR}/${CURRENT_TEST_DIR}/reference/Unstructured_Mesh.geo)
		SET(OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR}/Ensight_Gold_output/Unstructured_Mesh.geo)
		if(EXISTS ${REF_PATH})
		else()
			message (FATAL_ERROR "REFERENCE FILE ${REF_PATH} does not exist")
		endif()
		add_test(NAME "Test_${i}"
		COMMAND ${CMAKE_COMMAND}
		-DPROG_EXE=$<TARGET_FILE:Test>
		-DPROG_ARGS=${filename}
		-DPROG_OUTPUT_PATH=${OUTPUT_PATH}
		-DPROG_FILE_TO_COMPARE=${REF_PATH}
		-DLOG_NAME=${CURRENT_TEST_DIR}
		-DLOG_DIR=${TEST_DIR}/logs
		-P ${CMAKE_CURRENT_SOURCE_DIR}/runtest.cmake)
		set_tests_properties("Test_${i}" PROPERTIES VERBOSE 1)
	endif()
	math(EXPR i "${i} + 1")	
 endforeach()



	


