#pragma once
#include <vector>
#include "Mesh/UnstructuredMesh.hpp"
#include <fstream>
namespace PAMELA
{

  enum class INRIA_MESH_TYPE { VERTEX, TRIANGLE, EDGE, QUADRILATERAL, TETRAHEDRON, PENTRAHEDRON, HEXAHEDRON };

  class INRIA_mesh
  {

    public:
      INRIA_mesh() = default;
      Mesh* CreateMesh(const std::string file_path);

    private:
      std::string m_label {""};
      int m_dimension {0};
      int m_nvertices {0};
      int m_ntriangles {0};
      int m_nquadrilaterals {0};
      int m_ntetrahedra {0};
      int m_nhexahedra {0};

      std::unordered_map<int, ELEMENTS::TYPE> m_TypeMap {};

      void InitElementsMapping();
  };

}
