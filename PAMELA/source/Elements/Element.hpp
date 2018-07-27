#pragma once
#include <unordered_map>
#include <typeindex>
#include "Collection/Indexing.hpp"
#include "Utils/Assert.hpp"
#include <set>

namespace PAMELA
{

	///VTK ELEMENTS
	namespace ELEMENTS
	{

		//Family
		enum class FAMILY { POLYHEDRON = 3, POLYGON = 2, LINE = 1, POINT = 0, UNKNOWN = -1 };

		const std::unordered_map<FAMILY, int> dimension =
		{
			{ FAMILY::POLYHEDRON ,3 },
			{ FAMILY::POLYGON ,2 },
			{ FAMILY::LINE ,1 },
			{ FAMILY::POINT ,0 },
			{ FAMILY::UNKNOWN ,-1 },
		};

		//LABEL
		const std::unordered_map<FAMILY, std::string> label =
		{
			{ FAMILY::POLYHEDRON ,"POLYHEDRON" },
			{ FAMILY::POLYGON ,"POLYGON" },
			{ FAMILY::LINE ,"LINE" },
			{ FAMILY::POINT ,"POINT" },
			{ FAMILY::UNKNOWN ,"UNKNOWN" },
		};


		//TYPE
		enum class TYPE { UNKNOWN = -1, VTK_VERTEX = 1, VTK_LINE = 3, VTK_TRIANGLE = 5, VTK_QUAD = 9, VTK_TETRA = 10, VTK_HEXAHEDRON = 12, VTK_WEDGE = 13, VTK_PYRAMID = 14 };

		const std::unordered_map<TYPE, int> nVertex =
		{
			{ TYPE::VTK_VERTEX ,1 },
			{ TYPE::VTK_LINE ,1 },
			{ TYPE::VTK_TRIANGLE ,3 },
			{ TYPE::VTK_QUAD ,4 },
			{ TYPE::VTK_TETRA ,4 },
			{ TYPE::VTK_HEXAHEDRON ,8 },
			{ TYPE::VTK_WEDGE ,6 },
			{ TYPE::VTK_PYRAMID ,5 },
			{ TYPE::UNKNOWN ,-1 },
		};

		const std::unordered_map<TYPE, int> nFace =
		{
			{ TYPE::VTK_TETRA ,4 },
			{ TYPE::VTK_HEXAHEDRON ,8 },
			{ TYPE::VTK_WEDGE ,5 },
			{ TYPE::VTK_PYRAMID ,5 },
			{ TYPE::UNKNOWN ,-1 },
		};


		//MAPPING
		const std::unordered_map<TYPE, FAMILY> TypeToFamily =
		{
			{ TYPE::VTK_VERTEX ,FAMILY::POINT },
			{ TYPE::VTK_LINE ,FAMILY::LINE },
			{ TYPE::VTK_TRIANGLE ,FAMILY::POLYGON },
			{ TYPE::VTK_QUAD ,FAMILY::POLYGON },
			{ TYPE::VTK_TETRA ,FAMILY::POLYHEDRON },
			{ TYPE::VTK_HEXAHEDRON ,FAMILY::POLYHEDRON },
			{ TYPE::VTK_WEDGE ,FAMILY::POLYHEDRON },
			{ TYPE::VTK_PYRAMID ,FAMILY::POLYHEDRON }

		};

	};

	/**
	 * \brief Element base class
	 */
	class ElementBase
	{
	public:
		ElementBase() : m_vtkType(ELEMENTS::TYPE::UNKNOWN), m_family(ELEMENTS::FAMILY::UNKNOWN), m_index(-1), m_partitionOwner(0) {}
		//ElementBase(ELEMENTS::FAMILY family,ELEMENTS::TYPE elementType) : m_vtkType(elementType), m_family(family), m_index(-1) {};
		//ElementBase(ELEMENTS::FAMILY family, ELEMENTS::TYPE elementType, int index) : m_vtkType(elementType), m_family(family), m_index(index), m_partitionOwner(0) {};
		//ElementBase(ELEMENTS::FAMILY family, ELEMENTS::TYPE elementType, int index, int partition_owner) : m_vtkType(elementType), m_family(family), m_index(index), m_partitionOwner(partition_owner) {};

		//Getters
		const int get_localIndex() const { return m_index.Local; }
		const int get_globalIndex() const { return m_index.Global; }
		int get_dimension() { return ELEMENTS::dimension.at(m_family); };
		ELEMENTS::FAMILY get_family() { return m_family; }
		ELEMENTS::TYPE get_vtkType() { return m_vtkType; }

		//Setters
		void set_index(int i) { set_localIndex(i); }
		void set_localIndex(int i) { m_index.Local = i; }
		void set_globalIndex(int i) { m_index.Global = i; }
		void set_partionOwner(int i) { m_partitionOwner = i; }

	protected:

		//virtual ~ElementBase() = 0; 
		ELEMENTS::TYPE m_vtkType;
		ELEMENTS::FAMILY m_family;

		//Index
		IndexData m_index;
		int m_partitionOwner;

	};

	template <ELEMENTS::FAMILY family>
	class Element;

	template <ELEMENTS::FAMILY family, ELEMENTS::TYPE elementType>
	class ElementSpe;

}
