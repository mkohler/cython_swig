.. include:: <s5defs.txt>

=======================
Cython vs. SWIG, Fight!
=======================

:Author: Mark Kohler
:Date: 2013-03-16

Using C libraries in Python

.. class:: handout

    Hello, Welcome to Cython vs SWIG, Fight!.
    
    Today I'm going to talk about using C libraries in Python.
   
    What is this fight about?
    Wrapping C libraries 
    
    Well, not everything is in the standard library. 
    In fact, there may even be things you want that you can't get from
    the cheese shop.

    You may have 

    Cython and SWIG are two excellent tools for wrapping C libraries
    that don't already have Python bindings.
    cutting-edge algorithms

    Cython and SWIG are excellent, and yet very different, tools for using C
    libraries from Python. The goal of this talk is to introduce both tools,
    discuss their strengths, their weaknesses, and the situations that clearly
    favor one tool over the other.
    

Pre-Fight
=========

- CPython


- C extensions

http://docs.python.org/2/extending/
.. class:: handout

    What is this fight about?


    At some point, 

    You may have 

    You may need to use a C library

.. class:: handout

    If you haven't seen the term "CPython" before, it refers to the most
    popular Python implementation, the one hosted on python.org, and the
    one 

    CPython is the 


Why?
====

I'm going to show you the smallest C library ever. And then I'll show
you how to access it from Cython and SWIG.

(Quickly list examples, 1 per slide)

The Rules of the Fight
======================

- C libraries
- a *very* small library
- fear and magic

.. class:: handout

    - review C library anatomy
    - show a tiny C library
    - show SWIG
    - show Cython
    - Grow the library, and the SWIG and Cython interfaces.
    - Fear and Magic

    How am I going to present this?

    I'm going to quickly review the structure of C lib

    I'm going to show you a *very* small C library.
    
    Then I will show you how to access it, using SWIG first, and then using
    Cython. And then we will grow the library, a function at a time, and
    see how SWIG and Cython handle more complicated interfaces.

    And then I will talk about fear and magic.

    Finally, I will make some gross generalizations.

    This talk is for people who know C and Python, but haven't mixed them.
    

    "Code first and ask questions later."

    In other words, I want to show you the code before we move on to gross
    generalizations.


Anatomy of C Libraries
======================

C source + C header ---------> shared library (binary)
                     compiler,
                     linker



.. class:: handout
    TODO: Indicate on the slide which files are human-generated
    and which are computer-generated.
    Let's talk about C libraries.

    C libraries consists of:

        the C source code

        a header file, which describe the library interface,
        ...what goes in, and what comes out.

        a binary, known as a shared libary or shared object

    If you're not familiar with C...

    C is the language *Python* is written in. Many of the standard
    libraries are written in C, or are available in C and Python versions.

    CPython vs Python

import this
===========

import socket

.. class:: handout

    What happens when you execute this line? Is socket a Python file?

    No. socket is a Python extension. That is, it is compiled machine code.

    So, in case you didn't know this. Python can import both "pure python"
    \*.py" files, as well as shared objects, as long as they were built
    with the python interface.

adder.c: add
============

adder.c

.. code-block:: c

    int
    add(int x, int y) {
        return x + y;
    }

.. class:: handout

    This is a C function which adds to two integers, and returns the
    result...

    For this discussion, we're going to assume the library is already
    written. Maybe you wrote it, or maybe it's a third-party library,
    and all you have is a header file and a binary, but either way,
    we're going to assume we don't want to change the library's interface.

adder.h: add
============

adder.h

.. code-block:: c

    int add(int, int);

.. class:: handout

    In C, the interface to a function is typically declared in a separate file,
    a *header* file. Here is the header file for our libadder.

    Three parts:
        the *name* of the function
        the *types* of the parameters
        the *type* of the return value

    The header also can include the names of the parameters, but these are ignored.

    And you can see, it repeats a lot of the information that was in adder.c

    We will see this again in the SWIG and Cython interface files, which repeat
    a lot of information which is the C header file, which is C's *interface*
    file. 

    This is something to watch for, because as our projects gets larger, this
    represents more work...and more source for problems.

