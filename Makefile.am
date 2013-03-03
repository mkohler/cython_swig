ACLOCAL_AMFLAGS = -I m4

#
# Build libadder as a shared library.
#
lib_LTLIBRARIES = libadder.la
include_HEADERS = $(srcdir)/libadder/adder.h
libadder_la_SOURCES = libadder/adder.c libadder/adder.h
libadder_la_CPPFLAGS = -Wall -Werror


#
# Build a C test program that uses the shared library.
#
bin_PROGRAMS = test_adder
test_adder_SOURCES = c/test_adder.c
test_adder_CPPFLAGS = -Wall -Werror -I$(srcdir)/libadder -I$(srcdir)/minunit
test_adder_LDADD = libadder.la -lrt


TESTS = test_adder test_extensions.sh

slides:
	rst2odp README.rst cython_swig.odp
