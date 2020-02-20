# PAMELA

PAMELA (PArallel MEsh LibrAry) is a C++ library dedicated to the mesh manipulation in parallel.

## Features

### Mesh reader

PAMELA provides tools to read meshes from several formats
 * [GMSH v2](http://gmsh.info/doc/texinfo/gmsh.html#MSH-file-format-version-2-_0028Legacy_0029)
 * [INRIA MEDIT Mesh](https://people.sc.fsu.edu/~jburkardt/data/medit/medit.html)
 * ECLIPSE file formats (.GRDECL and .EGRID).


### Mesh partitioner

PAMELA provides a basic tools to partitionate meshes. PAMELA can also use METIS to
do the partitioning.

### Mesh cleaner

PAMELA is able to perform several operations related to mesh cleaning. If PAMELA is compiled
with MPI, these operations are done in parallel.

### Mesh converter

PAMELA provides tools to write meshes to several formats:
  * [VTK](https://vtk.org)
  * Ensight Gold

## Getting started

### Mandatory prerequisties

 * [CMake](https://cmake.org/)
 * A C++11 compiler.

### Optional dependencies

PAMELA can compile as a standalone but can be used with other libraries:

 * A MPI implementation (e.g. [OpenMPI](https://www.open-mpi.org/) ).
 * [METIS](http://glaros.dtc.umn.edu/gkhome/metis/metis/overview) for mesh partitioning.
 * [VTK](https://vtk.org/download/) for output.

In order to benefit to the full features of PAMELA, we advise the user to compile VTK with the CMake option
`VTK_USE_MPI` set to `On`.

### Download PAMELA

PAMELA uses git and has several submodules. To get the code, please ensure that the git lfs module is installed on your system. Instructions to get git lfs can be found [here](https://github.com/git-lfs/git-lfs/wiki/Installation).

```sh
git clone git@github.com:GEOSX/PAMELA.git
git submodule init
git submodule update
```

### Configure and build PAMELA

PAMELA uses CMake as a build system generator and 4 options you can choose to activate or not

#### `ENABLE_MPI`

To use PAMELA with MPI

#### `PAMELA_WITH_EXAMPLES`

To compile the PAMELA use cases

#### `PAMELA_WITH_VTK`

To use VTK for ouptut

#### `PAMELA_WITH_METIS`

To use METIS to partitionate the mesh.

For instance:

```sh
mkdir build
cd build
cmake .. -DENABLE_MPI=ON -DPAMELA_WITH_EXAMPLES=ON -DPAMELA_WITH_VTK=ON -DPAMELA_WITH_VTK=ON -DPAMELA_WITH_METIS=ON
make
```

### Use PAMELA

Examples in the folder `examples/` are showing how to use PAMELA.
