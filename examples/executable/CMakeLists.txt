cmake_minimum_required (VERSION 3.1)
set(PROJECT_NAME "EXAMPLE_executable")
project(${PROJECT_NAME} C CXX)
message(STATUS "adding example_inria")
blt_add_executable(NAME                  pamela
                   DEPENDS_ON            PAMELA
                   SOURCES               pamela.cpp)
