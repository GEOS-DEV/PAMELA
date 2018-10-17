// Header include
#include "Logger.hpp"
#include "Parallel/Communicator.hpp"
#include "Utils.hpp"

#define LEVEL_LOG_FILE "DEBUG"
#define LEVEL_LOG_SCREEN "BRIEF"
#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wexit-time-destructors"
#pragma clang diagnostic ignored "-Wglobal-constructors"
#endif
namespace PAMELA
{

        //Default values
	std::string Logger::m_level_logfile = "INFO";
	std::string Logger::m_file_name = "simulator.log";
	std::string Logger::m_level_screen = "BRIEF";
	std::string Logger::m_MPI_prefix = "";


	Logger* Logger::instance()
	{
		static Logger s_instance(LEVEL_LOG_FILE, m_file_name, LEVEL_LOG_SCREEN);
		return &s_instance;
	}


	void Logger::init(std::string LevelLogFile, std::string file_name, std::string LevelScreen)
	{
		m_level_logfile = LevelLogFile;
		m_file_name = file_name;
		m_level_screen = LevelScreen;
	}

	Logger::Logger(std::string LevelLogFile, std::string file_name, std::string LevelScreen)
	{

		//Log file
		if (LevelLogFile == "ERROR")
			m_log_level = VerbosityLevelLogFile::ERROR;
		if (LevelLogFile == "WARNING")
			m_log_level = VerbosityLevelLogFile::WARNING;
		if (LevelLogFile == "INFO")
			m_log_level = VerbosityLevelLogFile::INFO;
		if (LevelLogFile == "DEBUG")
			m_log_level = VerbosityLevelLogFile::DEBUG;

		m_logfile.open(file_name);

		//Screen
		if (LevelScreen == "SILENT")
			m_screen_level = VerbosityLevelScreen::SILENT;
		if (LevelScreen == "BRIEF")
			m_screen_level = VerbosityLevelScreen::BRIEF;
		if (LevelScreen == "ALL")
			m_screen_level = VerbosityLevelScreen::ALL;

		//MPI
#ifdef WITH_MPI
		m_MPI_prefix = std::to_string(Communicator::worldRank()) + " >>> ";
#else
		m_MPI_prefix = "";
#endif

	}


	void Logger::LogERROR(std::string msg)
	{

		std::string message = "[" + get_time() + "] ERROR:   " + msg.c_str();
		write_file(message);
		if (m_screen_level >= VerbosityLevelScreen::BRIEF)
			write_screen("ERROR:   " + msg + " in file " + __FILE__ + " line " + std::to_string(__LINE__));
                abort();
	}

	void Logger::LogWARNING(std::string msg)
	{

		std::string message = "[" + get_time() + "] WARNING: " + msg.c_str();
		if (m_log_level >= VerbosityLevelLogFile::WARNING)
			write_file(message);
		if (m_screen_level >= VerbosityLevelScreen::BRIEF)
			write_screen("WARNING: " + msg);

	}

	void Logger::LogINFO(std::string msg)
	{
		std::string message = "[" + get_time() + "] INFO:    " + msg.c_str();
		if (m_log_level >= VerbosityLevelLogFile::INFO)
			write_file(message);
		if (m_screen_level >= VerbosityLevelScreen::BRIEF)
			write_screen(msg);
	}

	void Logger::LogDEBUG(std::string msg)
	{

		std::string message = "[" + get_time() + "] DEBUG:   " + msg.c_str();
		if (m_log_level >= VerbosityLevelLogFile::DEBUG)
			write_file(message);
		if (m_screen_level >= VerbosityLevelScreen::ALL)
			write_screen(msg);

	}


	void Logger::Log(LogMessage& msg)
	{

		std::string str_msg = msg.getMessage();
		VerbosityLevelLogFile MessageLevel = msg.getMessageLevel();
		if (MessageLevel == VerbosityLevelLogFile::ERROR)
		{
			LogERROR(str_msg);
		}
		else if (MessageLevel == VerbosityLevelLogFile::WARNING)
		{
			LogWARNING(str_msg);
		}
		else if (MessageLevel == VerbosityLevelLogFile::DEBUG)
		{
			LogDEBUG(str_msg);
		}
		else if (MessageLevel == VerbosityLevelLogFile::INFO)
		{
			LogINFO(str_msg);
		};

	}



	std::string Logger::get_time()
	{
		std::stringstream res;
		time_t t = time(nullptr);
		struct tm *now;
#ifdef WIN32
		struct tm now_st;
		now = &now_st;
		localtime_s(now, &t);
#else
		now = localtime(&t);
#endif
		res << std::put_time(now, "%Y-%m-%d %H:%M:%S");
		//res << (now.tm_year + 1900) << "-" << (now.tm_mon + 1) << "-" << (now.tm_mday) << "-" << now.tm_hour << ":" << now.tm_min << ":" << now.tm_sec;
		return res.str();
	}

}