Build diagram
=============

C header + SWIG interface ---> shared library + python glue module

.. class:: handout

    With SWIG, you look at the C header file, and you write a C interface file,
    which, has pretty much the same information as the C header file, but it 
    is in the *SWIG* format.

    Then the SWIG tool, and remember, this IS THE ENTIRE POINT OF SWIG, it

    takes the C header file,
    and the SWIG interface file, and it generates two files:

       C source that wraps your C functions, (~4000)
       Python file that calls the C extension you *will build* from the
       C source. (100 lines)


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

adder.i (SWIG interface file)
=============================

.. code-block:: c

    %module adder
    %{
    #include "adder.h"
    %}

    int add(int x, int y);

.. class:: handout

    If you look at this a bit, you may think, "Hey, I've seen this before" and
    yes, it's pretty similar to the C header file.

Cython: Big Picture
===================

Given: header + shared object

You create:
    Cython interface file (.pxd)
    Cython source file (.pyx)

Cython will build:
    a Python extension


.. class:: handout 

.pxd
    - the interface file
    - references a C header

.pyx
    - Cython source code
    - reference .pxd file

    Take those two, plus a shared library, and you 
        Now let's look at Cython. Here's the 

Cython: cy_adder.pxd
====================

.. code-block:: c

    cdef extern from "adder.h":
        int add(int x, int y)

.. class:: handout

    Hmm...it sure looks a lot like the SWIG interface file. It has some other
    magic words, but it references a C header file, and it contains
    information that is very similar to the C header file.

Cython: cy_adder.pyx
====================

.. code-block:: python

    cimport c_adder

    def add(x, y):
        return c_adder.add(x, y)

.. class:: handout

    Now, this is new. This file does not have an analog in the SWIG
    workflow.
    
    This is a Cython source file. This is the red pill.


Cython, the language
====================

- almost 100% compatible with Python

- optional static types

- can use C libraries, with the cimport statement
    
.. class:: handout

    Now, the Cython source file, a PYX file. You need to write some
    Cython code that will get transformed into C code, which gets
    compiled, and that is your Python extension, which the Python
    interpreter can import.

    Line-by-line...the cimport line is referencing the "Cython*
    interface file.

    Finally, we have some Cython source code.


Cython Source File, 2
=====================

.. code-block:: python

    ADDER_VERSION = c_adder.ADDER_VERSION
    def get_version():
        return c_adder.get_version()

    def make_greeting(name):
        return c_adder.make_greeting(name)


adder.c: get_version
====================

.. code-block:: c

    char *
    get_version(void) {
        return "v1.0";
    }


Strings: Cython
===============



SWIG and Return Values
======================

Error

Status returns vs exception handling

adder_sr

SR stands for "status returns" or "second revision"

SWIG and Strings
================

"SWIG is not in the business of enforcing morality."
    - SWIG documentation, Section 8.3 C String Handling

By default, i.e. without typemaps, strings passed from scripting language to
SWIG must be read-only.




SWIG Code Walkthrough (5 minutes)
=================================

Should I make slides of the directory tree, and with yellow highlights over the
page I'm doing now.

Examples of standard libraries using SWIG?
Examples of standard libraries using Cython?

SWIG: structs, arrays and pointers
==================================

.. class:: handout

    This gets to the crux of one of the limits of SWIG. What can it do and more
    importantly, what *can't* it do.

    And what it can't do is de-reference a pointer.

    It can pass pointers around. And that's pretty powerful. You can have your
    Python object essentially holding a C pointer, but you can't de-reference
    it.

    Anything you get out of SWIG needs to be in the form that it can convert
    from a C object to a python object.

    It can convert numbers, ints and floats, at least, automatically.

    It can convert strings, with your help. (As long as strings means ASCII.)

    TODO: structs?

What is Cython? (2 minutes)
===========================

What goes in?
    
    PXD file: C declarations, in Cython (D stands for declarations)
    PYX file: Cython source

What comes out?

    A C file to be compiled as a Python extension

