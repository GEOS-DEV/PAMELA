#pragma once
// Library includes
#include "Mesh/Mesh.hpp"
#include "Utils/Types.hpp"
#include "Utils/Communicator.hpp"
#include "Elements/Element.hpp"
#if defined( _WIN32)
#include <direct.h>
#else
#include <sys/types.h>
#include <sys/stat.h>
#endif

namespace PAMELA
{

	//TODO variables in parallel, vector and tensors data

	namespace EnsightGold
	{

		enum class ENSIGHT_GOLD_TYPE { UNKNOWN = -1, ESG_POINT = 1, ESG_BAR2 = 2, ESG_TRIA3 = 4, ENS_QUAD4 = 6, ENS_TETRA4 = 8, ENS_PYRAMID5 = 10, ENS_PENTA6 = 12, ENS_HEXA8 = 14 };
		enum class FAMILY { POLYHEDRON = 3, POLYGON = 2, LINE = 1, POINT = 0, UNKNOWN = -1 };

		//Maps
		const std::unordered_map<ELEMENTS::TYPE, ENSIGHT_GOLD_TYPE> VTKToEnsightGold
			=
		{
			{ ELEMENTS::TYPE::VTK_VERTEX ,ENSIGHT_GOLD_TYPE::ESG_POINT },
			{ ELEMENTS::TYPE::VTK_LINE ,ENSIGHT_GOLD_TYPE::ESG_BAR2 },
			{ ELEMENTS::TYPE::VTK_TRIANGLE ,ENSIGHT_GOLD_TYPE::ESG_TRIA3 },
			{ ELEMENTS::TYPE::VTK_QUAD ,ENSIGHT_GOLD_TYPE::ENS_QUAD4 },
			{ ELEMENTS::TYPE::VTK_TETRA ,ENSIGHT_GOLD_TYPE::ENS_TETRA4 },
			{ ELEMENTS::TYPE::VTK_HEXAHEDRON ,ENSIGHT_GOLD_TYPE::ENS_HEXA8 },
			{ ELEMENTS::TYPE::VTK_WEDGE ,ENSIGHT_GOLD_TYPE::ENS_PENTA6 },
			{ ELEMENTS::TYPE::VTK_PYRAMID ,ENSIGHT_GOLD_TYPE::ENS_PYRAMID5 }
		};

		const std::unordered_map<ENSIGHT_GOLD_TYPE, std::string> ElementToLabel
			=
		{
			{ ENSIGHT_GOLD_TYPE::ESG_POINT,"point" },
			{ ENSIGHT_GOLD_TYPE::ESG_BAR2,"bar2" },
			{ ENSIGHT_GOLD_TYPE::ESG_TRIA3,"tria3" },
			{ ENSIGHT_GOLD_TYPE::ENS_QUAD4,"quad4" },
			{ ENSIGHT_GOLD_TYPE::ENS_TETRA4,"tetra4" },
			{ ENSIGHT_GOLD_TYPE::ENS_HEXA8,"hexa8" },
			{ ENSIGHT_GOLD_TYPE::ENS_PENTA6,"penta6" },
			{ ENSIGHT_GOLD_TYPE::ENS_PYRAMID5,"pyramid5" }
		};


		const std::unordered_map<ENSIGHT_GOLD_TYPE, FAMILY> TypeToFamily =
		{
			{ ENSIGHT_GOLD_TYPE::ESG_POINT,FAMILY::POINT },
			{ ENSIGHT_GOLD_TYPE::ESG_BAR2,FAMILY::LINE },
			{ ENSIGHT_GOLD_TYPE::ESG_TRIA3,FAMILY::POLYGON },
			{ ENSIGHT_GOLD_TYPE::ENS_QUAD4,FAMILY::POLYGON },
			{ ENSIGHT_GOLD_TYPE::ENS_TETRA4,FAMILY::POLYHEDRON },
			{ ENSIGHT_GOLD_TYPE::ENS_HEXA8,FAMILY::POLYHEDRON },
			{ ENSIGHT_GOLD_TYPE::ENS_PENTA6,FAMILY::POLYHEDRON },
			{ ENSIGHT_GOLD_TYPE::ENS_PYRAMID5,FAMILY::POLYHEDRON }
		};

