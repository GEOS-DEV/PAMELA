#pragma once
#include <vector>
#include "Mesh/UnstructuredMesh.hpp"
#include "Utils/Binary.hpp"

class File;

namespace PAMELA
{

	enum class ECLIPSE_MESH_TYPE { VERTEX, EDGE, QUADRILATERAL, HEXAHEDRON };

	class Eclipse_mesh
	{

	public:
		static Mesh* CreateMeshFromGRDECL(const File file);
		static Mesh* CreateMeshFromEGRID(const File file);

	private:

		virtual ~Eclipse_mesh() = 0;//{ };

		static void ParseStringFromGRDECL(std::string& str);
		static void ParseStringFromEGRID(std::string& str);
		static std::string extractDataBelowKeyword(std::istringstream& string_block);
		//static void get_slb_header(std::istringstream& content);
		//static void get_slb_header(std::istringstream& content, char* keyword, int& n_item, char* keyword_type);
		static Mesh* ConvertMesh();
		static void FillMeshWithProperties(Mesh* mesh);


		static std::string m_label;
		static int m_dimension;
		static int m_nvertices;
		static int m_nquadrilaterals;
		static int m_nhexahedra;

		///Eclipse file data
		//Grid
		static std::vector<int> m_SPECGRID;
		static std::vector<double>  m_COORD;
		static std::vector<double>  m_ZCORN;
		static std::vector<int>  m_ACTNUM;
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

		struct EGRIDDescriptor
		{
			std::string keyword;
			std::string type;
			int dim;
			int first_index;

		};


		//Egrid


		template<class T>
		static void EGRID_ExtractData(std::string& str, int& index, int dim, int type_size, std::vector<T>& output);

		template<class T>
		static void EGRID_ConvertData(std::string keyword, std::vector<T>& data)
		{
			LOGINFO("     o Skipping " + keyword);
		}

		template<>
		static void EGRID_ConvertData<bool>(std::string keyword, std::vector<bool>& data)
		{
			
		}
		template<>
		static void EGRID_ConvertData<double>(std::string keyword, std::vector<double>& data)
		{
			if (keyword == "COORD")
			{
				LOGINFO("     o COORD processed");
				m_COORD = std::vector<double>(data.begin(), data.end());		//TODO not efficient
			}
			else if (keyword == "ZCORN")
			{
				LOGINFO("     o ZCORN processed");
				m_ZCORN = std::vector<double>(data.begin(), data.end());
			}
			else if (keyword == "NNC")
			{
				LOGINFO("     o NNC processed");
			}
			else if (keyword == "PORO")
			{
				LOGINFO("     o PORO processed");
				m_Properties["PORO"] = std::vector<double>(data.begin(), data.end());
			}
			else if (keyword == "PORV")
			{
				LOGINFO("     o PORV processed");
				m_Properties["PORV"] = std::vector<double>(data.begin(), data.end());
			}
			else if (keyword == "PERMX")
			{
				LOGINFO("     o PERMX processed");
				m_Properties["PERMX"] = std::vector<double>(data.begin(), data.end());
			}
			else if (keyword == "PERMY")
			{
				LOGINFO("     o PERMY processed");
				m_Properties["PERMY"] = std::vector<double>(data.begin(), data.end());
			}
			else if (keyword == "PERMZ")
			{
				LOGINFO("     o PERMZ processed");
				m_Properties["PERMZ"] = std::vector<double>(data.begin(), data.end());
			}
			else if (keyword == "DEPTH")
			{
				LOGINFO("     o DEPTH processed");
				m_Properties["DEPTH"] = std::vector<double>(data.begin(), data.end());
			}
			else if (keyword == "NTG")
			{
				LOGINFO("     o NTG processed");
				m_Properties["NTG"] = std::vector<double>(data.begin(), data.end());
			}
			else
			{
				LOGINFO("     o" + keyword+" ignored");
			}

		}
		
		template<>
		static void EGRID_ConvertData<int>(std::string keyword, std::vector<int>& data)
		{
			if (keyword == "GRIDHEAD")
			{
				LOGINFO("     o GRIDHEAD Found");
				m_SPECGRID[0] = data[1];
				m_SPECGRID[1] = data[2];
				m_SPECGRID[2] = data[3];
			}
			else if (keyword == "ACTNUM")
			{
				LOGINFO("     o ACTNUM processed");
				m_ACTNUM =  data;
			}


		}

	};

	template <class T>
	void Eclipse_mesh::EGRID_ExtractData(std::string& str, int& index, int dim, int type_size, std::vector<T>& output)
	{
		output.reserve(dim);

		int block_size = 1000 * type_size;
		int data_size = dim * type_size;
		int nb_full_blocks = data_size / block_size;
		auto buffer = new char[block_size];


		int n_loops;

		//Full blocks
		n_loops = block_size / type_size;
		for (auto nb = 0; nb < nb_full_blocks; ++nb)
		{
			//Skip delimiter
			index = index + 4;

			for (auto n = 0; n < n_loops; ++n)
			{

				T value = *reinterpret_cast<T *>(&str.substr(index, type_size)[0]);
				utils::bites_swap(&value);
				output.push_back(value);
				index = index + type_size;

			}

			//Skip delimiter
			index = index + 4;
		}


		//Remaining partial blocks
		block_size = dim * type_size - nb_full_blocks * block_size;

		n_loops = block_size / type_size;

		if (block_size != 0)
		{
			//Skip delimiter
			index = index + 4;
			for (auto n = 0; n < n_loops; ++n)
			{

				T value = *reinterpret_cast<T *>(&str.substr(index, type_size)[0]);
				utils::bites_swap(&value);
				output.push_back(value);
				index = index + type_size;

			}


			//Skip delimiter
			index = index + 4;
		}


	}


}
