cmake_minimum_required (VERSION 3.1)

# # Options
# set(USE_MPI OFF CACHE BOOL "Enable MPI")

# C++11 enforced
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(PROJECT_NAME "EXAMPLE_INRIAUnstructuredMesh")
project(${PROJECT_NAME} C CXX)
set(SOURCE main.cpp)
add_executable(${PROJECT_NAME} ${SOURCE})
target_link_libraries(${PROJECT_NAME} PAMELA)
target_compile_definitions(${PROJECT_NAME} PUBLIC "${PAMELA_DEFINITIONS}")
target_compile_definitions(${PROJECT_NAME} PUBLIC -DPAMELA_PATH="${PAMELA_PATH}")
target_include_directories(${PROJECT_NAME} PUBLIC "${PAMELA_INCLUDE_DIRS}")


