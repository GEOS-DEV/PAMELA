cmake_minimum_required (VERSION 3.1)
set(PROJECT_NAME "EXAMPLE_CartesianMesh")
project(${PROJECT_NAME} C CXX)
message(STATUS "adding example_cartesian")
blt_add_executable(NAME                  example_cartesian
                   DEPENDS_ON            PAMELA
                   SOURCES               main.cpp)
