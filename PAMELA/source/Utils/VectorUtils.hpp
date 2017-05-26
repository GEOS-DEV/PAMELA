#pragma once
#include <vector>


namespace PAMELA
{

	namespace vectorUtils
	{


		int MostOccuringValue(std::vector<int> vec);

		std::vector<int> Vector2VectorMapping(const std::vector<int>& map_vec, const std::vector<int>& source_vec);

	}
}
