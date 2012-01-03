/***************************************************************
 *
 * Copyright (C) 2011 University of Nebraska-Lincoln
 * 
 * Licensed under the Apache License, Version 2.0 (the "License"); you
 * may not use this file except in compliance with the License.  You may
 * obtain a copy of the License at
 * 
 *    http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ***************************************************************/

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
