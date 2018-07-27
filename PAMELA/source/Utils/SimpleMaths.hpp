#pragma once
#include <vector>
#include <cmath>

namespace PAMELA
{

	struct Coordinates
	{
		Coordinates() :x(0), y(0), z(0) {}
		Coordinates(double x, double y, double z) :x(x), y(y), z(z) {}
		double x, y, z;
	};



	inline int CoinToss(int i0, int i1)
	{
		if (i0 < i1)
			return i0;
		return i1;
	};


	inline std::vector <double> cross_product(const std::vector <double>& vec1, const std::vector <double>& vec2)
	{
		std::vector <double> val(3);
		val[0] = vec1[1] * vec2[2] - vec1[2] * vec2[1];
		val[1] = vec1[2] * vec2[0] - vec1[0] * vec2[2];
		val[2] = vec1[0] * vec2[1] - vec1[1] * vec2[0];
		return val;
	}

	inline double norm(const std::vector <double>& vec)
	{
		return std::sqrt(vec[0] * vec[0] + vec[1] * vec[1] + vec[2] * vec[2]);
	}

}