		enum class ENSIGHT_GOLD_VARIABLE_TYPE { UNKNOWN = -1, SCALAR = 1, VECTOR = 3, TENSOR_SYMM = 6 };
		enum class ENSIGHT_GOLD_VARIABLE_LOCATION { UNKNOWN = -1, PER_NODE = 1, PER_CELL = 2 };

		const std::unordered_map<ENSIGHT_GOLD_VARIABLE_TYPE, int> VariableTypeToSize =
		{
			{ ENSIGHT_GOLD_VARIABLE_TYPE::SCALAR,1 },
			{ ENSIGHT_GOLD_VARIABLE_TYPE::VECTOR,3 },
			{ ENSIGHT_GOLD_VARIABLE_TYPE::TENSOR_SYMM,6 },
		};

		struct Variable
		{
			Variable(ENSIGHT_GOLD_VARIABLE_TYPE dtype, std::string label, int size) : Label(label), dType(dtype)
			{
				offset = VariableTypeToSize.at(dtype);
				Data = std::vector<double>(size*offset);
			}

			std::string Label;
			int offset;
			ENSIGHT_GOLD_VARIABLE_TYPE dType;

			void set_data(double cst) 
			{
				std::fill(Data.begin(), Data.end(), cst);
			}

			void set_data(std::vector<double> vec)
			{
				ASSERT(vec.size() == Data.size(), "Mismatch sizes");
				Data = vec;
			}

			std::vector<double> get_data(int i)
			{
				std::vector<double> vec(&Data[i*offset], &Data[(i+1)*offset-1]);
				return vec;
			}

		private:
			std::vector<double> Data;

		};


		template <class T>
		struct SubPart
		{
			SubPart(int size, ENSIGHT_GOLD_TYPE elementtype) { ElementType = elementtype; IndexMapping.reserve(size); SubCollection.reserve(size); }
			ENSIGHT_GOLD_TYPE ElementType;
			std::vector<int> IndexMapping;  //Subpart to Part
			Ensemble<T> SubCollection;
		};


		template <class T>
		struct Part
		{
			Part(std::string label, int index, Ensemble<T>* collection) { Label = label; Collection = collection; Index = index; }
			Variable* AddVariable(ENSIGHT_GOLD_VARIABLE_TYPE dtype, ENSIGHT_GOLD_VARIABLE_LOCATION dloc, std::string label);

			int Index;   // global including all families
			std::string Label;
			Ensemble<T>* Collection;
			std::set<Point*> Points;
			std::unordered_map<int, int> GlobalToLocalPointMapping;
			std::unordered_map<ENSIGHT_GOLD_TYPE, SubPart<T>*> SubParts;
			std::unordered_map<ENSIGHT_GOLD_TYPE, int> numberOfElementsPerSubPart
				=
			{
				{ ENSIGHT_GOLD_TYPE::ESG_POINT,0 },
				{ ENSIGHT_GOLD_TYPE::ESG_BAR2,0 },
				{ ENSIGHT_GOLD_TYPE::ESG_TRIA3,0 },
				{ ENSIGHT_GOLD_TYPE::ENS_QUAD4,0 },
				{ ENSIGHT_GOLD_TYPE::ENS_TETRA4,0 },
				{ ENSIGHT_GOLD_TYPE::ENS_HEXA8,0 },
				{ ENSIGHT_GOLD_TYPE::ENS_PENTA6,0 },
				{ ENSIGHT_GOLD_TYPE::ENS_PYRAMID5,0 }
			};

			std::vector<Variable*> PerElementVariable;
			std::vector<Variable*> PerNodeVariable;

		};


		class EnsightGoldWriter
		{

			template <typename T>
			using PartMap = std::unordered_map<std::string, Part<T>*>;

		public:

			EnsightGoldWriter(Mesh* mesh, std::string name) :m_mesh(mesh), m_name(name), m_partition(Communicator::worldRank()), m_nPartition(Communicator::worldSize()) { Init(); }

