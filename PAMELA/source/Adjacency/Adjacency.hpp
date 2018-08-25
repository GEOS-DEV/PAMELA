#pragma once
#include "Adjacency/CSRMatrix.hpp"
#include "Mesh/Mesh.hpp"

namespace PAMELA
{

	class Adjacency
	{

		friend class Mesh;
		friend class TopologicalAdjacency;

	public:

		Adjacency(ELEMENTS::FAMILY sourceFamily, ELEMENTS::FAMILY targetFamily, ELEMENTS::FAMILY baseFamily, ParallelEnsembleBase* source, ParallelEnsembleBase* target, ParallelEnsembleBase* base) :
			m_sourceElementCollection(source), m_targetElementCollection(target), m_baseElementCollection(base),
			m_sourceFamily(sourceFamily), m_targetFamily(targetFamily), m_baseFamily(baseFamily),
			m_adjacencySparseMatrix(new CSRMatrix(static_cast<int>(source->size_all()),
				static_cast<int>(target->size_all())))
		{
		}
		
		~Adjacency();

		CSRMatrix* get_adjacencySparseMatrix() { return m_adjacencySparseMatrix; }
		
		ParallelEnsembleBase* get_sourceElementCollection() const { return m_sourceElementCollection; }
		ParallelEnsembleBase* get_targetElementCollection() const { return m_targetElementCollection; }
		ParallelEnsembleBase* get_baseElementCollection() const { return m_baseElementCollection; }

		ELEMENTS::FAMILY get_sourceFamily() const { return m_sourceFamily; }
		ELEMENTS::FAMILY get_targetFamily() const { return m_targetFamily; }
		ELEMENTS::FAMILY get_baseFamily() const { return m_baseFamily; }

		std::pair<std::vector<int>, std::vector<int>> get_SingleElementAdjacency(int i) const;

	private:



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


}










