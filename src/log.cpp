#include "log.h"

#include "main.h"


static std::string replaceAll(std::string subject, const std::string& search, const std::string& replace)
{
	size_t pos = 0;
	while ((pos = subject.find(search, pos)) != std::string::npos) {
		subject.replace(pos, search.length(), replace);
		pos += replace.length();
	}
	return subject;
}


int Log::logWithLevel(unsigned int level, const char *str ...)
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

	// As we are writing HTML logs, we must escape the HTML special characters!
	std::string escaped = replaceAll(innerBuffer, "&", "&amp;");
	escaped = replaceAll(escaped, "<", "&lt;");
	escaped = replaceAll(escaped, ">", "&gt;");

	time_t ts = time(NULL);
	struct tm* tm_info;
	tm_info = gmtime(&ts);
	char time_buffer[32];
	strftime(time_buffer, sizeof(time_buffer), "%Y:%m:%d %H:%M:%S", tm_info);

	snprintf(outerBuffer, sizeof(outerBuffer), "[%s][%s] %s<br>\n", time_buffer, levelsStrings[level], escaped.c_str());
	int written = iv_fwrite(outerBuffer, sizeof(char), strlen(outerBuffer), fp);

	iv_fclose(fp);

	return written;
}

