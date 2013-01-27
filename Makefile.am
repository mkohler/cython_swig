ACLOCAL_AMFLAGS = -I m4

lib_LTLIBRARIES = libadder.la
libadder_la_SOURCES = libadder/adder.c libadder/adder.h
include_HEADERS = $(srcdir)/libadder/adder.h

bin_PROGRAMS = adder_example
adder_example_SOURCES = c/main.c
adder_example_LDADD = libadder.la
adder_example_CPPFLAGS = -I$(srcdir)/libadder
