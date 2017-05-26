// Header include
#include "Utils/LogMessage.hpp"

LogMessage* LogMessage::s_instance = nullptr;

LogMessage* LogMessage::instance()
{
  return instance(VerbosityLevelLogFile::INFO);
}

LogMessage* LogMessage::instance(VerbosityLevelLogFile lvl)
{
  static LogMessage s_instance(lvl);
  return &s_instance;
}

LogMessage::LogMessage()
{
  m_message_level = VerbosityLevelLogFile::INFO;
  m_message << "";
};

LogMessage::LogMessage(VerbosityLevelLogFile lvl)
{
  m_message_level = lvl;
  m_message << "";
};