			void CreateVariable(FAMILY family, ENSIGHT_GOLD_VARIABLE_TYPE dtype, ENSIGHT_GOLD_VARIABLE_LOCATION dloc, std::string name, std::string part);
			void MakeCaseFile();
			void MakeGeoFile();

			void SetVariable(std::string label, double univalue);
			void SetVariable(std::string label, const std::vector<double>& values);
			void DumpVariables();


		private:

			void Init();

			template<typename T>
			void FillParts(std::string prefixLabel, PartMap<T>* parts);

			//Geo
			void MakeGeoFile_Header();
			template<typename T>
			void MakeGeoFile_AddParts(const PartMap<T>* parts);
			template<typename T>
			void DumpVariables_Parts(const PartMap<T>* parts);

			std::string PartitionNumberForExtension();
			std::string TimeStepNumberForExtension();

			Mesh* m_mesh;
			std::string m_name;
			std::ofstream m_caseFile;
			std::ofstream m_geoFile;
			int m_nDigitsExtensionPartition;
			int m_nDigitsExtensionTime;

			Types::uint_t m_partition;
			Types::uint_t m_nPartition;

			double m_currentTime;
			int m_currentTimeStep;

			//Parts
			PartMap<Point*>   m_PointParts;
			PartMap<Line*>  m_LineParts;
			PartMap<Polygon*>  m_PolygonParts;
			PartMap<Polyhedron*>  m_PolyhedronParts;

			//Property
			std::unordered_map<std::string, Variable*> m_Variable;

		};


		template <class T>
		Variable* Part<T>::AddVariable(ENSIGHT_GOLD_VARIABLE_TYPE dtype, ENSIGHT_GOLD_VARIABLE_LOCATION dloc, std::string label)
		{
			if (dloc == ENSIGHT_GOLD_VARIABLE_LOCATION::PER_CELL)
			{
				PerElementVariable.push_back(new Variable(dtype, label, Collection->size_owned()));
				return PerElementVariable.back();
			}
			if (dloc == ENSIGHT_GOLD_VARIABLE_LOCATION::PER_NODE)
			{
				PerNodeVariable.push_back(new Variable(dtype, label, static_cast<int>(Points.size())));
				return PerNodeVariable.back();
			}

			return nullptr;

		}

		/**
		 * \brief
		 * \tparam T
		 * \param parts
		 */
		template<typename T>
		void EnsightGoldWriter::FillParts(std::string prefixLabel, PartMap<T>* parts)
		{
			//--Iterate over polygon parts
			for (auto it = parts->begin(); it != parts->end(); ++it)	//Loop over group and act on active groups
			{
				auto partptr = it->second;

				//Update Label
				partptr->Label = prefixLabel + "_" + partptr->Label;

				//----Count number of elements per part
				for (auto it2 = partptr->Collection->begin_owned(); it2 != partptr->Collection->end_owned(); ++it2)
				{
					auto vtkType = (*it2)->get_vtkType();
					auto ensightGoldType = EnsightGold::VTKToEnsightGold.at(vtkType);
					partptr->numberOfElementsPerSubPart[ensightGoldType] = partptr->numberOfElementsPerSubPart.at(ensightGoldType) + 1;
				}
				//----Create as many subparts as there are different elements
				for (auto it2 = partptr->numberOfElementsPerSubPart.begin(); it2 != partptr->numberOfElementsPerSubPart.end(); ++it2)
				{
					partptr->SubParts[it2->first] = new SubPart<T>(it2->second, it2->first);
				}
				//----Fill subparts with elements
				for (auto it2 = partptr->Collection->begin_owned(); it2 != partptr->Collection->end_owned(); ++it2)
				{
					auto vtkType = (*it2)->get_vtkType();
					auto ensightGoldType = EnsightGold::VTKToEnsightGold.at(vtkType);
					partptr->SubParts[ensightGoldType]->SubCollection.push_back_owned_unique(*it2);
				}

				//----Mapping from local to global
				int id = 0;
				for (auto it2 = partptr->Collection->begin_owned(); it2 != partptr->Collection->end_owned(); ++it2)
				{
					auto vtkType = (*it2)->get_vtkType();
					auto ensightGoldType = EnsightGold::VTKToEnsightGold.at(vtkType);
					auto subpart = partptr->SubParts[ensightGoldType];
					//Map local indexes
					subpart->IndexMapping.push_back(id);
					id++;

					//Insert vertices
					auto vertexlist = (*it2)->get_vertexList();
					int vertexsize = static_cast<int>(vertexlist.size());
					for (auto i = 0; i != vertexsize; ++i)
					{
						partptr->Points.insert(vertexlist[i]);
					}
				}
				//----Map Point Coordinates
				int i = 0;
				for (auto it2 = partptr->Points.begin(); it2 != partptr->Points.end(); ++it2)
				{
					partptr->GlobalToLocalPointMapping[(*it2)->get_globalIndex()] = i;
					i++;
				}

			}

		}


