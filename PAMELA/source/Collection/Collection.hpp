#pragma once
#include <vector>
#include <algorithm>
#include <map>
#include <numeric>
#include <set>
#include "Collection/Indexing.hpp"
#include "Elements/Point.hpp"
#include "Collection/Ensemble.hpp"

namespace PAMELA
{

	//Hash functions
	//--Element except Point
	template <class T>
	struct ElementHash
	{
		std::size_t operator()(const T& ele) const
		{
			auto vect_ver = ele->get_vertexList();
			int hc = 0;
			for (int i = 0; i < static_cast<int>(vect_ver.size()); i++)
			{
				hc += vect_ver[i]->get_localIndex();
			}

			return hc;
		}
	};

	//--Point
	template <>
	struct ElementHash<Point*>
	{
		std::size_t operator()(Point* vec) const
		{
			auto coord = vec->get_coordinates();
			size_t h1 = std::hash<double>()(coord.x);
			size_t h2 = std::hash<double>()(coord.y);
			size_t h3 = std::hash<double>()(coord.z);
			return (h1 ^ (h2 << 1)) ^ h3;
		}
	};


	//Equal function
	//--Element except Point
	template <class T>
	struct ElementEqual
	{
		bool operator()(const T& lhs, const T& rhs)  const
		{
			auto vertexlist_lhs = lhs->get_vertexList();
			auto vertexlist_rhs = rhs->get_vertexList();

			auto nb_vertex_lhs = vertexlist_lhs.size();
			auto nb_vertex_rhs = vertexlist_rhs.size();


			if (nb_vertex_lhs != nb_vertex_rhs) return false;

			std::set<size_t> test_set;

			for (auto i = 0; i < nb_vertex_lhs; i++)
			{
				test_set.insert(vertexlist_lhs[i]->get_localIndex());
				test_set.insert(vertexlist_rhs[i]->get_localIndex());
			}

			return (test_set.size() == nb_vertex_lhs);

		}
	};
	//--Point
	template <>
	struct ElementEqual<Point*>
	{
		bool operator()(Point* lhs, Point* rhs) const
		{
			return		((lhs)->get_coordinates().x == (rhs)->get_coordinates().x)
				&& ((lhs)->get_coordinates().y == (rhs)->get_coordinates().y)
				&& ((lhs)->get_coordinates().z == (rhs)->get_coordinates().z);
		}
	};


	//--Line
	template <>
	struct ElementEqual<Line*>
	{
		bool operator()(Line* lhs, Line* rhs) const
		{
			auto lhs_vertex_list = lhs->get_vertexList();
			auto rhs_vertex_list = rhs->get_vertexList();
			for (auto i = 0; i < lhs_vertex_list.size(); i++)
			{
				if (lhs_vertex_list[i]->get_globalIndex() != rhs_vertex_list[i]->get_globalIndex())
				{
					return false;
				}
			}
			return true;
		}
	};

	//--Polygon
	template <>
	struct ElementEqual<Polygon*>
	{
		bool operator()(Polygon* lhs, Polygon* rhs) const
		{
			auto lhs_vertex_list = lhs->get_vertexList();
			auto rhs_vertex_list = rhs->get_vertexList();
			for (auto i = 0; i < lhs_vertex_list.size(); i++)
			{
				if (lhs_vertex_list[i]->get_globalIndex() != rhs_vertex_list[i]->get_globalIndex())
				{
					return false;
				}
			}
			return true;
		}
	};

	//--Polyhedron
	template <>
	struct ElementEqual<Polyhedron*>
	{
		bool operator()(Polyhedron* lhs, Polyhedron* rhs) const
		{
			auto lhs_vertex_list = lhs->get_vertexList();
			auto rhs_vertex_list = rhs->get_vertexList();
			for (auto i = 0; i < lhs_vertex_list.size(); i++)
			{
				if (lhs_vertex_list[i]->get_globalIndex() != rhs_vertex_list[i]->get_globalIndex())
				{
					return false;
				}
			}
			//return true;
			return false;
		}
	};


	/**
	 * \brief
	 * \tparam T should be a pointer
	 */

	template <class T>
	class ElementCollection : public Ensemble<T, ElementHash<T>, ElementEqual<T>>
	{
	public:

		ElementCollection(ELEMENTS::FAMILY familyType) { m_family = familyType; }

		ELEMENTS::FAMILY get_family() const { return m_family; }

		//Add elements
		T AddElement(std::string label, T Element)
		{
			if (!groupExist(label))
			{
				addAndCreateGroup(label);
			}
			auto grp = m_labelToGroup[label];
			grp->push_back_unique(Element);
			auto add = this->push_back_unique(Element);//TODO:index twice
			return add;
		}



		T AddElement_owned(std::string label, T Element)
		{
			if (!groupExist(label))
			{
				addAndCreateGroup(label);
			}
			auto grp = m_labelToGroup[label];
			grp->push_back_owned_unique(Element);
			auto add = push_back_owned_unique(Element);
			return add;
		}

		T AddElement_ghost(std::string label, T Element)
		{
			if (!groupExist)
			{
				addAndCreateGroup(label);
			}
			auto grp = m_labelToGroup[label];
			grp->push_back_ghost_unique(Element);
			auto add = push_back_ghost_unique(Element);
			return add;
		}

		//Groups
		void addAndCreateGroup(std::string label) { m_labelToGroup[label] = new Ensemble<T, ElementHash<T>, ElementEqual<T>>(); }
		void activeGroup(std::string label) { ASSERT(groupExist(label), "The group does not exist"); m_activeGroup[label] = true; }
		std::unordered_map<std::string, bool>& get_ActiveGroupsMap() { return m_activeGroup; }
		Ensemble<T, ElementHash<T>, ElementEqual<T>>* get_Group(std::string label) { ASSERT(groupExist(label), "The group does not exist"); return m_labelToGroup.at(label); }

		//Parallel
		void ClearAfterPartitioning(std::set<int> owned, std::set<int> ghost);

	protected:

		//Family type
		ELEMENTS::FAMILY m_family;

		//Groups
		bool groupExist(std::string label) { return m_labelToGroup.count(label) == 1; }
		std::unordered_map<std::string, Ensemble<T, ElementHash<T>, ElementEqual<T>>*> m_labelToGroup;
		std::unordered_map<std::string, bool> m_activeGroup;

	};

	template <class T>
	void ElementCollection<T>::ClearAfterPartitioning(std::set<int> owned, std::set<int> ghost)
	{

		//Groups
		for (auto it = m_labelToGroup.begin(); it != m_labelToGroup.end(); ++it)
		{
			it->second->Shrink(owned, ghost);
		}

		//Collection itself
		this->Shrink(owned, ghost);

	}

	typedef ElementCollection<Point*> PointCollection;

	typedef ElementCollection<Line*> LineCollection;

	typedef ElementCollection<Polygon*> PolygonCollection;

	typedef ElementCollection<Polyhedron*> PolyhedronCollection;

}