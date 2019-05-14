#pragma once
// Library includes
#include "Elements/Element.hpp"
#include "Collection/Collection.hpp"
#include "MeshDataWriters/Variable.hpp"

#if defined( _WIN32)
#include <direct.h>
#else
#include <sys/types.h>
#include <sys/stat.h>
#endif

namespace PAMELA
{

	////////PARTS
	template <class T>
	struct SubPart
	{
		SubPart(int size, ELEMENTS::TYPE elementtype) { ElementType = elementtype; IndexMapping.reserve(size); SubCollection.reserve(size); }
		ELEMENTS::TYPE ElementType;
		std::vector<int> IndexMapping;  //Subpart to Part
		ElementEnsemble<T, ElementHash<T>, ElementEqual<T>> SubCollection;
	};


	template <class T>
	struct Part
	{
		Part(std::string label, int index, int localIndex, ElementEnsemble<T, ElementHash<T>, ElementEqual<T>>* collection) { Label = label; Collection = collection; Index = index; LocalIndex = localIndex; }
		VariableDouble* AddVariable(VARIABLE_DIMENSION dim, VARIABLE_LOCATION dloc, std::string label)
		{
			if (dloc == VARIABLE_LOCATION::PER_CELL)
			{
				PerElementVariable.push_back(new VariableDouble(dim, label, Collection->size_owned()));
				return PerElementVariable.back();
			}
			if (dloc == VARIABLE_LOCATION::PER_NODE)
			{
				PerNodeVariable.push_back(new VariableDouble(dim, label, static_cast<int>(Points.size())));
				return PerNodeVariable.back();
			}

			return nullptr;

		}

		int Index;   // global including all families
                int LocalIndex; // local index of this family
		std::string Label;
		ElementEnsemble<T, ElementHash<T>, ElementEqual<T>>* Collection;
		std::vector<Point*> Points;
		std::unordered_map<int, int> GlobalToLocalPointMapping;
		std::unordered_map<int, SubPart<T>*> SubParts;
		std::unordered_map<int, int> numberOfElementsPerSubPart
			=
		{
			{ static_cast<int>(ELEMENTS::TYPE::VTK_VERTEX),0 },
			{ static_cast<int>(ELEMENTS::TYPE::VTK_LINE),0 },
			{ static_cast<int>(ELEMENTS::TYPE::VTK_TRIANGLE),0 },
			{ static_cast<int>(ELEMENTS::TYPE::VTK_QUAD) ,0 },
			{ static_cast<int>(ELEMENTS::TYPE::VTK_TETRA),0 },
			{ static_cast<int>(ELEMENTS::TYPE::VTK_HEXAHEDRON) ,0 },
			{ static_cast<int>(ELEMENTS::TYPE::VTK_WEDGE),0 },
			{ static_cast<int>(ELEMENTS::TYPE::VTK_PYRAMID),0 }
		};

		std::vector<VariableDouble*> PerElementVariable;
		std::vector<VariableDouble*> PerNodeVariable;

	}; //TODO: specialize init of numberOfElementsPerSubPart


}
