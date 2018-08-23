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
	};


        template<>
            void Eclipse_mesh::EGRID_ConvertData<bool>(std::string keyword, std::vector<bool>& data);

        template<>
            void Eclipse_mesh::EGRID_ConvertData<double>(std::string keyword, std::vector<double>& data);

        template<>
            void Eclipse_mesh::EGRID_ConvertData<int>(std::string keyword, std::vector<int>& data);

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
