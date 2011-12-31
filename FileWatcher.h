
#include <string>
#include <exception>

class FileWatcherException : public std::exception {

public:
	FileWatcherException(const std::string &what_str) : m_what(what_str) {}
	~FileWatcherException() throw (){}

	virtual const char * what() const throw()
	{
		return m_what.c_str();
	}

private:
	const std::string m_what;

};

class FileWatcher {

public:
	FileWatcher(const std::string &location, unsigned sleep_seconds);
	~FileWatcher();

	/*
	 * Pause for sleep_seconds or until our file is modified.
	 */
	void pause();

private:

	bool GetEvent();
	unsigned m_sleep_secs;
	int m_fd, m_wd;
	const std::string m_location;

};
