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
TESTS = c_adder


#
# Create a Python extension using SWIG.
EXTRA_DIST = swig/adder.i
python_PYTHON = $(srcdir)/swig/adder.py
pyexec_LTLIBRARIES = _adder.la
_adder_la_SOURCES = swig/adder_wrap.c
_adder_la_CPPFLAGS = $(AX_SWIG_PYTHON_CPPFLAGS) -I$(srcdir)/libadder
_adder_la_LIBADD = libadder.la
# -module allows one to use non-standard library names,
#  e.g. _foo.la instead of libfoo.la.
_adder_la_LDFLAGS = -module

$(srcdir)/swig/adder_wrap.c $(srcdir)/swig/adder.py: $(srcdir)/swig/adder.i
	$(SWIG) $(AX_SWIG_PYTHON_OPT) $<

MAINTAINERCLEANFILES = $(srcdir)/swig/adder_wrap.c $(srcdir)/swig/adder.py
DISTCLEANFILES = test_adder.py

dist_noinst_SCRIPTS = $(srcdir)/swig/test_adder.py
TESTS += test_adder.py
TESTS_ENVIRONMENT = PYTHONPATH=.libs:$(srcdir)/swig;
test_adder.py: $(srcdir)/swig/test_adder.py
	cp $< $@
	chmod +x $@
