#pragma once

#include <cstddef>
#include <tuple>
#include "Elements/Element.hpp"
#include "Collection/Collection.hpp"


namespace PAMELA
{

	class Adjacency;
        class Mesh;

	struct TripletHash
	{
		std::size_t operator()(std::tuple<ELEMENTS::FAMILY, ELEMENTS::FAMILY, ELEMENTS::FAMILY> triplet) const
		{
			return static_cast<int>(static_cast<int>(std::get<0>(triplet)) + static_cast<int>(std::get<1>(triplet)) * 10 + static_cast<int>(std::get<2>(triplet)) * 100);
		}
	};

		typedef std::tuple<ELEMENTS::FAMILY, ELEMENTS::FAMILY, ELEMENTS::FAMILY> familyTriplet;

	class AdjacencySet
	{
		friend class Mesh;


	public:


		AdjacencySet(Mesh * mesh) : m_mesh(mesh) {}
		~AdjacencySet()=default;

		//General getter
		Adjacency* get_TopologicalAdjacency(ELEMENTS::FAMILY source, ELEMENTS::FAMILY target, ELEMENTS::FAMILY base);
		void ClearAfterPartitioning(std::set<int>& PolyhedronOwned, std::set<int>& PolyheronGhost, std::set<int>& PolygonOwned, std::set<int>& PolygonGhost);
		Adjacency ClearAfterPartitioning_Topological(Adjacency* adj, std::set<int>& PolygonOwned,
		                                        std::set<int>& PolygonGhost);
		Adjacency ClearAfterPartitioning_NonTopological(Adjacency* adjacency, std::set<int>& Polyhedron_owned,
		                                                 std::set<int>& Polyhedron_ghost);

		void Add_NonTopologicalAdjacency(std::string label, Adjacency* adj) { NonTopologicalAdjacencyMap[label] = adj; }
		void Add_NonTopologicalAdjacencySum(std::string label, std::vector<Adjacency*> sumAdj);

		Adjacency* get_NonTopologicalAdjacency(std::string label) { return NonTopologicalAdjacencyMap.at(label); }

	private:

		Mesh* m_mesh;

		//Test
		Adjacency* adjacencyExist(ELEMENTS::FAMILY source, ELEMENTS::FAMILY target, ELEMENTS::FAMILY base);

		//Adjacency storage
		std::unordered_map<std::string, Adjacency> NonTopologicalAdjacencyMap;
		std::unordered_map<familyTriplet, Adjacency, TripletHash> TopologicalAdjacencyMap;

		//Adjacency internal getter or builder
		Adjacency get_TopologicalAdjacency(const PolyhedronCollection& source, const PointCollection& target, const PolyhedronCollection& base);
		Adjacency get_TopologicalAdjacency(const PointCollection& source, const PolyhedronCollection& target, const PolyhedronCollection& base);
		Adjacency get_TopologicalAdjacency(const PolyhedronCollection& source, const PolygonCollection& target, const PolyhedronCollection& base);
		Adjacency get_TopologicalAdjacency(const PolygonCollection& source, const PolyhedronCollection& target, const PolyhedronCollection& base);
		Adjacency get_TopologicalAdjacency(const PolyhedronCollection& source, const PolyhedronCollection& target, const PolygonCollection& base);


	};



}










