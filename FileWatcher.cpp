
#include <cstdio>
#include <sstream>
#include <errno.h>
#include <string.h>
#include <sys/select.h>

#include "FileWatcher.h"

#include "config.h"

#if HAVE_SYS_INOTIFY_H
#include <sys/inotify.h>
#define BUFF_SIZE ((sizeof(struct inotify_event)+FILENAME_MAX)*10)
#endif

FileWatcher::FileWatcher(const std::string &location, unsigned sleep_interval_secs) : 
	m_sleep_secs(sleep_interval_secs),
	m_fd(-1),
	m_wd(-1),
	m_location(location)
{

#if HAVE_SYS_INOTIFY_H
	m_fd = inotify_init1(IN_NONBLOCK | IN_CLOEXEC);
	if (m_fd < 0) {
		std::stringstream ss;
		ss << "Unable to initialize inotify: " << strerror(errno) << " (errno=" << errno << ").";
		throw FileWatcherException(ss.str());
	}

	m_wd = inotify_add_watch(m_fd, m_location.c_str(), IN_ALL_EVENTS);
	if (m_wd < 0) {
		std::stringstream ss;
		ss << "Unable to initialize inotify watch: " << strerror(errno) << " (errno=" << errno << ").";
		throw FileWatcherException(ss.str());
	}

#endif

}

void FileWatcher::pause()
{

#if HAVE_SYS_INOTIFY_H
	struct timeval timeout;
	timeout.tv_sec = m_sleep_secs;
	timeout.tv_usec = 0;

	int fds = 0;
	fd_set set;
	do {
		// Re-initialize the file descriptors
		FD_ZERO(&set);
		FD_SET(m_fd, &set);

		fds = select(m_fd+1, &set, NULL, NULL, &timeout);
		if (FD_ISSET(m_fd, &set)) {
			if (GetEvent()) {
				return;
			}
		}

	} while ((fds > 0) || (errno == EINTR));
#else
	sleep(sleep_interval_secs);
#endif

}

// Read up to 10 inotify events at a time.
bool FileWatcher::GetEvent() {
	int rc = 0;
#if HAVE_SYS_INOTIFY_H
	int count;
	ssize_t len, i = 0;
	char buff[BUFF_SIZE] = {0};
	do {
		count = 0;
		i = 0;
		while (((len = read(m_fd, buff, BUFF_SIZE)) < 0) && (errno == EINTR)) {}
		while (i < len) { // on error, this while loop is skipped.
			if (len-i < static_cast<ssize_t>(sizeof(struct inotify_event))) {
				break; //Kernel is supposed to keep this from happening.
			}
			struct inotify_event *pevent = (struct inotify_event *)&buff[i];

			if (pevent->mask & (IN_MODIFY|IN_CLOSE_WRITE)) {
				rc = 1;
			}

			// Bump our counters, and try to read out some more.
			count ++;
			i += sizeof(struct inotify_event) + pevent->len;
		}
	} while (count > 0);
#endif
	return rc;
}

FileWatcher::~FileWatcher()
{
	if (m_fd >= 0) {
		close(m_fd);
	}
	if (m_wd >= 0) {
		close(m_wd);
	}
}

