#pragma once

#include <string>

namespace PAMELA
{
enum class GroupType
{
  CORNER,
  LINE,
  SURFACE,
  VOLUME,
  UNDEFINED
};

class GroupInfo
{
public:
  GroupInfo( const std::string& name, int index, GroupType groupType ) :
    m_name( name ),
    m_index( index ),
    m_groupType( groupType )
  {
  }

  GroupInfo() :
    m_name("UNDEFINED"),
    m_index( -1 ),
    m_groupType(GroupType::UNDEFINED)
  {
  }

  int Index() const
  {
    return m_index;
  }

  const std::string & Name() const
  {
    return m_name;
  }

  GroupType Type() const
  {
    return m_groupType;
  }

private:
  /// Name of the group
  std::string m_name;

  /// Index of the group
  int m_index;

  GroupType m_groupType;

};
}