		/**
		 * \brief
		 * \tparam T
		 * \param ipart
		 * \param parts
		 * \param prefixLabel
		 */
		template <typename T>
		void EnsightGoldWriter::MakeGeoFile_AddParts(const PartMap<T>* parts)
		{

			for (auto it = parts->begin(); it != parts->end(); ++it)			//Loop over Parts that are defined from the mesh input
			{

				auto partptr = it->second;

				m_geoFile << "part" << std::endl;
				m_geoFile << std::setw(10);
				m_geoFile << partptr->Index << std::endl;
				m_geoFile << partptr->Label << std::endl;

				//Coordinates
				m_geoFile << "coordinates" << std::endl;
				m_geoFile << std::setw(10);
				m_geoFile << partptr->Points.size() << std::endl;


				//--Id
				for (auto it2 = partptr->Points.begin(); it2 != partptr->Points.end(); ++it2)
				{
					m_geoFile << std::setw(10);
					m_geoFile << (*it2)->get_globalIndex() << std::endl;
				}

				//--x
				for (auto it2 = partptr->Points.begin(); it2 != partptr->Points.end(); ++it2)		//Loop over Points 
				{
					m_geoFile << std::setw(12);
					m_geoFile << (*it2)->get_coordinates().x << std::endl;
				}

				//--y
				for (auto it2 = partptr->Points.begin(); it2 != partptr->Points.end(); ++it2)		//Loop over Points 
				{
					m_geoFile << std::setw(12);
					m_geoFile << (*it2)->get_coordinates().y << std::endl;
				}

				//--z
				for (auto it2 = partptr->Points.begin(); it2 != partptr->Points.end(); ++it2)		//Loop over Points 
				{
					m_geoFile << std::setw(12);
					m_geoFile << (*it2)->get_coordinates().z << std::endl;
				}

				//Elements

				for (auto it2 = partptr->SubParts.begin(); it2 != partptr->SubParts.end(); ++it2)
				{
					if (it2->second->SubCollection.size_owned() > 0)
					{
						auto subpart = it2->second;
						auto elementType = subpart->ElementType;
						std::string ENSTypeLabel = EnsightGold::ElementToLabel.at(elementType);
						m_geoFile << ENSTypeLabel << std::endl;
						m_geoFile << std::setw(10);
						//----dimension
						m_geoFile << subpart->SubCollection.size_owned() << std::endl;
						//----Id
						for (auto it3 = subpart->SubCollection.begin_owned(); it3 != subpart->SubCollection.end_owned(); ++it3)
						{
							m_geoFile << std::setw(10) << (*it3)->get_globalIndex() << std::endl;
						}
						//----Connectivity
						for (auto it3 = subpart->SubCollection.begin_owned(); it3 != subpart->SubCollection.end_owned(); ++it3)
						{
							auto VertexList = (*it3)->get_vertexList();
							auto nVertex = VertexList.size();
							for (auto j = 0; j != nVertex; ++j)
							{
								m_geoFile << std::setw(10) << partptr->GlobalToLocalPointMapping.at(VertexList[j]->get_globalIndex()) + 1;
							}
							m_geoFile << std::endl;
						}
					}
				}
			}

		}

