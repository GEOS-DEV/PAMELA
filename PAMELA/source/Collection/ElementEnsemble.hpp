#pragma once
#include <vector>
#include <unordered_map>
#include "Elements/Element.hpp"
#include "Parallel/ParallelEnsemble.hpp"

namespace PAMELA
{

	//Default Equal
	template <class T>
	struct DefaultEqual
	{
		bool operator()(const T lhs, const T rhs)  const
		{

			return (lhs == rhs);

		}
	};


	template <class T, class HashStruct = std::hash<T>, class EqualStruct = DefaultEqual<T>>
	class ElementEnsemble : public ParallelEnsemble<T>
	{

	public:

		//Defaut constructor for empty ensemble
		ElementEnsemble() :ParallelEnsemble< T >() {}

		//Getter
		int get_Index(T element) { return m_pointerToIndex[element]; }

		//Push back T
		void push_back_owned(T data)  override
		{
                    this->m_data.insert(this->end_owned(), data);
                    this->Increment_owned();
		}

		void push_back_ghost(T data)  override
		{
			this->m_data.insert(this->end_ghost(), data);
			this->Increment_ghost();
		}

		void push_back_owned(std::vector<T> data)  override
		{
			this->m_data.insert(this->end_owned(), data.begin(),data.end());
			this->Increment_owned(data.size());
		}

		void push_back_ghost(std::vector<T> data)  override
		{
			this->m_data.insert(this->end_ghost(), data.begin(), data.end());
			this->Increment_ghost(data.size());
		}


		//Push_back unique
		T push_back_owned_unique(T data)
		{
			T returned_element = NULL;
			auto index = static_cast<int>(this->end_owned() - this->begin_owned());
			auto insertion = m_pointerToIndex.insert(std::make_pair(data, index));
			returned_element = insertion.first->first;
			if (insertion.second) //the element is new and the map has been updated
			{
				this->m_data.insert(this->end_owned(), data);
				(*data).set_localIndex(index);
				this->Increment_owned();
				m_elementToLocalIndexMap.insert(std::make_pair(data, index));
				return data;
			}
			return insertion.first->first;
		}

		T push_back_ghost_unique(T data)
		{
			T returned_element = NULL;
			int index = static_cast<int>(this->end_ghost() - this->begin_ghost());
			auto insertion = m_pointerToIndex.insert(std::make_pair(data, index));
			returned_element = insertion.first->first;
			if (insertion.second) //the element is new and the map has been updated
			{
				this->m_data.insert(this->end_ghost(), data);
				(*data).set_localIndex(index);
				this->Increment_ghost();
				m_elementToLocalIndexMap.insert(std::make_pair(data, index));
				return data;
			}
			return insertion.first->first;
		}

		T push_back_unique(T data)   //To be use before partitioning
		{
			T returned_element = NULL;
			int index = static_cast<int>(this->end() - this->begin());
			auto insertion = m_pointerToIndex.insert(std::make_pair(data, index));
			returned_element = insertion.first->first;
			if (insertion.second) //the element is new and the map has been updated
			{
				this->m_data.push_back(data);
				(*data).set_localIndex(index);
				(*data).set_globalIndex(index);
				this->Increment_all();
				m_elementToLocalIndexMap.insert(std::make_pair(data, index));
				return data;
			}
			return insertion.first->first;
		}

		//Make Empty
		void MakeEmpty() override
		{
			this->m_sizeAll = 0;
			this->m_sizeOwned = 0;
			this->m_sizeGhost = 0;
			this->m_data.clear();
			m_pointerToIndex.clear();
			m_elementToLocalIndexMap.clear();
		}

		//Shrink
		void Shrink(std::set<int> owned, std::set<int> ghost) override
		{
			//Copy ghost elements amd owned elements in temporary vector
			std::vector<T> ghost_vec_temp; ghost_vec_temp.reserve(ghost.size());
			std::vector<T> owned_vec_temp; ghost_vec_temp.reserve(owned.size());
			for (auto it = this->m_data.begin(); it != this->m_data.end(); ++it)
			{
				if (ghost.count((*it)->get_localIndex()) == 1)
				{
					ghost_vec_temp.push_back(*it);
				}
				else if (owned.count((*it)->get_localIndex()) == 1)
				{
					owned_vec_temp.push_back(*it);
				}
				else
				{
					//delete *it;  //TODO, need to delete dynamics objects but delete multiple time now. Smart pointer?
					//*it = nullptr;
				}
			}

			//Rebuilt data
			this->m_data.clear();
			this->m_data.reserve(ghost_vec_temp.size() + owned_vec_temp.size());
			this->m_data.insert(this->m_data.end(), owned_vec_temp.begin(), owned_vec_temp.end());
			this->m_data.insert(this->m_data.end(), ghost_vec_temp.begin(), ghost_vec_temp.end());
			this->resize_owned(static_cast<int>(owned_vec_temp.size()));
			this->resize_ghost(static_cast<int>(ghost_vec_temp.size()));

			//Update Numbering and map
			int i = 0;
			for (auto it = this->m_data.begin(); it != this->m_data.end(); ++it)
			{
				(*it)->set_localIndex(i);
				m_elementToLocalIndexMap.insert(std::make_pair((*it), static_cast<int>(it - this->m_data.begin())));
				i++;
			}

			//Test for emptyness
			if (this->m_data.size() == 0) MakeEmpty();

		}

	protected:

		//Pointer to Index
		std::unordered_map<T, int, HashStruct, EqualStruct> m_pointerToIndex;   //Working map only?

		//Map
		std::unordered_map<T, int, HashStruct, EqualStruct> m_elementToLocalIndexMap;

	};

}
