#include "Adjacency/CSRMatrix.hpp"
#include "Utils/Logger.hpp"
#include "Utils/Assert.hpp"
#include <algorithm>

bool CSRMatrix::checkMatrix()
{

	if (!(dimRow > 0 && dimColumn > 0))
	{
		LOGWARNING("The CSR matrix has a zero dimension");
	}

	int row_start = 0;
	for (int row = 0; row < dimRow - 1; ++row)
	{
		if (rowPtr[row] > rowPtr[row + 1])
		{
			LOGERROR("The CSR matrix has row pointer decreasing");
			return false;
		}

		if (rowPtr[row] == rowPtr[row + 1])
		{
			LOGERROR("The CSR matrix has an empty row");
			return false;
		}

		int row_stop = rowPtr[row + 1];
		for (int nnz_index = row_start; nnz_index < row_stop - 1; ++nnz_index)
		{
			if (columnIndex[nnz_index] >= columnIndex[nnz_index + 1])
			{
				LOGERROR("The CSR matrix has a column index vector not sorted");
				return false;
			}
		}
		row_start = row_stop;
	}

	return true;
}

void CSRMatrix::sortRowIndexAndMoveValues()
{

	for (auto i = 0; i <dimRow; ++i)
	{
		sortRowIndexAndMoveValues(i);
	}

}

void CSRMatrix::sortRowIndexAndMoveValues(int i)
{

	int i0 = rowPtr[i];
	int i1 = rowPtr[i+1];
	std::vector<int> column(columnIndex.begin()+i0, columnIndex.begin() + i1);
	std::vector<int> val(values.begin() + i0, values.begin() + i1);

	//Zip values
	std::vector<std::pair<int, int>> zipped;
	for (size_t j = 0; j<column.size(); ++j)
	{
		zipped.push_back(std::make_pair(column[j], val[j]));
	}

	//Sort
	std::sort(std::begin(zipped), std::end(zipped),
		[&](const auto& a, const auto& b)
	{
		return a.first < b.first;
	});

	for (size_t j = 0; j<column.size(); j++)
	{
		column[j] = zipped[j].first;
		val[j] = zipped[j].second;
	}

	//Copy back
	for (size_t j = 0; j < column.size(); j++)
	{
		columnIndex[j + i0] = column[j];
		values[j + i0] = val[j];
	}

}

void CSRMatrix::shrink()
{
	int size = rowPtr[dimRow];
	values.resize(size);
	values.shrink_to_fit();
	columnIndex.resize(size);
	columnIndex.shrink_to_fit();
}


