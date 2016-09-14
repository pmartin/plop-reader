#ifndef LOG_H_
#define LOG_H_

#include "inkview.h"


#define DEBUG(str...) Log::getInstance().log(0, str);
#define LOG(str...) Log::getInstance().log(1, str);
#define INFO(str...) Log::getInstance().log(1, str);
#define WARN(str...) Log::getInstance().log(2, str);
#define ERROR(str...) Log::getInstance().log(3, str);


class Log
{
public:
	int log(unsigned int level, const char *str ...);

	static Log &getInstance()
	{
		static Log instance;
		return instance;
	}

	Log(Log const&) = delete;
	void operator=(Log const&) = delete;

private:
	const char *DIR = FLASHDIR;
	const char *FILEPATH = FLASHDIR "/belladonna-logs.html";

	Log() {};
	~Log() {};
};


#endif /* LOG_H_ */
