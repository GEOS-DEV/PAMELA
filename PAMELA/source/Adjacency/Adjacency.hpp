#pragma once
#include "Adjacency/CSRMatrix.hpp"
#include "Mesh/Mesh.hpp"

namespace PAMELA
{

	class Adjacency
	{

		friend class Mesh;

	public:

                /*
		Adjacency(ELEMENTS::FAMILY sourceFamily, ELEMENTS::FAMILY targetFamily, ELEMENTS::FAMILY baseFamily, const ParallelEnsembleBase& source, const ParallelEnsembleBase& target, const ParallelEnsembleBase& base, const CSRMatrix& csr_mat) :
			m_sourceElementCollection(source), m_targetElementCollection(target), m_baseElementCollection(base),
			m_sourceFamily(sourceFamily), m_targetFamily(targetFamily), m_baseFamily(baseFamily)
		{
		}
                */

		Adjacency(ELEMENTS::FAMILY sourceFamily, ELEMENTS::FAMILY targetFamily, ELEMENTS::FAMILY baseFamily, const ParallelEnsembleBase& source, const ParallelEnsembleBase& target, const ParallelEnsembleBase& base) :
			m_sourceElementCollection(source), m_targetElementCollection(target), m_baseElementCollection(base),
			m_sourceFamily(sourceFamily), m_targetFamily(targetFamily), m_baseFamily(baseFamily),
                        m_adjacencySparseMatrix(static_cast<int>(source.size_all()), static_cast<int>(target.size_all()))
		{
		}

		~Adjacency();

		const CSRMatrix& get_adjacencySparseMatrix() const { return m_adjacencySparseMatrix; }
                void set_adjacencySparseMatrix(const CSRMatrix& adjacencySparseMatrix){m_adjacencySparseMatrix = adjacencySparseMatrix;}

		//Utils
		static Adjacency* transposed(Adjacency* input);
		static Adjacency* multiply(Adjacency* input_lhs, Adjacency* input_rhs);
		
		const ParallelEnsembleBase& get_sourceElementCollection() const { return m_sourceElementCollection; }
		const ParallelEnsembleBase& get_targetElementCollection() const { return m_targetElementCollection; }
		const ParallelEnsembleBase& get_baseElementCollection() const { return m_baseElementCollection; }

		ELEMENTS::FAMILY get_sourceFamily() const { return m_sourceFamily; }
		ELEMENTS::FAMILY get_targetFamily() const { return m_targetFamily; }
		ELEMENTS::FAMILY get_baseFamily() const { return m_baseFamily; }

		std::pair<std::vector<int>, std::vector<int>> get_SingleElementAdjacency(int i) const;

	private:



		//Components
		const ParallelEnsembleBase& m_sourceElementCollection;
		const ParallelEnsembleBase& m_targetElementCollection;
		const ParallelEnsembleBase& m_baseElementCollection;

		ELEMENTS::FAMILY m_sourceFamily;
		ELEMENTS::FAMILY m_targetFamily;
		ELEMENTS::FAMILY m_baseFamily;

		//data
		CSRMatrix m_adjacencySparseMatrix;
	};


}










