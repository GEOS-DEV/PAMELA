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

	class TopologicalAdjacency
	{
		friend class Mesh;


	public:


		TopologicalAdjacency(Mesh * mesh) : m_mesh(mesh) {}
		~TopologicalAdjacency();

		//General getter
		Adjacency* get_Adjacency(ELEMENTS::FAMILY source, ELEMENTS::FAMILY target, ELEMENTS::FAMILY base);
		void ClearAfterPartitioning(std::set<int> PolyhedronOwned, std::set<int> PolyhedronGhost,
		                            std::set<int> PolygonOwned,
		                            std::set<int> PolygonGhost);


	private:

		Mesh* m_mesh;

		//Utils
		Adjacency* transposed(Adjacency* input);
		Adjacency* multiply(Adjacency* input_lhs, Adjacency* input_rhs);


		//Test
		Adjacency* adjacencyExist(ELEMENTS::FAMILY source, ELEMENTS::FAMILY target, ELEMENTS::FAMILY base);

		//Adjacency storage
		std::unordered_map<familyTriplet, Adjacency*, TripletHash> adjacencyMap;

		//Adjacency internal getter or builder
		Adjacency* get_Adjacency(PolyhedronCollection* source, PointCollection* target, PolyhedronCollection* base);
		Adjacency* get_Adjacency(PointCollection* source, PolyhedronCollection* target, PolyhedronCollection* base);
		Adjacency* get_Adjacency(PolyhedronCollection* source, PolygonCollection* target, PolyhedronCollection* base);
		Adjacency* get_Adjacency(PolygonCollection* source, PolyhedronCollection* target, PolyhedronCollection* base);
		Adjacency* get_Adjacency(PolyhedronCollection* source, PolyhedronCollection* target, PolygonCollection* base);


	};



}










