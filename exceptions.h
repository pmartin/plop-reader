#ifndef EXCEPTIONS_H_
#define EXCEPTIONS_H_

#include <exception>
#include <stdexcept>
#include <string>


class AbortApplication : public std::runtime_error
{
public:
	AbortApplication(const std::string& message) : std::runtime_error(message + "\n\nThe application cannot continue and will close.") { };
	AbortApplication(void) : std::runtime_error("The application cannot continue and will close.") { };
};


class ConfigFileNotFoundException : public AbortApplication
{
public:
	ConfigFileNotFoundException(const std::string& message) : AbortApplication(message) { };
};


#endif /* EXCEPTIONS_H_ */
