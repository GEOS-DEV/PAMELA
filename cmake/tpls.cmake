if(USE_MPI)
    find_package(MPI REQUIRED)

    message(STATUS "MPI C Compile Flags:  ${MPI_C_COMPILE_FLAGS}")
    message(STATUS "MPI C Include Path:   ${MPI_C_INCLUDE_PATH}")
    message(STATUS "MPI C Link Flags:     ${MPI_C_LINK_FLAGS}")
    message(STATUS "MPI C Libraries:      ${MPI_C_LIBRARIES}")

    message(STATUS "MPI CXX Compile Flags: ${MPI_CXX_COMPILE_FLAGS}")
    message(STATUS "MPI CXX Include Path:  ${MPI_CXX_INCLUDE_PATH}")
    message(STATUS "MPI CXX Link Flags:    ${MPI_CXX_LINK_FLAGS}")
    message(STATUS "MPI CXX Libraries:     ${MPI_CXX_LIBRARIES}")

blt_register_library(NAME mpi
                     INCLUDES ${MPI_C_INCLUDE_PATH} ${MPI_CXX_INCLUDE_PATH}
                     TREAT_INCLUDES_AS_SYSTEM ON
                     LIBRARIES ${MPI_C_LIBRARIES} ${MPI_CXX_LIBRARIES}
                     COMPILE_FLAGS "${MPI_C_COMPILE_FLAGS} -DWITH_MPI"
                     LINK_FLAGS    "${MPI_C_COMPILE_FLAGS}")
endif()

if(USE_MPI)
  find_package(METIS REQUIRED)
   message(STATUS "Found METIS")
   blt_register_library(NAME METIS
                     INCLUDES ${METIS_INCLUDE_PATH}
                     TREAT_INCLUDES_AS_SYSTEM ON
                     LIBRARIES ${METIS_LIBRARIES}
                     COMPILE_FLAGS "-DWITH_METIS")
endif()

#VTK
if(USE_VTK)
  find_package(VTK REQUIRED COMPONENTS vtkParallelMPI vtkIOParallelXML)
   blt_register_library(NAME VTK
                     INCLUDES ${VTK_INCLUDE_DIRS}
                     TREAT_INCLUDES_AS_SYSTEM ON
                     LIBRARIES ${VTK_LIBRARIES}
                     COMPILE_FLAGS "${VTK_DEFINITIONS} -DWITH_VTK")
    message(STATUS "Found VTK")
endif()