CSRMatrix* CSRMatrix::product( CSRMatrix* matrix_lhs,  CSRMatrix* matrix_rhs)
{
	ASSERT(matrix_lhs->checkMatrix(), "Problem with CSR matrix data structure");
	ASSERT(matrix_rhs->checkMatrix(), "Problem with CSR matrix data structure");
	ASSERT(matrix_lhs->dimColumn == matrix_rhs->dimRow, "Matrix dimensions are not compatible for product operation");

	//Dimensions
	int nnz_lhs = matrix_lhs->nnz;
	int Nc_lhs = matrix_lhs->dimColumn;
	int Nr_lhs = matrix_lhs->dimRow;
	int nnz_rhs = matrix_rhs->nnz;
	int Nc_rhs = matrix_rhs->dimColumn;
	int Nr_rhs = matrix_rhs->dimRow;

	//Dynamic allocation of trans_mat
	int nnz_guess = (nnz_lhs + nnz_rhs) * 10;
	CSRMatrix* mult_mat = new CSRMatrix(Nr_lhs, Nr_lhs, nnz_guess);

	int Mnnz = mult_mat->nnz;
	int MNc = mult_mat->dimColumn;
	int MNr = mult_mat->dimRow;

	//Work data
	auto& rowPtr_lhs = matrix_lhs->rowPtr;
	auto& columnIndex_lhs = matrix_lhs->columnIndex;
	auto& values_lhs = matrix_lhs->values;
	auto& rowPtr_rhs = matrix_rhs->rowPtr;
	auto& columnIndex_rhs = matrix_rhs->columnIndex;
	auto& values_rhs = matrix_rhs->values;
	auto& MrowPtr = mult_mat->rowPtr;
	auto& McolumnIndex = mult_mat->columnIndex;
	auto& Mvalues = mult_mat->values;

	std::vector<int> iw(Mnnz, -1);
	int len = -1;

	MrowPtr[1] = 0;
	for (int ii = 0; ii < Nr_lhs; ++ii)
	{
		for (int ka = rowPtr_lhs[ii]; ka < rowPtr_lhs[ii + 1]; ++ka)
		{
			int scal = values_lhs[ka];
			int jj = columnIndex_lhs[ka];
			for (int kb = rowPtr_rhs[jj]; kb < rowPtr_rhs[jj + 1]; ++kb)
			{
				int jcol = columnIndex_rhs[kb];
				int jpos = iw[jcol];
				if (jpos == -1)
				{
					len++;
					McolumnIndex[len] = jcol;
					iw[jcol] = len;
					int temp;
					if (ii == jcol)
					{
						temp = -1;
					}
					else
					{
						//temp = scal*M->get_m_value(kb);
						temp = jj;
					}
					Mvalues[len] = temp;
				}
				else
				{
					int temp;
					if (ii == jcol)
					{
						temp = -1;
					}
					else
					{
						//temp = Mout->get_m_value(jpos) + scal*M->get_m_value(kb);
						temp = jj;
					}
					Mvalues[jpos] = temp;
				}
			}
		}

		for (int k = MrowPtr[ii]; k < len + 1; ++k)
		{
			iw[McolumnIndex[k]] = -1;
		}
		MrowPtr[ii + 1] = len + 1;
		//Mout->sort_row(ii);
	}

	MrowPtr[Nr_lhs] = len+1;

	//
	mult_mat->nnz = len;
	mult_mat->shrink();
	mult_mat->sortRowIndexAndMoveValues();
	ASSERT(mult_mat->checkMatrix(), "Something wrong with the resulting transposed matrix");

	return mult_mat;

}

CSRMatrix* CSRMatrix::transpose(CSRMatrix* matrix)
{
	ASSERT(matrix->checkMatrix(), "Problem with CSR matrix data structure");

	//Dimensions
	int nnz = matrix->nnz;
	int Nc = matrix->dimColumn;
	int Nr = matrix->dimRow;

	//Dynamic allocation of trans_mat
	CSRMatrix* trans_mat = new CSRMatrix(Nc, Nr, nnz);

	//Work data
	auto& rowPtr = matrix->rowPtr;
	auto& columnIndex = matrix->columnIndex;
	auto& values = matrix->values;
	auto& TrowPtr = trans_mat->rowPtr;
	auto& TcolumnIndex = trans_mat->columnIndex;
	auto& Tvalues = trans_mat->values;


	//
	// Stage 1: Compute pattern for B
	//
	int row_start = 0;
	for (int row = 0; row < Nr; ++row)
	{
		int row_stop = rowPtr[row + 1];

		for (int nnz_index = row_start; nnz_index < row_stop; ++nnz_index)
		{
			int ival = TrowPtr[columnIndex[nnz_index]];
			TrowPtr[columnIndex[nnz_index]] = ival + 1;
		}
		row_start = row_stop;
	}
	TrowPtr[Nc] = 1;

	// Bring row-start array in place using exclusive-scan:
	int offset = 0;

	for (int row = 0; row < Nc; ++row)
	{
		int tmp = TrowPtr[row];
		TrowPtr[row] = offset;
		offset += tmp;
	}
	TrowPtr[Nc] = offset;

	//
	// Stage 2: Fill with data
	//
	std::vector<int> B_offsets = TrowPtr; // index of first unwritten element per row

	row_start = 0;
	for (int row = 0; row < Nr; ++row)
	{
		int row_stop = rowPtr[row + 1];

		for (int nnz_index = row_start; nnz_index < row_stop; ++nnz_index)
		{
			int col_in_A = columnIndex[nnz_index];
			int B_nnz_index = B_offsets[col_in_A];
			TcolumnIndex[B_nnz_index] = row;
			Tvalues[B_nnz_index] = values[nnz_index];
			B_offsets[col_in_A] += 1;
		}

		row_start = row_stop;
	}

	ASSERT(trans_mat->checkMatrix(), "Something wrong with the resulting transposed matrix");

	return trans_mat;

}