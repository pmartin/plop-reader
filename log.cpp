#include "log.h"

#include "belladonna.h"


int Log::log(unsigned int level, const char *str ...)
{
	FILE *fp = iv_fopen(FILEPATH, "a");

	char outerBuffer[2048];
	char innerBuffer[1500];

	va_list args;
	va_start(args, str);
	vsnprintf(innerBuffer, sizeof(innerBuffer), str, args);
	va_end(args);

	const char *levelsStrings[] = {"debug", "info", "warn", "error"};
	if (level > sizeof(levelsStrings) - 1) {
		level = sizeof(levelsStrings) - 1;
	}

	snprintf(outerBuffer, sizeof(outerBuffer), "[%ld][%s] %s<br>\n", time(NULL), levelsStrings[level], innerBuffer);
	int written = iv_fwrite(outerBuffer, sizeof(char), strlen(outerBuffer), fp);

	iv_fclose(fp);

	return written;
}

