#pragma once

#include <string>
#include <iostream>
#include <memory>
#include <bitset>

namespace Coconut {
	class Log {
	public:
		Log();
		~Log();
		
		void dump(std::string msg);

	private:
		//time info
		struct m_TimeInfo {
			int year;
			int month;
			int day;
			int hour;
			int minute;
			int second;
		};

		Coconut::Log::m_TimeInfo m_getCurrentTime();
		static void tmp_getTimeAppend(m_TimeInfo timeInfo);

	private:
		// Todo: maybe some log info stuff..

	public:
		inline static  				std::shared_ptr<Coconut::Log> getLogger() {
			return m_Logger;
		}

	private:
		static std::shared_ptr<Coconut::Log> m_Logger;

	};

}

#define CC_LOG(...) ::Coconut::Log::getLogger()->dump(__VA_ARGS__)

//template<typename T>
//constexpr auto CC_LOG(T ...) { return ::Coconut::Log::getLogger()->dump(__VA_ARGS__); }