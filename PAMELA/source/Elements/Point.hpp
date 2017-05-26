#pragma once
#include "Utils/SimpleMaths.hpp"
#include "Elements/Element.hpp"
#include "Utils/Assert.hpp"

template <typename T>
class Collection;


template <>
class Element<ELEMENTS::FAMILY::POINT> : public ElementBase
{
public:

	Element(int index, double x, double y, double z) :ElementBase(), m_coordinates(Coordinates(x, y, z))
	{
		m_family = ELEMENTS::FAMILY::POINT;
	}

	virtual ~Element() = 0 {}

	//Getters
	const Coordinates& get_coordinates() const  { return m_coordinates; }
	Coordinates& get_coordinates() { return m_coordinates; }


protected:

	Coordinates m_coordinates;

};
typedef Element<ELEMENTS::FAMILY::POINT> Point;



template <ELEMENTS::TYPE elementType>
class ElementSpe<ELEMENTS::FAMILY::POINT, elementType> : public Element<ELEMENTS::FAMILY::POINT>
{
public:

	ElementSpe(int index, double x, double y, double z) :Element(index, x, y, z)
	{
		ELEMENTS::nVertex.at(elementType);
		ASSERT(ELEMENTS::TypeToFamily.at(elementType) == m_family, "Type not compatible with family");
		m_vtkType = elementType;
	}

	~ElementSpe() {};

private:


};
typedef ElementSpe<ELEMENTS::FAMILY::POINT, ELEMENTS::TYPE::VTK_VERTEX> Vertex;