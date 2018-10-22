set(PAMELA_target PAMELA)

set(PAMELA_headers
  Elements/ElementFactory.hpp
  Elements/Element.hpp
  Elements/Point.hpp
  Elements/Polygon.hpp
  Elements/Line.hpp
  Elements/Polyhedron.hpp
  Utils/Assert.hpp
  Utils/StringUtils.hpp
  Utils/LogMessage.hpp
  Utils/Types.hpp
  Utils/HashMap.hpp
  Utils/Utils.hpp
  Utils/Binary.hpp
  Utils/SimpleMaths.hpp
  Utils/File.hpp
  Utils/Resource.hpp
  Utils/Statistics.hpp
  Utils/Logger.hpp
  Utils/VectorUtils.hpp
  Adjacency/Adjacency.hpp
  Adjacency/CSRMatrix.hpp
  Adjacency/CSRGraph.hpp
  Adjacency/AdjacencySet.hpp
  PAMELA.hpp
  MeshDataWriters/MeshDataWriterFactory.hpp
  MeshDataWriters/EnsightGoldWriter.hpp
  MeshDataWriters/VTKWriter.hpp
  MeshDataWriters/Variable.hpp
  MeshDataWriters/Part.hpp
  MeshDataWriters/MeshDataWriter.hpp
  MeshDataWriters/MeshParts.hpp
  Mesh/MeshFactory.hpp
  Mesh/UnstructuredMesh.hpp
  Mesh/Mesh.hpp
  Mesh/CartesianMesh.hpp
  Parallel/ParallelEnsemble.hpp
  Parallel/Communicator.hpp
  Import/Gmsh_mesh.hpp
  Import/INRIA_mesh.hpp
  Import/Eclipse_mesh.hpp
  Collection/Collection.hpp
  Collection/Indexing.hpp
  Collection/ElementEnsemble.hpp
  Property/Property.hpp)

set(PAMELA_sources
  Elements/ElementFactory.cpp
  Utils/Logger.cpp
  Utils/Statistics.cpp
  Utils/File.cpp
  Utils/StringUtils.cpp
  Utils/LogMessage.cpp
  Utils/VectorUtils.cpp
  Adjacency/CSRMatrix.cpp
  Adjacency/Adjacency.cpp
  Adjacency/AdjacencySet.cpp
  MeshDataWriters/MeshDataWriter.cpp
  MeshDataWriters/VTKWriter.cpp
  MeshDataWriters/MeshDataWriterFactory.cpp
  MeshDataWriters/EnsightGoldWriter.cpp
  MeshDataWriters/MeshParts.cpp
  Mesh/CartesianMesh.cpp
  Mesh/MeshFactory.cpp
  Mesh/Mesh.cpp
  Parallel/Communicator.cpp
  Import/Eclipse_mesh.cpp
  Import/INRIA_mesh.cpp
  Import/Gmsh_mesh.cpp
  Collection/Collection.cpp)

if(${USE_MPI})
  set(pamela_dependencies_list ${pamela_dependencies_list} mpi)
  set(pamela_dependencies_list ${pamela_dependencies_list} METIS)
endif(${USE_MPI})

if(${USE_VTK})
  set(pamela_dependencies_list ${pamela_dependencies_list} VTK)
endif(${USE_VTK})
set(PAMELA_dependencies ${pamela_dependencies_list})

message("adding PAMELA_Library")
blt_add_library(NAME                  ${PAMELA_target}
                SOURCES               ${PAMELA_sources}
                HEADERS               ${PAMELA_headers}
                HEADERS_OUTPUT_SUBDIR PAMELA
                DEPENDS_ON             ${PAMELA_dependencies})

target_include_directories(${PAMELA_target} PUBLIC ${CMAKE_CURRENT_LIST_DIR}) 
