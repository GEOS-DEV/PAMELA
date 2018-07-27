#pragma once

// Std library includes
#include <sstream>

namespace PAMELA
{

	enum class VerbosityLevelLogFile
	{
		ERROR, WARNING, DEBUG, INFO
	};


	class LogMessage
	{

	public:

		static LogMessage* instance();
		static LogMessage* instance(VerbosityLevelLogFile lvl);

		std::string getMessage() const { return m_message.str(); }
		VerbosityLevelLogFile& getMessageLevel() { return m_message_level; }


		template<typename T>
		LogMessage& operator<<(const T& msg)
		{
			m_message << msg;
			return *this;
		}

	private:

		VerbosityLevelLogFile m_message_level;

		std::stringstream m_message;

		static LogMessage* s_instance;

		LogMessage();
		explicit LogMessage(VerbosityLevelLogFile lvl);


	};

}