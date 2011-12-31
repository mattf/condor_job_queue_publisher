#AC_PREREQ([2.63])
AC_INIT([condor-job-queue-publisher], [0.44], [claudiol@redhat.com])
AC_CONFIG_SRCDIR([config.h.in])
AC_CONFIG_HEADERS([config.h])

AM_INIT_AUTOMAKE(condor-job-queue-publisher,0.44, no-define)

# Check for boost headers
AC_LANG_PUSH([C++])

AC_CHECK_HEADERS([qpid/messaging/Connection.h qpid/messaging/Message.h qpid/messaging/Sender.h qpid/messaging/Session.h], [], [AC_MSG_FAILURE([qpid headers not found])])

AC_CHECK_HEADER([boost/flyweight.hpp], [AC_DEFINE([HAVE_BOOST_FLYWEIGHT], [1], [Define to 1 if you have <boost/flyweight.hpp>.])])

AC_MSG_CHECKING([for -lqpidmessaging])
SAVED_LDFLAGS=$LDFLAGS
LDFLAGS="$LDFLAGS -lqpidmessaging"
AC_LINK_IFELSE(
  [AC_LANG_PROGRAM([#include "qpid/messaging/Connection.h"],
  [qpid::messaging::Connection connection])],
  [AC_MSG_RESULT([yes])], [AC_MSG_FAILURE([qpidclient library is required])]
)
LDFLAGS=$SAVED_LDFLAGS

AC_LANG_POP([C++])

# Checks for header files.
AC_CHECK_HEADERS([syslog.h unistd.h sys/inotify.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_TYPE_SIZE_T

#LT_INIT
#AC_SUBST([LIBTOOL_DEPS])
AC_PROG_INSTALL

#PKG_CHECK_MODULES(DEPS, log4cpp >= 1.0)
AC_SUBST(DEPS_CXXFLAGS)
AC_SUBST(DEPS_LIBS) 

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC

# Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS([limits.h stdlib.h string.h strings.h syslog.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_C_CONST
AC_TYPE_SIZE_T

# Checks for library functions.
AC_FUNC_MALLOC
AC_FUNC_REALLOC
AC_CHECK_FUNCS([strcasecmp strdup strtol])

AC_CONFIG_FILES([Makefile])
AC_OUTPUT

