#pragma once
#include <vector>

namespace PAMELA
{

	struct CSRMatrix
	{

		CSRMatrix(int dim_row, int dim_col, int nn_z) : nnz(nn_z), dimRow(dim_row), dimColumn(dim_col), dimRow_owned(dim_row),
		                                               dimColumn_owned(dim_col), dimRow_ghost(0), dimColumn_ghost(0)
		{
			values.resize(nnz);
			rowPtr.resize(dim_row + 1);
			columnIndex.resize(nnz);
		}
		CSRMatrix(int dim_row, int dim_col) :CSRMatrix(dim_row, dim_col, 0) {};
		CSRMatrix() :CSRMatrix(0, 0, 0) {};

		static CSRMatrix* transpose(CSRMatrix* matrix);
		static CSRMatrix* product(CSRMatrix* matrix_lhs, CSRMatrix* matrix_rhs);
		static CSRMatrix* sum(CSRMatrix* matrix_lhs, CSRMatrix* matrix_rhs);
		bool checkMatrix();

		void sortRowIndexAndMoveValues();
		void sortRowIndexAndMoveValues(int i);
		void shrink();
		void fillEmpty(int dim_row, int dim_col);


		int nnz;
		int dimRow, dimColumn;
		int dimRow_owned, dimColumn_owned;
		int dimRow_ghost, dimColumn_ghost;
		std::vector<int> rowPtr, columnIndex;
		std::vector<int> values;

	};

}
