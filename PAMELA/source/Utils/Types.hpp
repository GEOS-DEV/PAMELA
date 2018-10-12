#pragma once



// Std library includes
#include <cstddef>


namespace PAMELA
{

	namespace Types
	{

		// general types
		using int_t = int;
		using uint_t = unsigned int;
		using size_type = std::size_t;

		// for unpartitioned entities, like groups
		using idx_t = unsigned int;

		// for partitioned entities, like cells
		using local_idx_t = unsigned int;
		using global_idx_t = unsigned long long;

		// for partitioning
		using proc_id_t = unsigned int;
		using weight_t = double;

	}
}
