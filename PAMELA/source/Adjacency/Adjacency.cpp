#include "Adjacency/Adjacency.hpp"
#include "Utils/Logger.hpp"
#include "Elements/Polyhedron.hpp"

namespace PAMELA
{
	Adjacency::~Adjacency()
	{
		delete m_adjacencySparseMatrix;
	}

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

	MeshAdjacency::~MeshAdjacency()
	{
		for (auto it= adjacencyMap.begin();it!= adjacencyMap.end();++it)
		{
			delete it->second;
		}
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

	void MeshAdjacency::ClearAfterPartitioning(std::set<int> PolyhedronOwned, std::set<int> PolyhedronGhost, std::set<int> PolygonOwned, std::set<int> PolygonGhost)
	{

		//Update Polyhedron to face adjacency
		auto adjacency = get_Adjacency(ELEMENTS::FAMILY::POLYHEDRON, ELEMENTS::FAMILY::POLYGON, ELEMENTS::FAMILY::POLYHEDRON);
		auto polyhedra = static_cast<PolyhedronCollection*>(adjacency->get_sourceElementCollection());
		auto polygons = static_cast<PolygonCollection*>(adjacency->get_targetElementCollection());

		auto csr_matrix = adjacency->get_adjacencySparseMatrix();
		auto dimRow = csr_matrix->dimRow;
		auto nnz = csr_matrix->nnz;
		auto& columIndex = csr_matrix->columnIndex;
		auto& rowPtr = csr_matrix->rowPtr;
		auto& val = csr_matrix->values;
		auto dimColumn = csr_matrix->dimColumn;
		
		CSRMatrix* new_csr_matrix = new CSRMatrix;
		auto& new_dimRow_ghost =  new_csr_matrix->dimRow_ghost = static_cast<int>(polyhedra->size_ghost());
		auto& new_dimRow_owned = new_csr_matrix->dimRow_owned = static_cast<int>(polyhedra->size_owned());
		auto& new_dimRow = new_csr_matrix->dimRow = static_cast<int>(polyhedra->size_all());
		auto& new_dimColumn_ghost = new_csr_matrix->dimColumn_ghost = static_cast<int>(polygons->size_ghost());
		auto& new_dimColumn_owned = new_csr_matrix->dimColumn_owned = static_cast<int>(polygons->size_owned());
		auto& new_dimColumn = new_csr_matrix->dimColumn = static_cast<int>(polygons->size_all());
		auto& new_nnz = new_csr_matrix->nnz = 0;
		auto& new_columIndex = new_csr_matrix->columnIndex;
		auto& new_rowPtr = new_csr_matrix->rowPtr;
		auto& new_val = new_csr_matrix->values;

		for (auto it= polyhedra->begin();it!= polyhedra->end();++it)
		{
			auto polyhedron_global_index = (*it)->get_globalIndex();
			auto polyhedron_local_index = (*it)->get_localIndex();
			auto i0 = rowPtr[polyhedron_global_index];
			auto i1 = rowPtr[polyhedron_global_index + 1];
			std::vector<int> sub_columnIndex(columIndex.begin() + i0, columIndex.begin() + i1);
			std::vector<int> sub_val(val.begin() + i0, val.begin() + i1);
			std::vector<int> temp;
			for (size_t i = 0; i != sub_columnIndex.size(); ++i)
			{
				auto polygon_global_index = sub_columnIndex[i];
				
				if ((PolygonOwned.count(polygon_global_index) == 1) || (PolygonGhost.count(polygon_global_index) == 1))	//face is in the partition
				{
					auto polygon_local_index = polygons->get_GlobalToLocalIndex().at(polygon_global_index);
					temp.push_back(polygon_local_index);
					new_val.push_back(polyhedron_local_index);
					++new_nnz;
				}
			}
			std::sort(temp.begin(),temp.end());
			new_columIndex.insert(new_columIndex.end(), temp.begin(),temp.end());
			new_rowPtr.push_back(new_nnz);
		}

		Adjacency* new_adjacency = new Adjacency(ELEMENTS::FAMILY::POLYHEDRON, ELEMENTS::FAMILY::POLYGON, ELEMENTS::FAMILY::POLYHEDRON, polyhedra, polyhedra, polygons);
		new_adjacency->m_adjacencySparseMatrix = new_csr_matrix;
		adjacencyMap.clear();
		adjacencyMap[std::make_tuple(ELEMENTS::FAMILY::POLYHEDRON, ELEMENTS::FAMILY::POLYGON, ELEMENTS::FAMILY::POLYHEDRON)] = new_adjacency;

	}

	// Polyhedra to Points adjacency 
	Adjacency* MeshAdjacency::get_Adjacency(PolyhedronCollection* source, PointCollection* target, PolyhedronCollection* base)
	{

		Adjacency* adj = new Adjacency(ELEMENTS::FAMILY::POLYHEDRON, ELEMENTS::FAMILY::POINT, ELEMENTS::FAMILY::POLYHEDRON, source, target, base);
		auto collectionSize = source->size_all();
		int nbVertex = 0;
		int vertexIndex = 0;
		int PolyhedronIndex = 0;
		int nval = 0;
		int nrow = 0;

		adj->m_adjacencySparseMatrix->rowPtr.push_back(0);
		for (size_t i = 0; i != collectionSize; i++)
		{
			Polyhedron* polyhedron = source->operator[](static_cast<int>(i));
			PolyhedronIndex = polyhedron->get_localIndex();
			const std::vector<Point*>& vertexList = polyhedron->get_vertexList();
			nbVertex = static_cast<int>(vertexList.size());
			for (auto j = 0; j < nbVertex; j++)
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
		if (adjacencyMap.find(tri) != adjacencyMap.end())
		{
			return adjacencyMap.at(tri);
		}
		return nullptr;
	}


	Adjacency* MeshAdjacency::transposed(Adjacency* input)
	{
		Adjacency* adj = new Adjacency(input->get_targetFamily(), input->get_sourceFamily(), input->get_baseFamily(),input->m_targetElementCollection, input->m_sourceElementCollection, input->m_baseElementCollection);
		adj->m_adjacencySparseMatrix = CSRMatrix::transpose(input->m_adjacencySparseMatrix);
		return  adj;
	}


	Adjacency* MeshAdjacency::multiply(Adjacency* input_lhs, Adjacency* input_rhs)
	{
		Adjacency* adj = new Adjacency(input_rhs->get_targetFamily(), input_lhs->get_sourceFamily(), input_lhs->get_targetFamily(),input_rhs->m_targetElementCollection, input_lhs->m_sourceElementCollection, input_lhs->m_targetElementCollection);
		adj->m_adjacencySparseMatrix = CSRMatrix::product(input_lhs->m_adjacencySparseMatrix, input_rhs->m_adjacencySparseMatrix);
		return adj;
	}

}