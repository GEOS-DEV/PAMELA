#pragma once

#include "Elements/Polyhedron.hpp"
#include "Elements/Element.hpp"
#include "Elements/Polygon.hpp"
#include "Elements/Line.hpp"
#include "Elements/Point.hpp"

namespace PAMELA
{
  namespace ElementFactory {
    Point* makePoint(ELEMENTS::TYPE  elementType, int index, double x, double y, double z);
    Line*  makeLine(ELEMENTS::TYPE  elementType, int index, const std::vector<Point*>& vertexList);
    Polygon*  makePolygon(ELEMENTS::TYPE  elementType, int index, const std::vector<Point*>& vertexList);
    Polyhedron* makePolyhedron(ELEMENTS::TYPE elementType, int index, const std::vector<Point*>& vertexList);
  }
}
