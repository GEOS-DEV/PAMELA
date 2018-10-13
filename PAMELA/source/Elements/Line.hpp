#pragma once
#include "Elements/Point.hpp"
#include "Utils/SimpleMaths.hpp"
#include "Elements/Element.hpp"
#include "Utils/Assert.hpp"

namespace PAMELA
{

	template <typename T>
	class Collection;

	template<>
	class Element<ELEMENTS::FAMILY::LINE> : public ElementBase
	{
	public:

		Element(int index, const std::vector<Point*>& vertexList) :ElementBase(), m_vertexList(vertexList)
		{
			m_family = ELEMENTS::FAMILY::LINE;
		}

		//virtual ~Element() = 0;// {}

		//Getter
		const std::vector<Point*>& get_vertexList() const { return m_vertexList; }

	protected:

		std::vector<Point*> m_vertexList;

		//Functions
		virtual double get_length() = 0;
		virtual Coordinates get_centroidLocation() = 0;

	};

	typedef Element<ELEMENTS::FAMILY::LINE> Line;

	template <ELEMENTS::TYPE elementType>
	class ElementSpe<ELEMENTS::FAMILY::LINE, elementType> : public Element<ELEMENTS::FAMILY::LINE>
	{
	public:
		ElementSpe(int index, const std::vector<Point*>& vertexList) :Element(index, vertexList)
		{
			ELEMENTS::nVertex.at(elementType);
			ASSERT(ELEMENTS::TypeToFamily.at(elementType) == m_family, "Type not compatible with family");
			m_vtkType = elementType;
		}
		~ElementSpe() {}

	private:

		//Functions
		double get_length() { return 0; }
		Coordinates get_centroidLocation() { return Coordinates(); }

	};

}
