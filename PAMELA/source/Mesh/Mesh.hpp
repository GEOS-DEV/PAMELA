#pragma once
#include <vector>
#include "Elements/Point.hpp"
#include "Elements/Line.hpp"
#include "Elements/Polygon.hpp"
#include "Elements/Polyhedron.hpp"
#include "Collection/Collection.hpp"
#include "Property/Property.hpp"
#include "Adjacency/AdjacencySet.hpp"

namespace PAMELA
{

    class Adjacency;

    /*!
     * @brief Class describing the Mesh data structure
     */
    class Mesh
    {
        public:
            virtual ~Mesh();
            Mesh();

            /*!
             * @brief Gets the Point Collection
             * @details The point collection contains all the points within
             * this mesh
             */
            const PointCollection & get_PointCollection() { return m_PointCollection; }

            /*!
             * @brief Gets the Line Collection
             * @details The line collection contains all the lines within
             * this mesh
             */
            const LineCollection & get_LineCollection() { return m_LineCollection; }

            /*!
             * @brief Gets the Polygon Collection
             * @details The polygon collection contains all the polygons
             * (i.e. quadrangles, triangles...) within this mesh
             */
            const PolygonCollection & get_PolygonCollection() { return m_PolygonCollection; }

            /*!
             * @brief Gets the Polyhedron Collection
             * @details The polyhedron collection contains all the polyhedrons
             * (i.e. tetrahedron, hexahedron, wedges and pyramids...) within this mesh
             */
            const PolyhedronCollection &  get_PolyhedronCollection() { return m_PolyhedronCollection; }

            LineCollection*  get_ImplicitLineCollection() { return &m_ImplicitLineCollection; }

            Property<PolyhedronCollection, double>* get_PolyhedronProperty_double() const { return m_PolyhedronProperty_double; }
            Property<PolyhedronCollection, int>* get_PolyhedronProperty_int() const { return m_PolyhedronProperty_int; }

            //Adjacency
            AdjacencySet* getAdjacencySet() const
            {
                return m_AdjacencySet;
            }

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

            //Add Implicit Elements
            void AddImplicitLine(ELEMENTS::TYPE elementType, std::string groupLabel, std::vector<Point*>& pointList);



            ///Partitioning 
            // This is a graph-based partitioning followed by the add of ghost elements according to ghostBaseElement parameter.
            void PerformPolyhedronPartitioning(ELEMENTS::FAMILY edgeElement, ELEMENTS::FAMILY ghostBaseElement);
            std::vector<int> METISPartitioning(Adjacency* adjacency, int npartition);
            std::vector<int> TRIVIALPartitioning();

            //Adjacency
            void CreateLineGroupWithAdjacency(std::string Label, Adjacency* adjacency);



        protected:

            //Explicit Element Collections - First owned then ghosts
            PointCollection m_PointCollection;
            LineCollection m_LineCollection;
            PolygonCollection m_PolygonCollection;
            PolyhedronCollection m_PolyhedronCollection;

            //Implicit Element Collections
            PointCollection m_ImplicitPointCollection;
            LineCollection m_ImplicitLineCollection;

            //Property
            Property<PolyhedronCollection,double>* m_PolyhedronProperty_double;
            Property<PolyhedronCollection, int>* m_PolyhedronProperty_int;

            //Adjacency
            AdjacencySet* m_AdjacencySet;

    };


}
