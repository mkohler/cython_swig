ACLOCAL_AMFLAGS = -I m4

#
# Build libadder as a shared library.
#
lib_LTLIBRARIES = libadder.la
include_HEADERS = $(srcdir)/libadder/adder.h
libadder_la_SOURCES = libadder/adder.c libadder/adder.h
libadder_la_CPPFLAGS = -Wall -Werror


#
# Build a C program that uses the shared library.
#
bin_PROGRAMS = c_adder
c_adder_SOURCES = c/c_adder.c
c_adder_CPPFLAGS = -Wall -Werror -I$(srcdir)/libadder
c_adder_LDADD = libadder.la
TESTS = c_adder


#
# Create a Python extension using SWIG.
#
EXTRA_DIST = swig/adder.i
python_PYTHON = $(srcdir)/swig/adder.py
pyexec_LTLIBRARIES = _adder.la
# distutils parses python's Makefile to capture the flags used to compile
# python itself. I'm not going to do that, so I watched distutils compile an
# extension and copied these flags.
PY_CFLAGS = -pthread -fno-strict-aliasing -DNDEBUG -g -fwrapv -O2 \
	-Wall -Wstrict-prototypes -fPIC
_adder_la_SOURCES = swig/adder_wrap.c
_adder_la_CPPFLAGS = $(PY_CFLAGS) \
					 $(AX_SWIG_PYTHON_CPPFLAGS) \
					 -I$(srcdir)/libadder
_adder_la_LIBADD = libadder.la
# -module allows one to use non-standard library names,
#  e.g. _foo.la instead of libfoo.la.
_adder_la_LDFLAGS = -module

$(srcdir)/swig/adder_wrap.c $(srcdir)/swig/adder.py: $(srcdir)/swig/adder.i
	$(SWIG) $(AX_SWIG_PYTHON_OPT) -I$(srcdir)/libadder $<

MAINTAINERCLEANFILES = $(srcdir)/swig/adder_wrap.c $(srcdir)/swig/adder.py

#
# Test that the SWIG'd extension works.
#
dist_noinst_SCRIPTS = $(srcdir)/swig/test_adder.py
DISTCLEANFILES = test_adder.py
TESTS += test_adder.py
TESTS_ENVIRONMENT = PYTHONPATH=.libs:$(srcdir)/swig;
test_adder.py: $(srcdir)/swig/test_adder.py
	cp $< $@
	chmod +x $@
