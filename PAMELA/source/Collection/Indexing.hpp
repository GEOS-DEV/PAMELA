#pragma once
#include <unordered_map>
#include <set>

struct IndexData
{
	IndexData() :Local(-1), Global(-1) {};
	IndexData(int id) :Local(id), Global(id) {};
	IndexData(int idl, int idg) :Local(idl), Global(idg) {};

	int Local;
	int Global;
};