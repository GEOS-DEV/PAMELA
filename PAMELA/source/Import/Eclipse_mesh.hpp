#pragma once
#include <vector>
#include "Mesh/UnstructuredMesh.hpp"
#include <fstream>
class File;

namespace PAMELA
{

	enum class ECLIPSE_MESH_TYPE { VERTEX, EDGE, QUADRILATERAL, HEXAHEDRON };

	class Eclipse_mesh
	{

	public:
		static Mesh* CreateMesh(const File file);


	private:

		virtual ~Eclipse_mesh() = 0;//{ };

		static void ParseString(std::string& str);
		static std::string extractDataBelowKeyword(std::istringstream& string_block);
		static Mesh* ConvertMesh();
		static void FillMeshWithProperties(Mesh* mesh);


		static std::string m_label;
		static int m_dimension;
		static int m_nvertices;
		static int m_ntriangles;
		static int m_nquadrilaterals;
		static int m_nhexahedra;

		///Eclipse file data
		//Grid
		static std::vector<int> m_SPECGRID;
		static std::vector<double>  m_COORD;
		static std::vector<double>  m_ZCORN;
		static std::vector<bool>  m_ACTNUM;
		static int  m_nCOORD;
		static int  m_nZCORN;
		static int m_nActiveCells;
		static int m_nTotalCells;

		//Duplicate
		static std::vector<double> m_Duplicate_Element;

		//Properties
		static std::unordered_map<std::string, std::vector<double>> m_Properties;

		static std::unordered_map<ECLIPSE_MESH_TYPE, ELEMENTS::TYPE> m_TypeMap;

		static void InitElementsMapping();


	};

}
