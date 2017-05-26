#pragma once
#include <vector>

namespace PAMELA
{

	struct CSRMatrix
	{

		CSRMatrix(int dim_row, int dim_col, int nnz) :nnz(nnz), dimRow(dim_row), dimColumn(dim_col) { values.resize(nnz); rowPtr.resize(dim_row + 1); columnIndex.resize(nnz); };
		CSRMatrix(int dim_row, int dim_col) :CSRMatrix(dim_row, dim_col, 0) {};
		CSRMatrix() :CSRMatrix(0, 0, 0) {};

		static CSRMatrix* transpose(CSRMatrix* matrix);
		static CSRMatrix* product(CSRMatrix* matrix_lhs, CSRMatrix* matrix_rhs);
		bool checkMatrix();

		void sortRowIndexAndMoveValues();
		void sortRowIndexAndMoveValues(int i);
		void shrink();

		int nnz;
		int dimRow, dimColumn;
		std::vector<int> rowPtr, columnIndex;
		std::vector<int> values;

	};

}
