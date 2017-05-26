

execute_process(COMMAND ${PROG_EXE} ${PROG_ARGS}
                 RESULT_VARIABLE res
				 OUTPUT_FILE "${LOG_DIR}/${LOG_NAME}_log.txt")
				
				
message(STATUS "${PROG_ARGS}")				
				
  if(res)
     message(STATUS "Test failed!")
  else()
	 message (STATUS "-------------- The code run fine -------------- ")
  endif()









# execute_process(COMMAND ${TEST_PROG} ${TEST_PROG_ARGS}
                 # RESULT_VARIABLE res)
				
# message(STATUS "${TEST_PROG_ARGS}")				
				
  # if(res)
     # message(FATAL_ERROR "Test failed!")
  # else()
	 # message (STATUS "-------------- The code run fine -------------- ")
  # endif()

  
 # execute_process(COMMAND ${CMAKE_COMMAND} -E compare_files
     # ${FILETOCOMPARE} ${OUTPUT_PATH}
     # RESULT_VARIABLE DIFFERENT)
 # if(DIFFERENT)
     # message(FATAL_ERROR "Test failed - files differ")
 # endif()