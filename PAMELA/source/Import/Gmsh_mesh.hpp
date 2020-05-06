#pragma once
#include <vector>
#include "Mesh/UnstructuredMesh.hpp"
#include "Collection/GroupInfo.hpp"
#include <fstream>
namespace PAMELA
{

  enum class GMSH_MESH_TYPE {NODE=0, LINE=1, TRIANGLE=2,QUADRANGLE=3,TETRAHEDRON=4,HEXAHEDRON=5, PRISM=6, PYRAMID=7,POINT=15};

  class Gmsh_mesh
  {

    public:
      Gmsh_mesh() = default;
      Mesh* CreateMesh(const std::string file_path);

    private:
      std::string m_label {""};
      int m_nphysicalregions {0};
      int m_dimension {0};
      int m_nelements {0};
      int m_nnodes {0};
      int m_ntriangles {0};
      int m_nquadrangles {0};
      int m_ntetrahedra {0};
      int m_nhexahedra {0};
      int m_nprisms {0};
      int m_npyramids {0};
      int m_npoints {0};

      /// Key : 
      std::unordered_map<int, GroupInfo> m_tagNamePolygon {};
      std::unordered_map<int, GroupInfo> m_tagNamePolyhedron {};

      void InitElementsMapping();


  };

}
