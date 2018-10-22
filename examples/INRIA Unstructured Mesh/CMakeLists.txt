cmake_minimum_required (VERSION 3.1)
set(PROJECT_NAME "EXAMPLE_INRIAUnstructuredMesh")
project(${PROJECT_NAME} C CXX)
message(STATUS "adding example_inria")
blt_add_executable(NAME                  example_inria
                   DEFINES               PAMELA_PATH=\"${PAMELA_PATH}\"
                   DEPENDS_ON            PAMELA
                   SOURCES                main.cpp)
