
#pragma once
// Library includes
#include <unordered_map>
#include "Utils/Assert.hpp"

#if defined( _WIN32)
#include <direct.h>
#else
#include <sys/types.h>
#include <sys/stat.h>
#endif

namespace PAMELA
{

	//////VARIABLES
	//Variable types
	enum class VARIABLE_TYPE { UNKNOWN = -1, SCALAR = 1, VECTOR = 3, TENSOR_SYMM = 6 };
	//Variable locations
	enum class VARIABLE_LOCATION { UNKNOWN = -1, PER_NODE = 1, PER_CELL = 2 };
	//Variable size
	const std::unordered_map<VARIABLE_TYPE, int> VariableTypeToSize =
	{
		{ VARIABLE_TYPE::SCALAR,1 },
		{ VARIABLE_TYPE::VECTOR,3 },
		{ VARIABLE_TYPE::TENSOR_SYMM,6 },
	};

	struct Variable
	{
		Variable(VARIABLE_TYPE dtype, std::string label, int size) : Label(label), dType(dtype)
		{
			offset = VariableTypeToSize.at(dtype);
			Data = std::vector<double>(size*offset);
		}

		std::string Label;
		int offset;
		VARIABLE_TYPE dType;

		void set_data(double cst)
		{
			std::fill(Data.begin(), Data.end(), cst);
		}

		void set_data(const std::vector<double>& vec)
		{
			ASSERT(vec.size() == Data.size(), "Mismatch sizes");
			Data = vec;
		}

		std::vector<double> get_data(int i)
		{
			if (offset == 1)
				return{ Data[i] };
			std::vector<double> vec(&Data[i*offset], &Data[(i + 1)*offset - 1]);
			return vec;
		}

	private:
		std::vector<double> Data;

	};


	struct VariableKey
	{
		std::string first;
		std::string second;

		VariableKey(std::string first, std::string second)
		{
			this->first = first;
			this->second = second;
		}

		bool operator==(const VariableKey &other) const
		{
			return (first == other.first
				&& second == other.second);
		}
	};

	struct VariableKeyHash
	{
		std::size_t operator()(const VariableKey& k) const
		{
			auto hfirst = std::hash<std::string>()(k.first);
			auto hsecond = std::hash<std::string>()(k.second);
			return hfirst ^ hsecond;
		}
	};

}
