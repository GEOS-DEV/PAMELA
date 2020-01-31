message(STATUS "adding example_grdecl")
blt_add_executable(NAME                  example_grdecl
                   DEFINES               PAMELA_PATH=\"${PAMELA_PATH}\"
                   DEPENDS_ON            PAMELA
                   SOURCES                main.cpp)
