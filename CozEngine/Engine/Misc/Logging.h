#pragma once

#include <iostream>
#include <string>

enum class LLogLevel
{
	ERROR,
	WARNING,
	INFO
};

inline void Log(const LLogLevel LogLevel, const std::string& Text)
{
#if defined(_DEBUG)
	switch (LogLevel)
	{
	case LLogLevel::ERROR:
		std::cout << "ERROR - " << Text << "\n";
		break;
	case LLogLevel::WARNING:
		std::cout << "WARNING - " << Text << "\n";
		break;
	case LLogLevel::INFO:
		std::cout << "INFO - " << Text << "\n";
		break;
	default:
		break;
	}
#endif
}

