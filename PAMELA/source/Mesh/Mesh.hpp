#pragma once
#include <vector>
#include "Elements/Point.hpp"
#include "Elements/Line.hpp"
#include "Elements/Polygon.hpp"
#include "Elements/Polyhedron.hpp"
#include "Collection/Collection.hpp"
#include "Property/Property.hpp"
#include "Adjacency/TopologicalAdjacency.hpp"

namespace PAMELA
{

	class Adjacency;

	class Mesh
	{
	public:
		virtual ~Mesh();
		Mesh();

		//Getters
		PointCollection*  get_PointCollection() { return &m_PointCollection; }
		LineCollection*  get_LineCollection() { return &m_LineCollection; }
		PolygonCollection*  get_PolygonCollection() { return &m_PolygonCollection; }
		PolyhedronCollection*  get_PolyhedronCollection() { return &m_PolyhedronCollection; }

		Property<PolyhedronCollection, double>* get_PolyhedronProperty_double() const { return m_PolyhedronProperty_double; }
		Property<PolyhedronCollection, int>* get_PolyhedronProperty_int() const { return m_PolyhedronProperty_int; }

		//Adjacency
		TopologicalAdjacency* getTopologicalMeshAdjacency() const
		{
			return m_TopologicalAdjacency;
		}

		std::unordered_map<std::string, Adjacency*>& get_OtherAdjacency() {return m_OtherAdjacency; }


		virtual void Distort(double alpha) {};

		////Updaters
		void CreateFacesFromCells();

		///Functions to add elements or group to the mesh
		//Add Element
		Point* addPoint(ELEMENTS::TYPE elementType, int elementIndex, std::string groupLabel, double x, double y, double z);
		Point* addPoint(std::string groupLabel, Point* point);
		Line* addLine(ELEMENTS::TYPE elementType, int elementIndex, std::string groupLabel, const std::vector<Point*>& vertexList);
		Polygon* addPolygon(ELEMENTS::TYPE elementType, int elementIndex, std::string groupLabel, const std::vector<Point*>& vertexList);
		Polyhedron* addPolyhedron(ELEMENTS::TYPE elementType, int elementIndex, std::string groupLabel, const std::vector<Point*>& vertexList);

		///Partitioning 
		// This is a graph-based partitioning followed by the add of ghost elements according to ghostBaseElement parameter.
		void PerformPolyhedronPartitioning(ELEMENTS::FAMILY edgeElement, ELEMENTS::FAMILY ghostBaseElement);
		std::vector<int> METISPartitioning(Adjacency* adjacency, int npartition);
		std::vector<int> TRIVIALPartitioning();

		//Adjacency
		void CreateLineGroupWithAdjacency(std::string Label, Adjacency* adjacency);

	

	protected:

		//Element Collections - First owned then ghosts
		PointCollection m_PointCollection;
		LineCollection m_LineCollection;
		PolygonCollection m_PolygonCollection;
		PolyhedronCollection m_PolyhedronCollection;

		//Property
		Property<PolyhedronCollection,double>* m_PolyhedronProperty_double;
		Property<PolyhedronCollection, int>* m_PolyhedronProperty_int;

		//Adjacency
		TopologicalAdjacency* m_TopologicalAdjacency;
		std::unordered_map<std::string, Adjacency*> m_OtherAdjacency;

	};


}
