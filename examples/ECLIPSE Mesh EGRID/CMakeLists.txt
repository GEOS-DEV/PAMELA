cmake_minimum_required (VERSION 3.1)

set(PROJECT_NAME "EXAMPLE_ECLIPSE_Mesh_EGRID")
project(${PROJECT_NAME} C CXX)
message(STATUS "adding example_egrid")
blt_add_executable(NAME                  example_egrid
                   DEFINES               PAMELA_PATH=\"${PAMELA_PATH}\"
                   DEPENDS_ON            PAMELA
                   SOURCES               main.cpp)
