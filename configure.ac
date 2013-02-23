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
AC_OUTPUT