.. class:: handout

    So, Cython, let's review where we are. Again, like SWIG, 
    you start with your C header file, and compiled object.

    Then *you* write a PXD file, which is *Cython's* interface file format.


Cython, the Language (2 minutes)
================================

    Python, with optional C datatypes.

    Big difference

    You can use Cython *instead* of Python.

    Here, we assume that you want to use Python, and Cython is the bridge.


Cython: How is that possible? (2 minutes)
=========================================

Cygin: build process
====================


Cython compiles .pyx file to .c file.


Compile C file into shared object library.


toolchain diagram

Cython Workflow
===============

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

.. class:: handout

    Cython inteface file
    write your Cython 
    mash those up.

Strings
=======

Cython and C strings
====================

What They Are
=============

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


SWIG Advantages and Disadvantages(1 minute)
===========================================


Create Extensions for Other Languages
=====================================

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


Magic
=====

.. class:: handout

    I've been a C programmer for a long time, and when I first saw how easy it
    was to play with the sockets library, to play with, interactively, from the
    Python prompt, I was amazed. 
    
    It was so different from the world I lived in, and compiling, and linking
    and just plain (pause) waiting for the build to finish.
    
    The python way seemed like magic to me.

    Then later, when I saw 

Show generated code from SWIG and Cython. 

When you look under the covers, it is *still* magic.

Which brings me to fear.

Fear
====

- If you don't understand a tool, it is easy to be scared.

- My concern is that I would choose a tool, and get 90% of the way there, and
  then find a routine that I couldn't properly wrap, and I would be stuck.

- I would have trouble incorporating it into the build.

- Once incorporated, the tool would be too hard to understand or debug.

- The tool would take too long to get started.


Alternatives to Cython and SWIG (2 minutes)
===========================================

    writing extensions with the Python C/API

    ctypes


Extra bonus: distutils vs autotools, fight!
===========================================

distutils is much easier for compiling SWIG or Cython extensions.


It is possible with autotools, but ...
(show slide of all the stuff in the Makefile from 90e325):w

.. class:: handout

    (pause). 
    distutils wins. distutils includes 

    Did anyone knows that distutils includes code to parse Makefiles?
    It's in sysconfig.py, and it actually parses the Makefile that builds the
    Python interpreter. There's nothing like that in the autotools.

    For the code in this presentation, I started out with autotools, but
    switched to distutils because I thought it was just too ugly to watch
    what distutils did, and copy that.
    CFLAGS from a 


Resources
=========

code and slides:
    https://github.com/mkohler/swig_cython

rst2odp:
    https://github.com/mattharrison/rst2odp.git

.. class:: handout

    Let me thank Matt Harrison for his restructed-text-to-libreoffice-impress
    tool. I am loving restructuredText, and rst2odp let me create this
    presentation in restructuredText and convert it to libreOffice Impress.

End
===

Unincorporated Content
======================
- Performance comparison

- DRY and maintainability. How much of the header do you have copy?

- Safety comparison

- UTF strings, don't ask
    To a first approximation, ANSI C doesn't do Unicode...so

- Show what happens when you do the wrong thing.
    i.e. send the wrong kind of parameters into a C function


SWIG isn't magic. You have to SWIG each target language separately.
    
What are the rules of this fight?

Not that long ago, I was given several C libraries...

This is a Python conference. Why am I talking about C?

SWIG: Can You Get Stuck?
========================

.. class:: handout

    In SWIG, if you get to a C function that doesn't "fit", that SWIG can't
    wrap, you can go a few ways:

        1. If you're lucky, there is always a typemap macro to help you.
        2. If not, you use typemaps to massage things. And finally, if that
           doesn't work
        3. Write another C library, to wrap the first, and use SWIG to wrap
           that.

TODO
====

Have to include a C struct example.


SWIG: Other Uses
================

SWIG will create C wrappers 


SWIG: Other Use Cases

Complicated Bits I Did Not Mention
==================================

SWIG typemaps

shared libraries
    Writing Shared Libraries by Ulrich Drepper
