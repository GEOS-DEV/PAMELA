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
		static Mesh* CreateMesh(const std::string file_path);

	private:

		virtual ~INRIA_mesh() = 0;//{ };

		static std::string m_label;
		static int m_dimension;
		static int m_nvertices;
		static int m_ntriangles;
		static int m_nquadrilaterals;
		static int m_ntetrahedra;
		static int m_nhexahedra;

		static std::unordered_map<INRIA_MESH_TYPE, ELEMENTS::TYPE> m_TypeMap;

		static void InitElementsMapping();


	};

}
