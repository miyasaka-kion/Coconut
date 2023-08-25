#pragma once

#include <string>
#include <iostream>

namespace Coconut {
	class Log {
		Log();
		~Log();
		
		void dump(std::string msg);

	private:
		struct m_TimeInfo {
			int year;
			int month;
			int day;
			int hour;
			int minute;
			int second;
		};
		Coconut::Log::m_TimeInfo m_getCurrentTime();

	private:
		// Todo: maybe some log info stuff..

	};
}