Cython vs. SWIG, Fight!

Cython and SWIG are excellent, and yet very different, tools for using C libraries from Python. The goal of this talk is to introduce both tools, discuss their strengths, their weaknesses, and the situations that clearly favor one tool over the other.



Introduction (2 minutes)
========================

Not that long ago, I was given several C libraries, 

What are the rules of this fight?


What is SWIG? (2 minutes)
=========================

SWIG
    Simplified Wrapper and Interface Generator

What goes in?
    The SWIG user creates a SWIG inteface file, with a .i extension. The
    SWIG interface file references the C header files for the library
    that 

What comes out?
    A C source file to be compiled into a Python extension.
    A Python file to be imported by the Python interpreter.

Is SWIG a language?
    Not really. The SWIG interface file is a way of marking up a C
    header file to do some common conversions.

An Aside: Anatomy of C Libraries (3 minutes)
============================================

This is a Python conference. Why am I talking about C?

C is the language *Python* is written in. Many of the standard libraries
are written in C, or are available in C and Python versions.

So, let's assume there are C libraries worth using from Python.
(libxml, sockets, libpng, libusb, ...)

I'm going to show you the smallest C library ever. And then I'll show
you how to access it from Cython and SWIG.

(Quickly list examples, 1 per slide)

Build diagram

C source + C header ---> shared library

C header + SWIG interface ---> shared library + python glue module


Smallest C Library
------------------

C Source File
~~~~~~~~~~~~~

This is a C function which adds to two integers, and returns the
result::

    int
    add(int x, int y) {
        return x + y;
    }

Header File
~~~~~~~~~~~


In C, the interface to a function is typically declared in a separate file, a
*header* file. Here is the header file for our libadder::

    int add(int, int);

Three parts:
    the *name* of the function
    the *types* of the parameters
    the *type* of the return value

The header also can include the names of the parameters, but these are ignored.

SWIG interface file
~~~~~~~~~~~~~~~~~~~

%module adder
%{
#include "adder.h"
%}

int add(int x, int y);


SWIG Code Walkthrough (5 minutes)
=================================

Examples of standard libraries using SWIG?
Examples of standard libraries using Cython?

Adding structs, arrays and pointers
-----------------------------------

Typemaps (2 minutes)
====================

What is Cython? (2 minutes)
===========================

What goes in?
    
    PXD file    
    PYX file

What comes out?

    A C file to be compiled as a Python extension

Cython, the Language (2 minutes)
================================

    Python, with optional C datatypes.


Cython: How is that possible? (2 minutes)
=========================================

build process
-------------

Cython compiles .pyx file to .c file.
Compile C file into shared object library.


toolchain diagram

Cython Code Walkthrough (5 minutes)
===================================

SWIG Advantages and Disadvantages(1 minute)
===========================================

Create Extensions for Other Languages
-------------------------------------

If you write C library code, and you want to provide bindings (wrappers)
for Python, Java, and Ruby, SWIG can do that.

autotools support, sort of
+ SWIG doesn't pull the gcc flags from the Python Makefile like distutils
        
learning curve of typemaps

Cython Advantages and Disadvantages (1 minute)
==============================================

    Evolve the interface.
    Do performance optimizations "just in time"
    Cython headers

Chart of Use Cases and Tool Recommendation (1 minute)
=====================================================




There are a lot of details in doing this kind of work.


Can you paint yourself into a corner?

Fear: At the beginning of a project, when you're deciding what tools
you're going to invest your time in, there's a concern that you will
pick a tool that gets you 90% of the way there.

Neither of these tools will do that, I think?

Alternatives to Cython and SWIG (2 minutes)
===========================================

    writing extensions with the Python C/API

    ctypes
