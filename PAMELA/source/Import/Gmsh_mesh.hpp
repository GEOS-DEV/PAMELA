#pragma once
#include <vector>
#include "Mesh/UnstructuredMesh.hpp"
#include <fstream>
namespace PAMELA
{

	enum class GMSH_MESH_TYPE {NODE=0, LINE=1, TRIANGLE=2,QUADRANGLE=3,TETRAHEDRON=4,HEXAHEDRON=5, PRISM=6, PYRAMID=7,POINT=15};

	class Gmsh_mesh
	{

	public:
		static Mesh* CreateMesh(const std::string file_path);

	private:
		static std::string m_label;
		static int m_nphysicalregions;
		static int m_dimension;
		static int m_nelements;
		static int m_nnodes;
		static int m_ntriangles;
		static int m_nquadrangles;
		static int m_ntetrahedra;
		static int m_nhexahedra;
		static int m_nprisms;
		static int m_npyramids;
		static int m_npoints;

		static std::unordered_map<int, std::string> m_TagNamePolygon;
		static std::unordered_map<int, std::string> m_TagNamePolyhedron;

		static void InitElementsMapping();


	};

}
