message(STATUS "adding example_egrid")
blt_add_executable(NAME                  example_egrid
                   DEFINES               PAMELA_PATH=\"${PAMELA_PATH}\"
                   DEPENDS_ON            PAMELA
                   SOURCES               main.cpp)
