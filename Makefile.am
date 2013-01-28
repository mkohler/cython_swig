ACLOCAL_AMFLAGS = -I m4

#
# Build libadder as a shared library.
#
lib_LTLIBRARIES = libadder.la
include_HEADERS = $(srcdir)/libadder/adder.h
libadder_la_SOURCES = libadder/adder.c libadder/adder.h


#
# Build a C program that uses the shared library.
#
bin_PROGRAMS = c_adder
c_adder_SOURCES = c/c_adder.c
c_adder_CPPFLAGS = -I$(srcdir)/libadder
c_adder_LDADD = libadder.la


#
# Create a Python extension using SWIG.
#
python_PYTHON = adder.py
pyexec_LTLIBRARIES = _adder.la
BUILT_SOURCES = adder_wrap.c
_adder_la_SOURCES = adder_wrap.c swig/adder.i
_adder_la_CPPFLAGS = $(AX_SWIG_PYTHON_CPPFLAGS) -I$(srcdir)/libadder
_adder_la_LIBADD = libadder.la
# -module allows one to use non-standard library names, e.g. _foo.la instead of
#  libfoo.la.
_adder_la_LDFLAGS = -module

adder_wrap.c: $(srcdir)/swig/adder.i
	$(SWIG) $(AX_SWIG_PYTHON_OPT) -o $@ $<
