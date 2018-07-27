#pragma once
#ifndef FILE_HPP_INCLUDED
#define FILE_HPP_INCLUDED

#include <string>
#include <fstream>

class File
{

public:
  explicit File(std::string fileName);
  ~File();

  std::string getFullName() const { return m_FullName; }
  std::string getDirectory() const { return m_Directory; }
  std::string getShortName() const { return m_ShortName; }
  std::string getExtension() const { return m_Extension; }
  std::string getNameWithoutExtension() const { return m_NameWithoutExtension; }
  bool exists() const 
  {
	  std::ifstream f(m_FullName); 
	  return f.good();
  }
  
private:

  std::string m_FullName;
  std::string m_Directory;
  std::string m_ShortName;
  std::string m_Extension;
  std::string m_NameWithoutExtension;

};

#endif //FILE_HPP_INCLUDED