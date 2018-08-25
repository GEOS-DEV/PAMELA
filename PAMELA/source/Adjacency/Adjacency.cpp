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

	
}