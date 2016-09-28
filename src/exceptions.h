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


class ConfigFileInvalidException : public AbortApplication
{
public:
	ConfigFileInvalidException(const std::string& message) : AbortApplication(message) { };
};


class ConfigFileMissingItemException : public AbortApplication
{
public:
	ConfigFileMissingItemException(const std::string& message) : AbortApplication(message) { };
};




class SyncAbortAllOperations : public std::runtime_error
{
public:
	SyncAbortAllOperations(const std::string& message) : std::runtime_error(message + "\n\nThe synchronization cannot go on.") { };
	SyncAbortAllOperations(void) : std::runtime_error("The synchronization cannot go on.") { };
};


class SyncOAuthException : public SyncAbortAllOperations
{
public:
	SyncOAuthException(const std::string& message) : SyncAbortAllOperations(message) { };
};


class SyncHttpException : public SyncAbortAllOperations
{
public:
	SyncHttpException(const std::string& message) : SyncAbortAllOperations(message) { };
};


class SyncInvalidJsonException : public SyncAbortAllOperations
{
public:
	SyncInvalidJsonException(const std::string& message) : SyncAbortAllOperations(message) { };
};


#endif /* EXCEPTIONS_H_ */
