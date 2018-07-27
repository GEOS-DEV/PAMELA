#include "Adjacency/Adjacency.hpp"
#include "Utils/Logger.hpp"
#include "Elements/Polyhedron.hpp"

namespace PAMELA
{

	std::pair<std::vector<int>, std::vector<int>> Adjacency::get_SingleElementAdjacency(int i) const
	{
		auto firstColumnIndex = m_adjacencySparseMatrix->columnIndex.begin() + m_adjacencySparseMatrix->rowPtr[i];
		auto lastColumnIndex = m_adjacencySparseMatrix->columnIndex.begin() + m_adjacencySparseMatrix->rowPtr[i + 1];
		auto firstValues = m_adjacencySparseMatrix->values.begin() + m_adjacencySparseMatrix->rowPtr[i];
		auto lastValues = m_adjacencySparseMatrix->values.begin() + m_adjacencySparseMatrix->rowPtr[i + 1];
		std::vector<int> columnIndexVec(firstColumnIndex, lastColumnIndex);
		std::vector<int> valuesVec(firstValues, lastValues);
		return std::make_pair(columnIndexVec, valuesVec);
	}

	Adjacency* MeshAdjacency::get_Adjacency(ELEMENTS::FAMILY source, ELEMENTS::FAMILY target, ELEMENTS::FAMILY base)
	{
		if (adjacencyExist(source, target, base) != nullptr)
		{
			return adjacencyExist(source, target, base);
		}

		// SOURCE = POLYHEDRON ; TARGET = POINT ; BASE = POLYHEDRON
		if ((source == ELEMENTS::FAMILY::POLYHEDRON) && (target == ELEMENTS::FAMILY::POINT) && (base == ELEMENTS::FAMILY::POLYHEDRON))
		{
			return get_Adjacency(m_mesh->get_PolyhedronCollection(), m_mesh->get_PointCollection(), m_mesh->get_PolyhedronCollection());
		}

		// SOURCE = POINT ; TARGET = POLYHEDRON ; BASE = POLYHEDRON
		if ((source == ELEMENTS::FAMILY::POINT) && (target == ELEMENTS::FAMILY::POLYHEDRON) && (base == ELEMENTS::FAMILY::POLYHEDRON))
		{
			return get_Adjacency(m_mesh->get_PointCollection(), m_mesh->get_PolyhedronCollection(), m_mesh->get_PolyhedronCollection());
		}


		// SOURCE = POLYHEDRON ; TARGET = POLYGON ; BASE = POLYHEDRON
		if ((source == ELEMENTS::FAMILY::POLYHEDRON) && (target == ELEMENTS::FAMILY::POLYGON) && (base == ELEMENTS::FAMILY::POLYHEDRON))
		{
			return get_Adjacency(m_mesh->get_PolyhedronCollection(), m_mesh->get_PolygonCollection(), m_mesh->get_PolyhedronCollection());
		}

		// SOURCE = POLYGON ; TARGET = POLYHEDRON ; BASE = POLYHEDRON
		if ((source == ELEMENTS::FAMILY::POLYGON) && (target == ELEMENTS::FAMILY::POLYHEDRON) && (base == ELEMENTS::FAMILY::POLYHEDRON))
		{
			return get_Adjacency(m_mesh->get_PolygonCollection(), m_mesh->get_PolyhedronCollection(), m_mesh->get_PolyhedronCollection());
		}

		// SOURCE = POLYHEDRON ; TARGET = POLYHEDRON ; BASE = POLYGON
		if ((source == ELEMENTS::FAMILY::POLYHEDRON) && (target == ELEMENTS::FAMILY::POLYHEDRON) && (base == ELEMENTS::FAMILY::POLYGON))
		{
			return get_Adjacency(m_mesh->get_PolyhedronCollection(), m_mesh->get_PolyhedronCollection(), m_mesh->get_PolygonCollection());
		}

		LOGERROR("Adjacency not implemented yet");
		return nullptr;


	}

