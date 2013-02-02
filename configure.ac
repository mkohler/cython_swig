AC_PREREQ([2.68])
AC_INIT([adder], [0.1], [mark.kohler@gmail.com])

# Initialize automake.
# 'foreign' indicates this is not a GNU project.
# 'subdir-objects' puts the objects in a directory tree like the source tree.
AM_INIT_AUTOMAKE([-Wall -Werror foreign silent-rules subdir-objects])

# Initialize libtool
LT_INIT

AC_CONFIG_FILES([Makefile])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_SRCDIR([configure.ac])

# Find a C compiler.
AC_PROG_CC()

# AM_PATH_PYTHON is built-in to automake and is needed by the PYTHON primary.
AM_PATH_PYTHON([2.7])

# AX_PYTHON_DEVEL, AX_PKG_SWG, and AX_SWIG_PYTHON come from the GNU Autoconf
# archive, and are used for building Python extensions wih SWIG.
AX_PYTHON_DEVEL([>=2.7])
AX_PKG_SWIG(2.0, [], [ AC_MSG_ERROR([SWIG is required to build..]) ])
AX_SWIG_PYTHON

AC_OUTPUT
