message(STATUS "adding example_gmsh")
blt_add_executable(NAME                  example_gmsh
                   DEFINES               PAMELA_PATH=\"${PAMELA_PATH}\"
                   DEPENDS_ON            PAMELA
                   SOURCES                main.cpp)
