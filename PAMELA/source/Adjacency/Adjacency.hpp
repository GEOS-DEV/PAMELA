#pragma once
#include "Adjacency/CSRMatrix.hpp"
#include "Mesh/Mesh.hpp"

namespace PAMELA
{

	struct TripletHash
	{
		std::size_t operator()(std::tuple<ELEMENTS::FAMILY, ELEMENTS::FAMILY, ELEMENTS::FAMILY> triplet) const
		{
			return static_cast<int>(static_cast<int>(std::get<0>(triplet)) + static_cast<int>(std::get<1>(triplet)) * 10 + static_cast<int>(std::get<2>(triplet)) * 100);
		}
	};


	class Adjacency
	{

		friend class MeshAdjacency;
		friend class Mesh;

	public:
		~Adjacency();

		CSRMatrix* get_adjacencySparseMatrix() { return m_adjacencySparseMatrix; }
		
		ParallelEnsembleBase* get_sourceElementCollection() const { return m_sourceElementCollection; }
		ParallelEnsembleBase* get_targetElementCollection() const { return m_targetElementCollection; }
		ParallelEnsembleBase* get_baseElementCollection() const { return m_baseElementCollection; }

		ELEMENTS::FAMILY get_sourceFamily() const { return m_sourceFamily; };
		ELEMENTS::FAMILY get_targetFamily() const { return m_targetFamily; };
		ELEMENTS::FAMILY get_baseFamily() const { return m_baseFamily; };

		std::pair<std::vector<int>, std::vector<int>> get_SingleElementAdjacency(int i) const;

	private:

		Adjacency(ELEMENTS::FAMILY sourceFamily, ELEMENTS::FAMILY targetFamily, ELEMENTS::FAMILY baseFamily,ParallelEnsembleBase* source, ParallelEnsembleBase* target, ParallelEnsembleBase* base) :
			m_sourceElementCollection(source), m_targetElementCollection(target), m_baseElementCollection(base),
			m_sourceFamily(sourceFamily), m_targetFamily(targetFamily), m_baseFamily(baseFamily),
			m_adjacencySparseMatrix(new CSRMatrix(static_cast<int>(source->size_all()),
			                                      static_cast<int>(target->size_all())))
		{
		}

		//Components
		ParallelEnsembleBase* m_sourceElementCollection;
		ParallelEnsembleBase* m_targetElementCollection;
		ParallelEnsembleBase* m_baseElementCollection;

		ELEMENTS::FAMILY m_sourceFamily;
		ELEMENTS::FAMILY m_targetFamily;
		ELEMENTS::FAMILY m_baseFamily;

		//data
		CSRMatrix* m_adjacencySparseMatrix;
	};


	class MeshAdjacency
	{

		friend class Mesh;

	public:

		typedef std::tuple<ELEMENTS::FAMILY, ELEMENTS::FAMILY, ELEMENTS::FAMILY> familyTriplet;

		MeshAdjacency(Mesh* mesh) :m_mesh(mesh) {}
		~MeshAdjacency();

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










