#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.61)
AC_INIT([urbackupserver_bdbplugin], [0.1], [martin@urbackup.org])
AC_CONFIG_SRCDIR([dllmain.cpp])
AC_CONFIG_HEADER([config.h])
AM_INIT_AUTOMAKE

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC
AC_PROG_LIBTOOL

# Checks for libraries.

BDB_CHECK($lib)

if !BDB
	echo "Sorry, you need BerkleyDB."
	echo "Please look for distribution packages(libdb-dev) or get it from http://www.oracle.com/technetwork/database/berkeleydb/"
	exit 1
endif

if !BDBSQL
	echo "Sorry, you need SQL support for BerkleyDB."
	echo "Please look for distribution packages(libdb-sql-dev) or get it from http://www.oracle.com/technetwork/database/berkeleydb/"
	exit 1
endif

# Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS([stdlib.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_C_CONST
AC_TYPE_SIZE_T

# Checks for library functions.

AC_CONFIG_FILES([Makefile])
AC_OUTPUT