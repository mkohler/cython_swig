AC_PREREQ([2.68])
AC_INIT([adder], [0.1], [mark.kohler@gmail.com])
AM_INIT_AUTOMAKE([-Wall -Werror foreign subdir-objects])

LT_INIT

AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([Makefile])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_SRCDIR([libadder/adder.h])

# AM_PATH_PYTHON is built-in to automake.
AM_PATH_PYTHON([2.7])

# AX_PYTHON_DEVEL, AX_PKG_SWG, and AX_SWIG_PYTHON come from the GNU Autoconf
# archive.
AX_PYTHON_DEVEL([>=2.7])
AX_PKG_SWIG(2.0, [], [ AC_MSG_ERROR([SWIG is required to build..]) ])
AX_SWIG_PYTHON

AC_PROG_CC([libadder/adder.h])
AC_OUTPUT
