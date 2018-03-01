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
		std::unordered_map<std::string, std::vector<T2>>& get_PropertyMap() { return m_data; }
		T1* get_Owner() { return m_Owner; }

		//Property
		void ReferenceProperty(std::string label)
		{
			m_data[label] = std::vector<T2>(m_Owner->size_owned(), 0);
		}

		//void CreateProperty(std::string label)
		//{
		//	ReferenceProperty(label);
		//	m_data[label] = std::vector<T2>(m_Owner->size_owned(),0);
		//}

		void SetConstantProperty(std::string label, T2 val)
		{
			ASSERT(m_data.count(label) == 1, "Property does not exist");
			m_data[label] = std::vector<T2>(m_Owner->size_owned(),val);
		}

		void SetProperty(std::string label, std::vector<T2> val)
		{
			ASSERT(m_data.count(label) == 1, "Property does not exist");
			//TODO:ASSERT(static_cast<int>(val.size) == m_Owner->size_owned(), "Input vector should be same size as the owner");
			m_data[label] = val;
		}

	protected:

		T1* m_Owner;
		std::unordered_map<std::string, std::vector<T2>> m_data;
	};
	
}