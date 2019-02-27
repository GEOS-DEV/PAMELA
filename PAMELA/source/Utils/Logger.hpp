#pragma once

// Project includes
#include "Utils/LogMessage.hpp"

// Std library includes
#include <fstream>
#include <chrono>
#include <iostream>
#include <iomanip>

////Macros
#define LOGWARNING(msg) do {Logger::instance()->LogWARNING(msg);} while (0)
#define LOGERROR(msg) do {Logger::instance()->LogERROR(msg);} while (0)
#define LOGINFO(msg) do {Logger::instance()->LogINFO(msg);} while (0)
#define LOGDEBUG(msg) do {Logger::instance()->LogDEBUG(msg);} while (0)

//ADGPRS Native style
#define LogFatal(msg) do {Logger::instance()->Log((*(LogMessage::instance(VerbosityLevelLogFile::ERROR))<<msg));} while (0)
#define LogMessage2(lvl) do {*(LogMessage::instance(VerbosityLevelLogFile::lvl));} while (0)

namespace PAMELA
{

	enum class VerbosityLevelScreen
	{
		SILENT, BRIEF, ALL
	};


	enum ErrorCode // negative is error, positive is success
	{
		FAILED = -1,
		SUCCEEDED = 1,
		CONVERGED = 2,
		DID_NOT_CONVERGE_YET = 4,
		GOT_STUCK_ON_STATIONARY_POINT = -8,
		MAKE_SCHUR_COMPLEMENT_FAILED = -16,
		LINEAR_SOLVER_FAILED = -32,
		NEGATIVE_PRESSURE_FOUND = -64,
		FAILED_TO_DEFINE_STATUS = -128,
		SOLUTION_UPDATE_BELOW_THRESHOLD = 256,
		RESIDUAL_NORM_BELOW_THRESHOLD = 512,
		PROBLEM_IN_SCHUR_BLOCK = -1024,
		GOT_STACK_ON_STATIONARY_POINT_GEOMECH = -2048,
		NOT_SUPPORTED = -4096
	};


	class Logger
	{

	public:

		static Logger* instance();
		static void init(std::string LevelLogFile, std::string file_name, std::string LevelScreen);

		[[ noreturn ]] void LogERROR(std::string msg) __attribute__((noreturn));
		void LogWARNING(std::string msg);
		void LogINFO(std::string msg);
		void LogDEBUG(std::string msg);

		void Log(LogMessage& msg);

	protected:

		Logger(std::string level_logfile, std::string file_name, std::string level_screen);

		//Input
		static std::string m_level_logfile;
		static std::string m_file_name;
		static std::string m_level_screen;
		//MPI rank
		static std::string m_MPI_prefix;

	private:

		//Screen
		VerbosityLevelScreen m_screen_level;

		//Log file
		VerbosityLevelLogFile m_log_level;



		std::ofstream m_logfile;

		template<typename T>
		static void write_screen(T &&t);

		template<typename Head, typename Tail, typename... Tails>
		static void write_screen(Head &&head, Tail &&tail, Tails&&... tails);

		template<typename T>
		void write_file(T &&t);

		template<typename Head, typename Tail, typename... Tails>
		void write_file(Head &&head, Tail &&tail, Tails&&... tails);

		static std::string get_time();

	};


	template<typename T>
	void Logger::write_screen(T &&t)
	{
		std::cout << m_MPI_prefix;
		std::cout << t << std::endl;
	}

	template<typename Head, typename Tail, typename... Tails>
	void Logger::write_screen(Head &&head, Tail &&tail, Tails&&... tails)
	{
		std::cout << head;
		write_screen(std::forward<Tail>(tail), std::forward<Tails>(tails)...);
	}

	template<typename T>
	void Logger::write_file(T &&t)
	{
		m_logfile << t << std::endl;
	}

	template<typename Head, typename Tail, typename... Tails>
	void Logger::write_file(Head &&head, Tail &&tail, Tails&&... tails)
	{
		m_logfile << head;
		write_file(std::forward<Tail>(tail), std::forward<Tails>(tails)...);
	}

}
