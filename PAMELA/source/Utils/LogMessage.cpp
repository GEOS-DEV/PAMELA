// Header include
#include "Utils/LogMessage.hpp"

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wexit-time-destructors"
#pragma clang diagnostic ignored "-Wglobal-constructors"
#endif

namespace PAMELA
{

	LogMessage* LogMessage::s_instance = nullptr;

	LogMessage* LogMessage::instance()
	{
		return instance(VerbosityLevelLogFile::INFO);
	}

	LogMessage* LogMessage::instance(VerbosityLevelLogFile lvl)
	{
		static LogMessage instance(lvl);
		return &instance;
	}

	LogMessage::LogMessage()
	{
		m_message_level = VerbosityLevelLogFile::INFO;
		m_message << "";
	}

	LogMessage::LogMessage(VerbosityLevelLogFile lvl)
	{
		m_message_level = lvl;
		m_message << "";
	}

}