		template <typename T>
		void EnsightGoldWriter::DumpVariables_Parts(const PartMap<T>* parts)
		{

			//Polyhedron
			for (auto it = parts->begin(); it != parts->end(); ++it)			//Loop over Parts that are defined from the mesh input
			{
				auto partptr = it->second;


				//PerElementVariables
				for (auto it2 = partptr->PerElementVariable.begin(); it2 != partptr->PerElementVariable.end(); ++it2)
				{

					auto variableptr = (*it2);

					//Create file
					std::ofstream variableFile;
					variableFile.open("./" + partptr->Label + "_" + variableptr->Label + "_" + TimeStepNumberForExtension(), std::fstream::in | std::fstream::out | std::fstream::trunc);

					//write
					variableFile << variableptr->Label << std::endl;
					variableFile << "part" << std::endl;
					variableFile << std::setw(10);
					variableFile << partptr->Index << std::endl;


					for (auto it3 = partptr->SubParts.begin(); it3 != partptr->SubParts.end(); ++it3)
					{
						if (it3->second->SubCollection.size_owned() > 0)
						{
							auto subpart = it3->second;
							auto elementType = subpart->ElementType;
							std::string ENSTypeLabel = EnsightGold::ElementToLabel.at(elementType);
							variableFile << ENSTypeLabel << std::endl;
							variableFile << std::setw(10);
							for (auto it4 = subpart->SubCollection.begin_owned(); it4 != subpart->SubCollection.end_owned();  ++it4)
							{
								auto collectionIndex = it4 - subpart->SubCollection.begin_owned();
								auto variableIndex = subpart->IndexMapping[collectionIndex];
								auto variableData = variableptr->get_data(variableIndex);
								for (auto it5 = variableData.begin(); it5 != variableData.end(); ++it5)
								{
									variableFile << std::setw(12) << (*it5) << std::endl;
								}
							}

						}
					}

				}

				//PerNodeVariables
				for (auto it2 = partptr->PerNodeVariable.begin(); it2 != partptr->PerNodeVariable.end(); ++it2)
				{

					auto variableptr = (*it2);

					//Create file
					std::ofstream variableFile;
					variableFile.open("./" + partptr->Label + "_" + variableptr->Label + "_" + TimeStepNumberForExtension(), std::fstream::in | std::fstream::out | std::fstream::trunc);

					//write
					variableFile << variableptr->Label << std::endl;
					variableFile << "part" << std::endl;
					variableFile << std::setw(10);
					variableFile << partptr->Index << std::endl;
					variableFile << "coordinates" << std::endl;
					for (auto it3 = partptr->Points.begin(); it3 != partptr->Points.end(); ++it3)
					{
						auto collectionIndex = std::distance(partptr->Points.begin(), it3);
						auto variableData = variableptr->get_data(collectionIndex);
						for (auto it5 = variableData.begin(); it5 != variableData.end(); ++it5)
						{
							variableFile << std::setw(12) << (*it5) << std::endl;
						}
					}


					for (auto it3 = partptr->SubParts.begin(); it3 != partptr->SubParts.end(); ++it3)
					{
						if (it3->second->SubCollection.size_owned() > 0)
						{
							auto subpart = it3->second;
							auto elementType = subpart->ElementType;
							std::string ENSTypeLabel = EnsightGold::ElementToLabel.at(elementType);
							variableFile << ENSTypeLabel << std::endl;
							variableFile << std::setw(10);
							for (auto it4 = subpart->SubCollection.begin_owned(); it4 != subpart->SubCollection.end_owned(); ++it4)
							{
								auto collectionIndex = it4 - subpart->SubCollection.begin_owned();
								auto variableIndex = subpart->IndexMapping[collectionIndex];
								auto variableData = variableptr->get_data(collectionIndex);
								for (auto it5 = variableData.begin(); it5 != variableData.end(); ++it5)
								{
									variableFile << std::setw(12) << (*it5) << std::endl;
								}

							}

						}
					}

				}

			}



		}
	}

}