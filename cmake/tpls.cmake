if(ENABLE_MPI)
   find_package(METIS REQUIRED)
   blt_register_library(NAME METIS
                     INCLUDES ${METIS_INCLUDE_PATH}
                     TREAT_INCLUDES_AS_SYSTEM ON
                     LIBRARIES ${METIS_LIBRARIES})
endif()

#VTK
if(PAMELA_WITH_VTK)
  find_package(VTK REQUIRED COMPONENTS vtkParallelMPI vtkIOParallelXML)
   blt_register_library(NAME VTK
                     INCLUDES ${VTK_INCLUDE_DIRS}
                     TREAT_INCLUDES_AS_SYSTEM ON
                     LIBRARIES ${VTK_LIBRARIES}
                     COMPILE_FLAGS "${VTK_DEFINITIONS} -DWITH_VTK")
   message(STATUS "Found VTK")
   message(STATUS "VTK Include Path: ${VTK_INCLUDE_DIRS}")
   message(STATUS "VTK Libraries: ${VTK_LIBRARIES}")
endif()

