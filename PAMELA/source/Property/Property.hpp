#pragma once



namespace PAMELA
{
	template <class T1,class T2>
	class Property
	{
	public:
		Property(T1* owner)
		{
			m_Owner = owner;
		}

		//Getter
		std::unordered_map<std::string, ParallelEnsemble<T2>>& get_PropertyMap() { return m_data; }
		T1* get_Owner() { return m_Owner; }

		//Property
		void ReferenceProperty(std::string label)
		{
			m_data[label] = ParallelEnsemble<T2> ();
			//Ensemble<T2>(m_Owner->size_owned(), 0);
		}

		//void CreateProperty(std::string label)
		//{
		//	ReferenceProperty(label);
		//	m_data[label] = std::vector<T2>(m_Owner->size_owned(),0);
		//}

		void SetConstantProperty(std::string label, T2 val)
		{
			ASSERT(m_data.count(label) == 1, "Property does not exist");
			this->m_data[label] = ElementEnsemble<T2>(m_Owner->size_owned(),val);
		}

		void SetProperty(std::string label, std::vector<T2> val)
		{
			ASSERT(m_data.count(label) == 1, "Property does not exist");
			//TODO:ASSERT(static_cast<int>(val.size) == m_Owner->size_owned(), "Input vector should be same size as the owner");
			m_data[label].push_back_owned(val);
		}

		void GetProperty_owned(std::string label) { return m_data[label].data(); }



		void ClearAfterPartitioning(std::set<int> owned, std::set<int> ghost)
		{

			for (auto it = m_data.begin(); it != m_data.end(); ++it)
			{
				it->second.Shrink(owned, ghost);
			}

		}


	protected:

		T1* m_Owner;
		//std::unordered_map<std::string, std::vector<T2>> m_data;
		std::unordered_map<std::string, ParallelEnsemble<T2>> m_data;

		
	};
	
}
