// Header include
#include "StringUtils.hpp"

// Std library includes
#include <algorithm>


namespace PAMELA
{

	void StringUtils::Trim(std::string& str)
	{
		if (str.size() > 0)
		{
			int first = int(str.find_first_not_of(' '));
			int last = int(str.find_last_not_of(' '));
			if (first < 0)
			{
				str.resize(0);
			}
			else
			{
				str = str.substr(first, (last - first + 1));
			}

		}
	}


	bool StringUtils::RemoveStringAndFollowingContentFromLine(std::string ToBeRemoved, std::string& line)
	{
		// does the line have a semi-colon?
		std::size_t pos = line.find(ToBeRemoved);
		bool res = false;
		if (pos != std::string::npos)
		{
			// remove the character and everything afterwards
			line = line.substr(0, pos);
			res = true;
		}
		return res;
	}

	std::string StringUtils::RemoveString(std::string ToBeRemoved, std::string data)
	{
		std::size_t pos = data.find(ToBeRemoved);

		if (pos != std::string::npos)
		{
			std::string line;
			std::size_t L1 = ToBeRemoved.size();
			std::size_t L2 = data.size();
			line = data.substr(pos + L1, L2 - 1);
			return line;
		}
		else
		{
			return data;
		}

	}


	void StringUtils::RemoveTab(std::vector<std::string>& v)
	{

		for (int i = 0; i < v.size(); i++)
		{
			std::replace(v[i].begin(), v[i].end(), '\t', ' ');
		}


	}

	void StringUtils::RemoveEndOfLine(std::vector<std::string>& v)
	{

		for (int i = 0; i < v.size(); i++)
		{
			std::replace(v[i].begin(), v[i].end(), '\n', ' ');
		}

	}

	void StringUtils::RemoveTab(std::string& v)
	{

		std::replace(v.begin(), v.end(), '\t', ' ');

	}

	void StringUtils::RemoveEndOfLine(std::string& v)
	{

		std::replace(v.begin(), v.end(), '\n', ' ');

	}

	bool StringUtils::ExtendDefault(int n, std::string& v)
	{

		std::stringstream iss(v);
		std::string sub;
		int cpt = 0;
		while (std::getline(iss, sub, ' '))  //Count number of elements in string
		{
			cpt++;
		}

		v = iss.str();

		for (int i = cpt; i < n; i++)
		{
			v = v + " DEFAULT";
		}

		RemoveExtraSpaces(v);

		return cpt <= n;


	}


	void StringUtils::RemoveExtraSpaces(std::string& v)
	{

		v.erase(std::unique(v.begin(), v.end(),
			[](char a, char b) { return a == ' ' && b == ' '; }), v.end());

	}


	std::string StringUtils::GetNthWord(int n, std::string data)
	{

		std::size_t found = data.find(" ");

		if ((found == std::string::npos) && (n > 1))
			return "";

		int i = n - 1;
		while (i > 0) //Remove n-1 first words
		{
			auto pos = data.find_first_of(' ');
			if (pos == std::string::npos) data = "";
			data = data.substr(pos + 1);
			i--;
		}

		return data.substr(0, data.find(' '));

	}


	std::istream& StringUtils::safeGetline(std::istream& is, std::string& t)
	{
		t.clear();

		// The characters in the stream are read one-by-one using a std::streambuf.
		// That is faster than reading them one-by-one using the std::istream.
		// Code that uses streambuf this way must be guarded by a sentry object.
		// The sentry object performs various tasks,
		// such as thread synchronization and updating the stream state.

		std::istream::sentry se(is, true);
		std::streambuf* sb = is.rdbuf();

		for (;;)
		{
			int c = sb->sbumpc();
			switch (c)
			{
			case '\n':
				return is;
			case '\r':
				if (sb->sgetc() == '\n')
					sb->sbumpc();
				return is;
			case EOF:
				// Also handle the case when the last line has no line ending
				if (t.empty())
					is.setstate(std::ios::eofbit);
				return is;
			default:
				t += static_cast<char>(c);
			}
		}
	}

}