	// Polyhedra to Points adjacency 
	Adjacency* MeshAdjacency::get_Adjacency(PolyhedronCollection* source, PointCollection* target, PolyhedronCollection* base)
	{

		Adjacency* adj = new Adjacency(source, target, base);
		int collectionSize = source->size_all();
		int nbVertex = 0;
		int vertexIndex = 0;
		int PolyhedronIndex = 0;
		int nval = 0;
		int nrow = 0;

		adj->m_adjacencySparseMatrix->rowPtr.push_back(0);
		for (int i = 0; i < collectionSize; i++)
		{
			Polyhedron* polyhedron = source->operator[](i);
			PolyhedronIndex = polyhedron->get_localIndex();
			const std::vector<Point*>& vertexList = polyhedron->get_vertexList();
			nbVertex = static_cast<int>(vertexList.size());
			for (int j = 0; j < nbVertex; j++)
			{
				vertexIndex = vertexList[j]->get_localIndex();
				adj->m_adjacencySparseMatrix->columnIndex.push_back(vertexIndex);
				adj->m_adjacencySparseMatrix->values.push_back(PolyhedronIndex);
				nval++;
			}
			nrow++;
			adj->m_adjacencySparseMatrix->rowPtr[nrow] = nval;
		}
		adj->m_adjacencySparseMatrix->nnz = nval;
		adj->m_adjacencySparseMatrix->sortRowIndexAndMoveValues();
		adj->m_adjacencySparseMatrix->checkMatrix();

		//Add to map
		adjacencyMap[std::make_tuple(source->get_family(), target->get_family(), base->get_family())] = adj;

		return adj;
	}

	// Points to Polyhedra adjacency 
	Adjacency* MeshAdjacency::get_Adjacency(PointCollection* source, PolyhedronCollection* target, PolyhedronCollection* base)
	{
		Adjacency* adj = transposed(get_Adjacency(ELEMENTS::FAMILY::POLYHEDRON, ELEMENTS::FAMILY::POINT, ELEMENTS::FAMILY::POLYHEDRON));
		return adj;
	}


	// Polyhedra to Face adjacency
	Adjacency* MeshAdjacency::get_Adjacency(PolyhedronCollection* source, PolygonCollection* target, PolyhedronCollection* base)
	{

		LOGERROR("This adjacency must be created while creating Polygons from Polyhedra");
		return nullptr;
	}

	Adjacency* MeshAdjacency::get_Adjacency(PolygonCollection* source, PolyhedronCollection* target, PolyhedronCollection* base)
	{

		Adjacency* adj = transposed(get_Adjacency(ELEMENTS::FAMILY::POLYHEDRON, ELEMENTS::FAMILY::POLYGON, ELEMENTS::FAMILY::POLYHEDRON));
		return adj;
	}



	Adjacency* MeshAdjacency::get_Adjacency(PolyhedronCollection* source, PolyhedronCollection* target, PolygonCollection* base)
	{
		Adjacency* adj1 = get_Adjacency(ELEMENTS::FAMILY::POLYHEDRON, ELEMENTS::FAMILY::POLYGON, ELEMENTS::FAMILY::POLYHEDRON);
		Adjacency* adj2 = get_Adjacency(ELEMENTS::FAMILY::POLYGON, ELEMENTS::FAMILY::POLYHEDRON, ELEMENTS::FAMILY::POLYHEDRON);
		Adjacency* adj = multiply(adj1, adj2);
		return adj;
	}


	Adjacency* MeshAdjacency::adjacencyExist(ELEMENTS::FAMILY source, ELEMENTS::FAMILY target, ELEMENTS::FAMILY base)
	{
		familyTriplet tri = std::make_tuple(source, target, base);
		return adjacencyMap[tri];
	}


	Adjacency* MeshAdjacency::transposed(Adjacency* input)
	{
		Adjacency* adj = new Adjacency(input->m_targetElementCollection, input->m_sourceElementCollection, input->m_baseElementCollection);
		adj->m_adjacencySparseMatrix = CSRMatrix::transpose(input->m_adjacencySparseMatrix);
		return  adj;
	}


	Adjacency* MeshAdjacency::multiply(Adjacency* input_lhs, Adjacency* input_rhs)
	{
		Adjacency* adj = new Adjacency(input_rhs->m_targetElementCollection, input_lhs->m_sourceElementCollection, input_lhs->m_targetElementCollection);
		adj->m_adjacencySparseMatrix = CSRMatrix::product(input_lhs->m_adjacencySparseMatrix, input_rhs->m_adjacencySparseMatrix);
		return adj;
	}

}