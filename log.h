#ifndef LOG_H_
#define LOG_H_

#include "inkview.h"

#include <string>

#define DEBUG(str...) Log::getInstance().logWithLevel(Log::LOG_DEBUG, str);
#define LOG(str...) Log::getInstance().logWithLevel(Log::LOG_INFO, str);
#define INFO(str...) Log::getInstance().logWithLevel(Log::LOG_INFO, str);
#define WARN(str...) Log::getInstance().logWithLevel(Log::LOG_WARN, str);
#define ERROR(str...) Log::getInstance().logWithLevel(Log::LOG_ERROR, str);


class Log
{
public:
	enum {LOG_DEBUG=0, LOG_INFO, LOG_WARN, LOG_ERROR};

	int logWithLevel(unsigned int level, const char *str ...);

	static Log &getInstance()
	{
		static Log instance;
		return instance;
	}

	Log(Log const&) = delete;
	void operator=(Log const&) = delete;

private:
	const char *DIR = FLASHDIR;
	const char *FILEPATH = FLASHDIR "/plop-reader-logs.html";

	Log() {};
	~Log() {};
};


#endif /* LOG_H_ */
