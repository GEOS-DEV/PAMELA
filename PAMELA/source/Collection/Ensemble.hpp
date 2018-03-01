#pragma once
#include <vector>
#include <unordered_map>

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


	class EnsembleBase
	{

	public:

		EnsembleBase() :m_sizeAll(0), m_sizeOwned(0), m_sizeGhost(0) {};

		//Size
		int size_all() const { return m_sizeAll; }
		int size_owned() const { return m_sizeOwned; }
		int size_ghost() const { return m_sizeGhost; }

		//Resize
		void resize_owned(int size) { m_sizeOwned = size; m_sizeAll = m_sizeOwned + m_sizeGhost; }
		void resize_ghost(int size) { m_sizeGhost = size; m_sizeAll = m_sizeOwned + m_sizeGhost; }

		//Increment
		void Increment_all() { m_sizeAll = m_sizeAll + 1; m_sizeOwned = m_sizeOwned + 1; }
		void Increment_ghost() { m_sizeGhost = m_sizeGhost + 1; m_sizeAll = m_sizeAll + 1; }
		void Increment_owned() { m_sizeOwned = m_sizeOwned + 1; m_sizeAll = m_sizeAll + 1; }

	protected:

		//virtual ~EnsembleBase()=0;// {}


		//Sizes
		int m_sizeAll;
		int m_sizeOwned;
		int m_sizeGhost;

	};



	template <class T, class HashStruct = std::hash<T>, class EqualStruct = DefaultEqual<T>>
	class Ensemble : public EnsembleBase
	{

	public:

		//Defaut constructor for empty ensemble
		Ensemble() :EnsembleBase() {}

		//Reserve capacity for storage
		void reserve(int n) { m_data.reserve(n); }

		//Getter
		int get_Index(T element) { return m_pointerToIndex[element]; }

		//Iterators
		typedef typename std::vector<T>::iterator collection_iterator;

		collection_iterator begin() { return m_data.begin(); }
		collection_iterator end() { return m_data.end(); }

		collection_iterator begin_owned() { return m_data.begin(); }
		collection_iterator end_owned() { return m_data.begin() + size_owned(); }

		collection_iterator begin_ghost() { return m_data.begin() + size_owned(); }
		collection_iterator end_ghost() { return m_data.end(); }

		//Operators
		T &operator[](int i) { return m_data[i]; }


		//Push_back
		T push_back_owned_unique(T data)
		{
			T returned_element = NULL;
			int index = static_cast<int>(this->end_owned() - this->begin_owned());
			auto insertion = m_pointerToIndex.insert(std::make_pair(data, index));
			returned_element = insertion.first->first;
			if (insertion.second) //the element is new and the map has been updated
			{
				m_data.insert(this->end_owned(), data);
				(*data).set_localIndex(index);
				Increment_owned();
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
				m_data.insert(this->end_ghost(), data);
				(*data).set_localIndex(index);
				Increment_ghost();
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
				m_data.push_back(data);
				(*data).set_localIndex(index);
				(*data).set_globalIndex(index);
				Increment_all();
				m_elementToLocalIndexMap.insert(std::make_pair(data, index));
				return data;
			}
			return insertion.first->first;
		}

		//Make Empty
		void MakeEmpty();

		//Shrink
		void Shrink(std::set<int> owned, std::set<int> ghost)
		{
			//Copy ghost elements amd owned elements in temporary vector
			std::vector<T> ghost_vec_temp; ghost_vec_temp.reserve(ghost.size());
			std::vector<T> owned_vec_temp; ghost_vec_temp.reserve(owned.size());
			for (auto it = m_data.begin(); it != m_data.end(); ++it)
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
			m_data.clear();
			m_data.reserve(ghost_vec_temp.size() + owned_vec_temp.size());
			m_data.insert(m_data.end(), owned_vec_temp.begin(), owned_vec_temp.end());
			m_data.insert(m_data.end(), ghost_vec_temp.begin(), ghost_vec_temp.end());
			resize_owned(static_cast<int>(owned_vec_temp.size()));
			resize_ghost(static_cast<int>(ghost_vec_temp.size()));

			//Update Numbering and map
			int i = 0;
			for (auto it = m_data.begin(); it != m_data.end(); ++it)
			{
				(*it)->set_localIndex(i);
				m_elementToLocalIndexMap.insert(std::make_pair((*it), it - m_data.begin()));
				i++;
			}

			//Test for emptyness
			if (m_data.size() == 0) MakeEmpty();

		}


	protected:

		//data
		std::vector<T> m_data;

		//Pointer to Index
		std::unordered_map<T, int, HashStruct, EqualStruct> m_pointerToIndex;   //Working map only?

		//Map
		std::unordered_map<T, int, HashStruct, EqualStruct> m_elementToLocalIndexMap;



	};

	template <class T, class HashStruct, class EqualStruct>
	void Ensemble<T, HashStruct, EqualStruct>::MakeEmpty()
	{
		m_sizeAll = 0;
		m_sizeOwned = 0;
		m_sizeGhost = 0;
		m_data.clear();
		m_pointerToIndex.clear();
		m_elementToLocalIndexMap.clear();
	}

}