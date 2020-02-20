# PAMELA

PAMELA (PArallel MEsh LibrAry) is a C++ library dedicated to the mesh manipulation in parallel.

## Getting started

### Optional dependencies

PAMELA can compile as a standalone but can be used with other libraries:

 * A MPI implementation (e.g. [OpenMPI](https://www.open-mpi.org/) ).
 * [METIS](http://glaros.dtc.umn.edu/gkhome/metis/metis/overview) for mesh partitioning.
 * [VTK](https://vtk.org/download/) for output.

In order to benefit to the full features of PAMELA, we advise the user to compile VTK with the CMake option
`VTK_USE_MPI` set to `On`.

### Download the code

PAMELA uses git and has several submodules. To get the code as long as
