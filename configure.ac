dnl Process this file with autoconf

dnl AC_INIT([upnp-render], m4_esyscmd([echo -n $(git describe --tags --long | sed "s/v\([0-9]\.[0-9]-.*\)-.*/\1/")]), [wvdial@gmail.com])

AC_INIT([upnp-render], [1.0.0], [wvdial@gmail.com])

AC_CONFIG_MACRO_DIR([m4])
AM_INIT_AUTOMAKE

AC_PROG_CXX
AC_PROG_MAKE_SET
AC_PROG_INSTALL

AC_LANG_CPLUSPLUS

dnl Check for libtool
LT_PREREQ([2.2.6])
LT_INIT([disable-static])
LTCFLAGS="-O2"
LTCXXFLAGS="-O2"

AC_SUBST([DOLLAR_SIGN],[$])

AC_HAVE_HEADERS(string.h fcntl.h sys/file.h sys/param.h)
AC_CHECK_FUNCS([alarm])
AC_CHECK_FUNCS([bzero])
AC_CHECK_FUNCS([clock_gettime])
AC_CHECK_FUNCS([memset])
AC_CHECK_FUNCS([socket])
AC_CHECK_FUNCS([strchr])
AC_CHECK_FUNCS([strerror])
AC_CHECK_FUNCS([strstr])
AC_CHECK_FUNCS([strtol])
AC_CHECK_HEADERS([arpa/inet.h])
AC_CHECK_HEADERS([netdb.h])
AC_CHECK_HEADERS([netinet/in.h])
AC_CHECK_HEADERS([sys/socket.h])
AC_CHECK_HEADERS([syslog.h])
AC_CHECK_HEADER_STDBOOL
AC_C_INLINE
AC_FUNC_ERROR_AT_LINE
AC_FUNC_FORK
AC_FUNC_MALLOC
AC_FUNC_MKTIME
AC_PROG_RANLIB
AC_TYPE_INT64_T
AC_TYPE_INT8_T
AC_TYPE_MODE_T
AC_TYPE_PID_T
AC_TYPE_SIZE_T
AC_TYPE_SSIZE_T
AC_TYPE_UID_T
AC_TYPE_UINT16_T
AC_TYPE_UINT32_T
AC_TYPE_UINT64_T
AC_TYPE_UINT8_T

AC_ARG_ENABLE(debug,
AS_HELP_STRING([--enable-debug],
	[enable debugging, default: no]),
		[case "${enableval}" in
		yes) debug=true ;;
		no)  debug=false ;;
		*)   AC_MSG_ERROR([bad value ${enableval} for --enable-debug]) ;;
		esac],
		[debug=false])
AM_CONDITIONAL(DEBUG, test x"$debug" = x"true")

AC_CHECK_LIB([upnp],[UpnpInit],[],[AC_MSG_WARN([upnp library missing])])
AC_CHECK_LIB([ixml],[ixmlParseBufferEx],[],[AC_MSG_ERROR([ixml library missing])])

CPPFLAGS="-W -Wall"

AC_CONFIG_HEADERS([config.h])

AC_CONFIG_FILES([
 Makefile
])

AC_OUTPUT
