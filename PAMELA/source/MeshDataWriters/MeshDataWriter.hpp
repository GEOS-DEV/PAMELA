#pragma once
// Library includes
#include "MeshDataWriters/Part.hpp"
#include "MeshDataWriters/Variable.hpp"
#include "Parallel/Communicator.hpp"
#if defined( _WIN32)
#include <direct.h>
#else
#include <sys/types.h>
#include <sys/stat.h>
#endif

namespace PAMELA
{

	enum class FAMILY { POLYHEDRON = 3, POLYGON = 2, LINE = 1, POINT = 0, UNKNOWN = -1 };

	class Mesh;

	class MeshDataWriter
	{
		template <typename T>
		using PartMap = std::unordered_map<std::string, Part<T>*>;

	public:

		MeshDataWriter(Mesh * mesh, std::string name);

		virtual void Init() = 0;

		void DeclareVariable(FAMILY family, VARIABLE_TYPE dtype, VARIABLE_LOCATION dloc, std::string name, std::string part);
		void DeclareVariable(FAMILY family, VARIABLE_TYPE dtype, VARIABLE_LOCATION dloc, std::string name);
		template<class T>
		void SetVariable(std::string label, T univalue)
		{
			
				for (auto const& part : m_PolyhedronParts)
				{
					if (m_Variable.find(VariableKey(label, part.first))!=m_Variable.end())
					{
						auto var = m_Variable.at(VariableKey(label, part.first));
						var->set_data(univalue);
					}
				}

				for (auto const& part : m_PolygonParts)
				{
					if (m_Variable.find(VariableKey(label, part.first)) != m_Variable.end())
					{
						auto var = m_Variable.at(VariableKey(label, part.first));
						var->set_data(univalue);
					}
				}

		}
		template<class T>
		void SetVariable(std::string label, std::string part, T univalue)
		{
			auto var = m_Variable.at(VariableKey(label, part));
			var->set_data(univalue);
		}

		template<class T>
		void SetVariable(std::string label, ParallelEnsemble<T>& values)
		{
			auto var = m_Variable.at(VariableKey(label, m_PolyhedronParts.begin()->first));
			var->set_data(values.begin_owned(), values.end_owned());
		}

		template<class T>
		void SetVariable(std::string label, std::string part, ParallelEnsemble<T>& values)
		{
			auto var = m_Variable.at(VariableKey(label, part));
			var->set_data(values);
		}

		virtual void DumpVariables() = 0;

	
	protected:

		std::string PartitionNumberForExtension();
		std::string TimeStepNumberForExtension();

		Mesh* m_mesh;

		std::string m_name;

		Types::uint_t m_partition;
		Types::uint_t m_nPartition;

		double m_currentTime;
		int m_currentTimeStep;

		int m_nDigitsExtensionPartition;
		int m_nDigitsExtensionTime;
		
		//Parts
		PartMap<Point*>   m_PointParts;
		PartMap<Line*>  m_LineParts;
		PartMap<Polygon*>  m_PolygonParts;
		PartMap<Polyhedron*>  m_PolyhedronParts;

		template<typename T>
		void FillParts(std::string prefixLabel, PartMap<T>* parts);

		//Property
		std::unordered_map<VariableKey, Variable*, VariableKeyHash> m_Variable;
	
	};

	/**
	* \brief
	* \tparam T
	* \param parts
	*/
	template<typename T>
	void MeshDataWriter::FillParts(std::string prefixLabel, PartMap<T>* parts)
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
				partptr->numberOfElementsPerSubPart[vtkType] = partptr->numberOfElementsPerSubPart.at(vtkType) + 1;
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
				//auto ensightGoldType = EnsightGold::VTKToEnsightGold.at(vtkType);
				partptr->SubParts[vtkType]->SubCollection.push_back_owned_unique(*it2);
			}

			//----Mapping from local to global
			int id = 0;
			for (auto it2 = partptr->Collection->begin_owned(); it2 != partptr->Collection->end_owned(); ++it2)
			{
				auto vtkType = (*it2)->get_vtkType();
				//auto ensightGoldType = EnsightGold::VTKToEnsightGold.at(vtkType);
				auto subpart = partptr->SubParts[vtkType];
				//Map local indexes
				subpart->IndexMapping.push_back(id);
				id++;

				//std::set<Point*> PointsSet;

				//Insert vertices
				auto vertexlist = (*it2)->get_vertexList();
				int vertexsize = static_cast<int>(vertexlist.size());
				for (auto i = 0; i != vertexsize; ++i)
				{
					partptr->Points.push_back(vertexlist[i]);
				}

			}

			std::sort(partptr->Points.begin(), partptr->Points.end());
			partptr->Points.erase(std::unique(partptr->Points.begin(), partptr->Points.end()), partptr->Points.end());


			//----Map Point Coordinates
			int i = 0;
			for (auto it2 = partptr->Points.begin(); it2 != partptr->Points.end(); ++it2)
			{
				partptr->GlobalToLocalPointMapping[(*it2)->get_globalIndex()] = i;
				i++;
			}

		}

	}

}