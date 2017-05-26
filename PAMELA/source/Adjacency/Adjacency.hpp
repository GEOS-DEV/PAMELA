#pragma once
#include "Adjacency/CSRMatrix.hpp"
#include "Mesh/Mesh.hpp"



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

	CSRMatrix* get_adjacencySparseMatrix() {return m_adjacencySparseMatrix; }
	EnsembleBase* get_sourceElementCollection() const {return m_sourceElementCollection;}
	EnsembleBase* get_targetElementCollection() const {return m_targetElementCollection;}
	EnsembleBase* get_baseElementCollection() const {return m_baseElementCollection;}
	std::pair<std::vector<int>, std::vector<int>> get_SingleElementAdjacency(int i) const;

private:

	Adjacency(EnsembleBase* source, EnsembleBase* target, EnsembleBase* base) :
		m_sourceElementCollection(source), m_targetElementCollection(target), m_baseElementCollection(base), m_adjacencySparseMatrix(new CSRMatrix(source->size_all(), target->size_all())) {};

	//Components
	EnsembleBase* m_sourceElementCollection;
	EnsembleBase* m_targetElementCollection;
	EnsembleBase* m_baseElementCollection;

	//data
	CSRMatrix* m_adjacencySparseMatrix;
};


class MeshAdjacency
{

	friend class Mesh;

public:

	typedef std::tuple<ELEMENTS::FAMILY, ELEMENTS::FAMILY, ELEMENTS::FAMILY> familyTriplet;
	MeshAdjacency(Mesh* mesh) :m_mesh(mesh) {}
	
	//General getter
	Adjacency* get_Adjacency(ELEMENTS::FAMILY source, ELEMENTS::FAMILY target, ELEMENTS::FAMILY base);

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














