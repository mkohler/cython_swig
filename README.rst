.. include:: <s5defs.txt>

=======================
Cython vs. SWIG, Fight!
=======================

:Author: Mark Kohler
:Date: 2013-03-15

Need to use a C library from Python?

"Should I use SWIG or Cython?"

The Rules of this Fight
=======================


Topics
======

"Code first

- C libraries
- SWIG example
- Cython example
- Fear and Magic

.. class:: handout
    
    Cython and SWIG are excellent, and yet very different, tools for using C
    libraries from Python. The goal of this talk is to introduce both tools,
    discuss their strengths, their weaknesses, and the situations that clearly
    favor one tool over the other.

    In other words, I want to show you the code before we move on to gross
    generalizations.




An Aside: Anatomy of C Libraries (3 minutes)
============================================

C source + C header ---------> shared library (binary)
                     compiler,
                     linker

.. class:: handout
    C is the language *Python* is written in. Many of the standard
    libraries are written in C, or are available in C and Python versions.

So, let's assume there are C libraries worth using from Python.
(libxml, sockets, libpng, libusb, ...)

I'm going to show you the smallest C library ever. And then I'll show
you how to access it from Cython and SWIG.

(Quickly list examples, 1 per slide)

Build diagram
=============


C header + SWIG interface ---> shared library + python glue module


Smallest C Library
------------------

C Source File
~~~~~~~~~~~~~

    int
    add(int x, int y) {
        return x + y;
    }

.. class:: handout
    This is a C function which adds to two integers, and returns the
    result::

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

Audience
========

You know C and Python, but you haven't mixed them.


Magic
-----

Show generated code from SWIG and Cython. 

When you look under the covers, it is *still* magic.

Which brings me to fear.

Fears
=====

- If you don't understand a tool, it is easy to be scared.

- My concern is that I would choose a tool, and get 90% of the way there, and
  then find a routine that I couldn't properly wrap, and I would be stuck.

- I would have trouble incorporating it into the build.

- Once incorporated, the tool would be too hard to understand or debug.

- The tool would take too long to get started.


What They Are
-------------

SWIG is...

    Wrapper C libraries for Python and Ruby and Perl and PHP and Scheme and ...
    ...and noticing that there is a lot in common and maybe that can be wrapped
    up and automated.

    If I take something like a header file, with a few hints, I could
    automatically create these wrappers.


Cython is...

    If I take something like a header, I can, line-by-line, reference objects
    as C or Python objects, and convert between them.

    Sort of like in-line assembly.




What is SWIG? (2 minutes)
=========================

SWIG
    Simplified Wrapper and Interface Generator

What goes in?
.. class:: handout
    The SWIG user creates a SWIG inteface file, with a .i extension. The
    SWIG interface file references the C header files for the library
    that 

What comes out?
.. class:: handout
    A C source file to be compiled into a Python extension.
    A Python file to be imported by the Python interpreter.

Is SWIG a language?
.. class:: handout
    Not really. The SWIG interface file is a way of marking up a C
    header file to do some common conversions.

SWIG interface file
-------------------

.. code-block:: c
    %module adder
    %{
    #include "adder.h"
    %}

    int add(int x, int y);

Return Values
-------------

Error

Status returns vs exception handling

adder_sr

SR stands for "status returns" or "second revision"

SWIG and Strings
----------------

By default, i.e. without typemaps, strings passed from scripting language to
SWIG must be read-only.




SWIG Code Walkthrough (5 minutes)
=================================

Should I make slides of the directory tree, and with yellow highlights over the
page I'm doing now.

Examples of standard libraries using SWIG?
Examples of standard libraries using Cython?

Adding structs, arrays and pointers
-----------------------------------

Typemaps (2 minutes)
====================

What is Cython? (2 minutes)
===========================

What goes in?
    
    PXD file: C declarations, in Cython (D stands for declarations)
    PYX file: Cython source

What comes out?

    A C file to be compiled as a Python extension

Cython, the Language (2 minutes)
================================

    Python, with optional C datatypes.

    Big difference

    You can use Cython *instead* of Python.

    Here, we assume that you want to use Python, and Cython is the bridge.


Cython: How is that possible? (2 minutes)
=========================================

build process
-------------

Cython compiles .pyx file to .c file.


Compile C file into shared object library.


toolchain diagram

Cython Workflow
---------------

Take your C header file and (manually) create a .pxd file::
    Copy the file
    Remove semi-colons.
    Convert #defines to variables.
    ints to bints

Create a .pyx file.
    This is where you are really using the Cython language.
    It can be repetitive, but you also have tons of flexibility in making a
    Pythonic interface.

Build a Python extension from the .pyx file. (Create a .so)

Import the .so from plain python.


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


Extra bonus: distutils vs autotools, fight!
===========================================

distutils is much easier for compiling SWIG or Cython extensions.
It is possible with autotools, but ...
(show slide of all the stuff in the Makefile from 90e325):w



Unincorporated Content
======================
- Performance comparison

- String manipulation, C strings

- DRY and maintainability. How much of the header do you have copy?

"SWIG is not in the business of enforcing morality."
    - SWIG documentation, Section 8.3 C String Handling

- Safety comparison

- UTF strings, don't ask
    To a first approximation, ANSI C doesn't do Unicode...so

- Show what happens when you do the wrong thing.
    i.e. send the wrong kind of parameters into a C function


In SWIG, if you get to a C function that doesn't "fit", that SWIG can't wrap, you can go a few ways
    1. If you're lucky, there is always a typemap macro to help you.
    2. If not, you use typemaps to massage things. And finally, if that doesn't work

    3. Write another C library, to wrap the first, and use SWIG to wrap that.


SWIG isn't magic. You have to SWIG each target language separately.
    
What are the rules of this fight?

Not that long ago, I was given several C libraries...

This is a Python conference. Why am I talking about C?
