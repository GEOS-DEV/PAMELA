#include "Elements/ElementFactory.hpp"
#include "Utils/Logger.hpp"
#include "Utils/Assert.hpp"
namespace PAMELA
{

	Point* ElementFactory::makePoint(ELEMENTS::TYPE elementType, int index, double x, double y, double z)
	{
		ASSERT(elementType == ELEMENTS::TYPE::VTK_VERTEX, "Cannot make a point with this type");
		return new Vertex(index, x, y, z);
	}

	Line* ElementFactory::makeLine(ELEMENTS::TYPE elementType, int index, const std::vector<Point*>& vertexList)
	{
		ASSERT(elementType == ELEMENTS::TYPE::VTK_LINE, "Cannot make a line with this type");
		return new ElementSpe<ELEMENTS::FAMILY::LINE, ELEMENTS::TYPE::VTK_LINE>(index, vertexList);
	}

	Polygon* ElementFactory::makePolygon(ELEMENTS::TYPE elementType, int index, const std::vector<Point*>& vertexList)
	{

		ASSERT(ELEMENTS::TypeToFamily.at(static_cast<int>(elementType)) == ELEMENTS::FAMILY::POLYGON, "Element type is not a polygon");

		switch (elementType)
		{
		case ELEMENTS::TYPE::VTK_TRIANGLE:
			return new ElementSpe<ELEMENTS::FAMILY::POLYGON, ELEMENTS::TYPE::VTK_TRIANGLE>(index, vertexList);

		case ELEMENTS::TYPE::VTK_QUAD:
			return new ElementSpe<ELEMENTS::FAMILY::POLYGON, ELEMENTS::TYPE::VTK_QUAD>(index, vertexList);

		default:
			LOGERROR("Element type is unknown");
			return nullptr;
		}

	}

	Polyhedron* ElementFactory::makePolyhedron(ELEMENTS::TYPE elementType, int index, const std::vector<Point*>& vertexList)
	{

		ASSERT(ELEMENTS::TypeToFamily.at(static_cast<int>(elementType)) == ELEMENTS::FAMILY::POLYHEDRON, "Element type is not a polyhedron");

		switch (elementType)
		{

		case ELEMENTS::TYPE::VTK_HEXAHEDRON:
			return new ElementSpe<ELEMENTS::FAMILY::POLYHEDRON, ELEMENTS::TYPE::VTK_HEXAHEDRON>(index, vertexList);

		case ELEMENTS::TYPE::VTK_TETRA:
			return new ElementSpe<ELEMENTS::FAMILY::POLYHEDRON, ELEMENTS::TYPE::VTK_TETRA>(index, vertexList);

		case ELEMENTS::TYPE::VTK_PYRAMID:
			return new ElementSpe<ELEMENTS::FAMILY::POLYHEDRON, ELEMENTS::TYPE::VTK_PYRAMID>(index, vertexList);

		case ELEMENTS::TYPE::VTK_WEDGE:
			return new ElementSpe<ELEMENTS::FAMILY::POLYHEDRON, ELEMENTS::TYPE::VTK_WEDGE>(index, vertexList);

		default:
			LOGERROR("Element type is unknown");
			return nullptr;
		}
	}
}
