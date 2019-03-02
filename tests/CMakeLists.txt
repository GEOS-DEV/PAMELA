set(gtest_pamela_tests
    small.cpp
    big.cpp
    medium.cpp)

foreach(test ${gtest_pamela_tests})
    get_filename_component( test_name ${test} NAME_WE )
    blt_add_executable( NAME ${test_name}
                        SOURCES ${test}
                        OUTPUT_DIR ${TEST_OUTPUT_DIRECTORY}
                        DEFINES PAMELA_PATH=\"${PAMELA_PATH}\"
                        DEPENDS_ON PAMELA gtest
                        )

    blt_add_test( NAME ${test_name} 
                  COMMAND ${test_name}
                  )
endforeach()
