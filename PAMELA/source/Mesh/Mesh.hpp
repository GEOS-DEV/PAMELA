#pragma once
#include <vector>
#include "Elements/Point.hpp"
#include "Elements/Line.hpp"
#include "Elements/Polygon.hpp"
#include "Elements/Polyhedron.hpp"
#include "Collection/Collection.hpp"
#include "Utils/types.hpp"
#include "Elements/Line.hpp"


class Adjacency;
class MeshAdjacency;

class Mesh
{
public:
	virtual ~Mesh() = default;

	Mesh();

	//Getters
	PointCollection*  get_PointCollection()  {return &m_PointCollection;}
	LineCollection*  get_LineCollection()  { return &m_LineCollection; }
	PolygonCollection*  get_PolygonCollection()  { return &m_PolygonCollection; }
	PolyhedronCollection*  get_PolyhedronCollection()  { return &m_PolyhedronCollection; }

	//Adjacency
	MeshAdjacency* getMeshAdjacency() const;
	
	virtual void Distort(double alpha){};

	////Updaters
	void CreateFacesFromCells();

	///Functions to add elements or group to the mesh
	//Add Element
	Point* addPoint(ELEMENTS::TYPE elementType, int elementIndex, std::string groupLabel, double x, double y, double z);
	Line* addLine(ELEMENTS::TYPE elementType, int elementIndex, std::string groupLabel, const std::vector<Point*>& vertexList);
	Polygon* addPolygon(ELEMENTS::TYPE elementType, int elementIndex, std::string groupLabel, const std::vector<Point*>& vertexList);
	Polyhedron* addPolyhedron(ELEMENTS::TYPE elementType, int elementIndex, std::string groupLabel, const std::vector<Point*>& vertexList);

	///Partitioning 
	// This is a graph-based partitioning followed by the add of ghost elements according to ghostBaseElement parameter.
	void PerformPolyhedronPartitioning(ELEMENTS::FAMILY edgeElement, ELEMENTS::FAMILY ghostBaseElement);
	std::vector<int> METISPartitioning(Adjacency* adjacency, int npartition);
	std::vector<int> TRIVIALPartitioning();


protected:

	//Element Collections - First owned then ghosts
	PointCollection m_PointCollection;
	LineCollection m_LineCollection;
	PolygonCollection m_PolygonCollection;
	PolyhedronCollection m_PolyhedronCollection;

	//Adjacency
	MeshAdjacency* m_adjacency;

	void ShrinkMesh(const std::set<int>& Polyhedron_owned, const std::set<int>& vector);
};


////////////////////////////////////////////////////////////////////////////////////////
