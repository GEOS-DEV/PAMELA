#include "Utils/VectorUtils.hpp"
#include <algorithm>


int vectorUtils::MostOccuringValue(std::vector<int> vec)
{
	
	std::sort(std::begin(vec), std::end(vec));
	int curr_freq = 0;
	int max_freq = 0;
	int most_frequent_value = vec.front();
	int last_seen_value = vec.front();

	for (int value : vec)
	{
		if (value == last_seen_value) ++curr_freq;
		else
		{
			if (curr_freq > max_freq)
			{
				max_freq = curr_freq;
				most_frequent_value = last_seen_value;
			}

			last_seen_value = value;
			curr_freq = 1;
		}
	}
	return most_frequent_value;
}

std::vector<int> vectorUtils::Vector2VectorMapping(const std::vector<int>& map_vec, const std::vector<int>& source_vec)
{

	int size = map_vec.size();
	std::vector<int> res(size);
	for (auto i=0;i<size;++i)
	{
		res[i] = source_vec[map_vec[i]];
	}

	return res;
}


