cmake_minimum_required (VERSION 3.1)
set(PROJECT_NAME "EXAMPLE_ECLIPSE_Mesh_GRDECL")
project(${PROJECT_NAME} C CXX)
message(STATUS "adding example_grdecl")
blt_add_executable(NAME                  example_grdecl
                   DEFINES               PAMELA_PATH=\"${PAMELA_PATH}\"
                   DEPENDS_ON            PAMELA
                   SOURCES                main.cpp)
