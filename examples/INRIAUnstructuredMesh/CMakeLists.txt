message(STATUS "adding example_inria")
blt_add_executable(NAME                  example_inria
                   DEFINES               PAMELA_PATH=\"${PAMELA_PATH}\"
                   DEPENDS_ON            PAMELA
                   SOURCES                main.cpp)
