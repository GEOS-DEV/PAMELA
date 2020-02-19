message(STATUS "adding example_cartesian")

blt_add_executable(NAME                  example_cartesian
                   DEPENDS_ON            PAMELA
                   SOURCES               main.cpp)
