#pragma once
#include <vector>
#include <set>

namespace PAMELA
{

	class ParallelEnsembleBase
	{

	public:
		virtual ~ParallelEnsembleBase() = default;

		ParallelEnsembleBase() :m_sizeAll(0), m_sizeOwned(0), m_sizeGhost(0) {}

		//Size
		size_t size_all() const { return m_sizeAll; }
		size_t size_owned() const { return m_sizeOwned; }
		size_t size_ghost() const { return m_sizeGhost; }

		//Resize
		void resize_owned(size_t size) { m_sizeOwned = size; m_sizeAll = m_sizeOwned + m_sizeGhost; }
		void resize_ghost(size_t size) { m_sizeGhost = size; m_sizeAll = m_sizeOwned + m_sizeGhost; }

		//Increment
		void Increment_all() { m_sizeAll = m_sizeAll + 1; m_sizeOwned = m_sizeOwned + 1; }
		void Increment_ghost() { m_sizeGhost = m_sizeGhost + 1; m_sizeAll = m_sizeAll + 1; }
		void Increment_owned() { m_sizeOwned = m_sizeOwned + 1; m_sizeAll = m_sizeAll + 1; }

		void Increment_all(size_t increment) { m_sizeAll = m_sizeAll + increment; m_sizeOwned = m_sizeOwned + increment; }
		void Increment_ghost(size_t increment) { m_sizeGhost = m_sizeGhost + increment; m_sizeAll = m_sizeAll + increment; }
		void Increment_owned(size_t increment) { m_sizeOwned = m_sizeOwned + increment; m_sizeAll = m_sizeAll + increment; }

	protected:

		//virtual ~EnsembleBase()=0;// {}


		//Sizes
		size_t m_sizeAll;
		size_t m_sizeOwned;
		size_t m_sizeGhost;

	};


	template <class T>
	class ParallelEnsemble : public ParallelEnsembleBase
	{
	public:

		//Defaut constructor for empty ensemble
		ParallelEnsemble() :ParallelEnsembleBase() {}

		//Reserve capacity for storage
		void reserve(int n) { m_data.reserve(n); }

		//Iterators
		typedef typename std::vector<T>::iterator collection_iterator;

		collection_iterator begin() { return m_data.begin(); }
		collection_iterator end() { return m_data.end(); }

		collection_iterator begin_owned() { return m_data.begin(); }
		collection_iterator end_owned() { return m_data.begin() + size_owned(); }

		collection_iterator begin_ghost() { return m_data.begin() + size_owned(); }
		collection_iterator end_ghost() { return m_data.end(); }

		//Operators
		T &operator[](size_t i) { return m_data[i]; }


		//Push back T
		virtual void push_back_owned(T data)
		{
			m_data.insert(this->end_owned(), data);
			Increment_owned();
		}

		virtual void push_back_ghost(T data)
		{
			m_data.insert(this->end_ghost(), data);
			Increment_ghost();
		}

		virtual void push_back_owned(std::vector<T> data)
		{
			m_data.insert(this->end_owned(), data.begin(), data.end());
			Increment_owned(data.size());
		}

		virtual void push_back_ghost(std::vector<T> data)
		{
			m_data.insert(this->end_ghost(), data.begin(), data.end());
			Increment_ghost(data.size());
		}


		virtual void MakeEmpty()
		{
			m_sizeAll = 0;
			m_sizeOwned = 0;
			m_sizeGhost = 0;
			m_data.clear();
		}


		//Shrink
		virtual void Shrink(std::set<int> owned, std::set<int> ghost)
		{
			//Copy ghost elements amd owned elements in temporary vector
			std::vector<T> ghost_vec_temp; ghost_vec_temp.reserve(ghost.size());
			std::vector<T> owned_vec_temp; ghost_vec_temp.reserve(owned.size());
			for (auto it = m_data.begin(); it != m_data.end(); ++it)
			{
				if (ghost.count(static_cast<int>(it - m_data.begin())) == 1)
				{
					ghost_vec_temp.push_back(*it);
				}
				else if (owned.count(static_cast<int>(it - m_data.begin())) == 1)
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

			//Test for emptyness
			if (m_data.empty()) MakeEmpty();

		}

		//Data
		std::vector<T>& data_all() { return m_data; }

	protected:

		//data
		std::vector<T> m_data;

	};



}
