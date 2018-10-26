#include "Part.hpp"
#include "Mesh/Mesh.hpp"

namespace PAMELA {
  template <typename T>
    using PartMap = std::unordered_map<std::string, Part<T>*>;

  std::tuple< PartMap<Polyhedron*>, int> getPolyhedronPartMap(Mesh * mesh, int partIndex = 0);
  std::tuple< PartMap<Polygon*>, int> getPolygonPartMap(Mesh * mesh, int partIndex = 0);
  std::tuple< PartMap<Line*>, int> getLinePartMap(Mesh * mesh, int partIndex = 0);
  std::tuple< PartMap<Point*>, int> getPointPartMap(Mesh * mesh, int partIndex = 0);
}
