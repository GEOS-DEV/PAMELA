#pragma once
// Library includes
#include "Mesh/Mesh.hpp"
#include "Utils/Types.hpp"
#include "Utils/Communicator.hpp"
#include "Elements/Element.hpp"
#include <iterator> 

#if defined( _WIN32)
#include <direct.h>
#else
#include <sys/types.h>
#include <sys/stat.h>
#endif

namespace PAMELA
{

	enum class FAMILY { POLYHEDRON = 3, POLYGON = 2, LINE = 1, POINT = 0, UNKNOWN = -1 };

	//////VARIABLES
	//Variable types
	enum class VARIABLE_TYPE { UNKNOWN = -1, SCALAR = 1, VECTOR = 3, TENSOR_SYMM = 6 };
	//Variable locations
	enum class VARIABLE_LOCATION { UNKNOWN = -1, PER_NODE = 1, PER_CELL = 2 };
	//Variable size
	const std::unordered_map<VARIABLE_TYPE, int> VariableTypeToSize =
	{
		{ VARIABLE_TYPE::SCALAR,1 },
	{ VARIABLE_TYPE::VECTOR,3 },
	{ VARIABLE_TYPE::TENSOR_SYMM,6 },
	};

	struct Variable
	{
		Variable(VARIABLE_TYPE dtype, std::string label, int size) : Label(label), dType(dtype)
		{
			offset = VariableTypeToSize.at(dtype);
			Data = std::vector<double>(size*offset);
		}

		std::string Label;
		int offset;
		VARIABLE_TYPE dType;

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
			if (offset == 1)
				return{ Data[i] };
			std::vector<double> vec(&Data[i*offset], &Data[(i + 1)*offset - 1]);
			return vec;
		}

	private:
		std::vector<double> Data;

	};


	struct VariableKey
	{
		std::string first;
		std::string second;

		VariableKey(std::string first, std::string second)
		{
			this->first = first;
			this->second = second;
		}

		bool operator==(const VariableKey &other) const
		{
			return (first == other.first
				&& second == other.second);
		}
	};

	struct VariableKeyHash
	{
		std::size_t operator()(const VariableKey& k) const
		{
			auto hfirst = std::hash<std::string>()(k.first);
			auto hsecond = std::hash<std::string>()(k.second);
			return hfirst ^ hsecond;
		}
	};


	////////PARTS
	template <class T>
	struct SubPart
	{
		SubPart(int size, ELEMENTS::TYPE elementtype) { ElementType = elementtype; IndexMapping.reserve(size); SubCollection.reserve(size); }
		ELEMENTS::TYPE ElementType;
		std::vector<int> IndexMapping;  //Subpart to Part
		Ensemble<T, ElementHash<T>, ElementEqual<T>> SubCollection;
	};


	template <class T>
	struct Part
	{
		Part(std::string label, int index, Ensemble<T, ElementHash<T>, ElementEqual<T>>* collection) { Label = label; Collection = collection; Index = index; };
		Variable* AddVariable(VARIABLE_TYPE dtype, VARIABLE_LOCATION dloc, std::string label);

		int Index;   // global including all families
		std::string Label;
		Ensemble<T, ElementHash<T>, ElementEqual<T>>* Collection;
		std::vector<Point*> Points;
		std::unordered_map<int, int> GlobalToLocalPointMapping;
		std::unordered_map<ELEMENTS::TYPE, SubPart<T>*> SubParts;
		std::unordered_map<ELEMENTS::TYPE, int> numberOfElementsPerSubPart
			=
		{
			{ ELEMENTS::TYPE::VTK_VERTEX,0 },
		{ ELEMENTS::TYPE::VTK_LINE,0 },
		{ ELEMENTS::TYPE::VTK_TRIANGLE,0 },
		{ ELEMENTS::TYPE::VTK_QUAD ,0 },
		{ ELEMENTS::TYPE::VTK_TETRA,0 },
		{ ELEMENTS::TYPE::VTK_HEXAHEDRON ,0 },
		{ ELEMENTS::TYPE::VTK_WEDGE,0 },
		{ ELEMENTS::TYPE::VTK_PYRAMID,0 }
		};

		std::vector<Variable*> PerElementVariable;
		std::vector<Variable*> PerNodeVariable;

	};


	template <class T>
	Variable* Part<T>::AddVariable(VARIABLE_TYPE dtype, VARIABLE_LOCATION dloc, std::string label)
	{
		if (dloc == VARIABLE_LOCATION::PER_CELL)
		{
			PerElementVariable.push_back(new Variable(dtype, label, Collection->size_owned()));
			return PerElementVariable.back();
		}
		if (dloc == VARIABLE_LOCATION::PER_NODE)
		{
			PerNodeVariable.push_back(new Variable(dtype, label, static_cast<int>(Points.size())));
			return PerNodeVariable.back();
		}

		return nullptr;

	}



	class MeshDataWriter
	{};

}