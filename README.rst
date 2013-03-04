.. include:: <s5defs.txt>

=======================
Cython vs. SWIG, Fight!
=======================

:Author: Mark Kohler
:Date: 2013-03-16

Using C libraries in Python

.. class:: handout

    Hello, welcome to Cython vs SWIG, Fight!.

    This talk is about what happens when you realize that *not* everything
    is in the standard library. In fact, there may be things you want
    that you can't even get from the *cheese shop*.

    If those things are C libraries that you want for Python,
    you're in luck, because there are excellent tools available for
    wrapping C libraries for Python.

Pre-Fight
=========

"Code first and ask questions later."

- import statement
- C libraries
- growing a library with SWIG and Cython

.. class:: handout

    To start this talk, I'm going to to quickly discuss
    the Python import statement and what it hides.

    Then, I'll review the structure of *C* libraries. That is, libraries
    written in *C*, to be used by program written in *C*.

    From there, I'll show you a very small C library, we'll create
    Python intefaces for it using both SWIG and Cython, and then we'll
    grow the library, adding new features and Python interfaces for
    those features.

    At the end, I'll talk about fear and magic, make some gross
    generalizations, and take questions.

import this
===========

>>> import socket

What is socket.__file__?

>>> import datetime

What is datetime.__file__?

>>> import time

What is time.__file__?

.. class:: handout

    First, I have a question. What does import *import*?

    Each of these statements imports a module. What exactly is being
    imported? Or to put it another way, after the import. what does the
    module object's dunder file object contain?

    This is a bit of a trick question, because the answer depends on the
    platform, but I want to make sure everyone understands the 3
    3 possibilities.


import socket
=============

.. code-block:: python

    >>> socket.__file__
    '/usr/lib/python2.7/socket.pyc'
    >>>

.. class:: handout

    This is the most straight-forward case. The import statement caused
    the interpreter to load a .pyc file, containing architecture-neutral
    python byte-code into memory.

import datetime
===============

.. code-block:: python

    >>> datetime.__file__
    '/usr/lib/python2.7/lib-dynload/datetime.so'
    >>>

    $ file /usr/lib/python2.7/lib-dynload/datetime.so
    ELF 64-bit LSB shared object, x86-64, dynamically linked
    $

.. class:: handout

    This is the case we care about. The import statement caused a Python
    extension module to be loaded into memory. The ".so" stands for
    Shared Object, and it is architecture-specific machine code. In this
    example, the shared object uses the x86-64 instruction set.
    Nevertheless, when you import it, it looks and feels like Python code.

    That's pretty great. Applications can import modules containing
    either interpreted Python code, or compiled machine code, and the
    interface is exactly the same.

    And that's why we're here. That's what we'll be doing with SWIG and
    Cython, taking compiled code, that doesn't know anything about
    Python, wrapping it up, and turning it into Python extension
    modules, that work just like pure Python code.

import time
===========

.. code-block:: python

    >>> time.__file__
    Traceback (most recent call last):
      File "<stdin>", line 1, in <module>
    AttributeError: 'module' object has no attribute '__file__'
    >>>

.. class:: handout

    The last case isn't relevant to SWIG or Cython, but I thought I
    should mention it anyway, as it confused the hell out of me the
    first time I went to look for a Python module on disk, and I
    couldn't find a correponding .py or .pyc or .so file.

    The answer to this riddle is that some Python extension modules are
    linked with the Python interpreter when it is built. And those
    module don't have dunder file attributes.

See libraries
=============

1. Compile

    **adder.c + adder.h** --> adder.o

2. Link

    adder.o --> libadder.so

.. class:: handout

    Let's talk about C libraries.

    The library we're going to talk about is called adder, because it
    adds two numbers, and because snake references are easier to make
    than Monty Python references.

    The slide shows the build process for libadder. The names in bold
    are human-generated files. The others are machine-generated. (pause)

    If you're not familiar with C:

    adder.c is the source code. We will look at it next.

    adder.h describes the interface to the library.

    and libadder.so is the final shared object, ready to be linked with
    programs and other shared libraries.

    Now let's look at some code.

adder.c: add()
==============

adder.c

.. code-block:: c

    int
    add(int x, int y) {
        return x + y;
    }

.. class:: handout

    This is a C function which adds to two integers, and returns the
    result.

    For this discussion, we're going to assume the library is already
    written. Maybe you wrote it, or maybe it's a third-party library,
    and all you have is a header file and a binary, but either way,
    we're going to assume we don't want to change the library's interface.

adder.h: add()
==============

adder.h

.. code-block:: c

    int add(int, int);

.. class:: handout

    In C, the interface to a function is typically declared in a separate file,
    a *header* file. Here is the header file for our libadder.

    It has three parts:
        the *name* of the function
        the *types* of the parameters
        the *type* of the return value

    As you may notice, it consists entirely of information that was
    already in adder.c.

    This kind of repetition is something to watch out for, because
    keeping the same information in two places makes for troublesome
    code maintenance...and we're going to see a lot more repetition in
    the SWIG and Cython workflow.

    Now that we have covered the C build process, let's look at SWIG.

adder.i (SWIG interface file)
=============================

.. code-block:: c

    %module adder
    %{
    #include "adder.h"
    %}

    int add(int x, int y);

.. class:: handout

    To use SWIG, you, the programmer, have to create one file, the SWIG
    interface, or .i file. This is adder.i, the SWIG interface file for
    libadder.

    If you look at the last line a bit, you may think, "Hey, I've seen
    this before" and yes, it's identical to the C header file.

    And for SWIG-friendly interfaces, I don't want to say simple
    interfaces, for SWIG-friendly interfaces, the .i file is a little
    boilerplate, followed by copy-and-pasting the C header file.

    Now, what do you do with this file? Let's look at the SWIG build
    diagram.

SWIG build diagram
==================

1. SWIG

    **adder.h + adder.i** --> adder_wrap.c + adder.py

2. Compile

    **adder.h** + adder-wrap.c --> adder_wrap.o

3. Link

    adder_wrap.o + libadder.so --> _adder.so

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

    What goes in?

        The SWIG user creates a SWIG inteface file, with a .i extension. The
        SWIG interface file references the C header files for the library
        that

    What comes out?

        A C source file to be compiled into a Python extension.
        A Python file to be imported by the Python interpreter.

cy_adder.pxd: add()
===================

.. code-block:: cython

    cdef extern from "adder.h":
        int add(int x, int y)

.. class:: handout

    Hmm...it sure looks a lot like the SWIG interface file. It has some other
    magic words, but it references a C header file, and it contains
    information that is very similar to the C header file.

cy_adder.pyx: add()
===================

.. code-block:: cython

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

Cython build diagram
====================

1. Cython

    **adder.h + cy_adder.pxd + cy_adder.pyx** --> cy_adder.c

2. Compile

   **adder.h** + cy_adder.c --> cy_adder.o

3. Link

   cy_adder.o --> cy_adder.so

.. class:: handout

    So, Cython, let's review where we are. Again, like SWIG,
    you start with your C header file, and compiled object.

    Then *you* write a PXD file, which is *Cython's* interface file format.

    Given: header + shared object

    You create:
        Cython interface file (.pxd)
        Cython source file (.pyx)

    Cython will build:
        a Python extension

    .pxd
        - the interface file
        - references a C header

    .pyx
        - Cython source code
        - reference .pxd file

        Take those two, plus a shared library, and you 
            Now let's look at Cython. Here's the 

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

adder.h: pair_add()
===================

.. code-block:: c

    typedef struct _PAIR {
        int x;
        int y;
    } PAIR;

    int pair_add(PAIR * ppair);

adder.c: pair_add()
===================

.. code-block:: c

    int
    pair_add(PPAIR ppair) {
        return ppair->x + ppair->y;
    }

adder.i: pair_add()
===================

.. code-block:: c

    typedef struct _PAIR {
        int x;
        int y;
    } PAIR;

    int pair_add(PPAIR);


adder.pxd: pair_add()
=====================

.. code-block:: cython

    ctypedef struct PAIR:
        int x
        int y

    int pair_add(PAIR * ppair)


adder.pyx: pair_add()
=====================

.. code-block:: cython

    def pair_add(x, y):
        cdef c_adder.PAIR pair
        pair.x = x
        pair.y = y
        return c_adder.pair_add(&pair)

.. class:: handout

    Takes a Python object and returns a Python object.


test_swig.py: pair_add
========================

.. code-block:: python

    def test_pair():
        pair = adder.PAIR()
        pair.x = 3
        pair.y = 4
        eq_(adder.pair_add(pair), 7)

.. class:: handout

    eq\_ is an assert from nose unit-testing framework.

test_cython.py: pair_add
========================

.. code-block:: cython

    def test_pair_add():
        eq_(cy_adder.pair_add(3, 4), 7)

adder.h: get_version()
======================

.. code-block:: c

    char * get_version(void);

adder.c: get_version()
======================

.. code-block:: c

    static char version[] = "v1.0";

    char *
    get_version(void) {
        return version;
    }

adder.i: get_version()
=============================

.. code-block:: c

    char * get_version(void);

adder.pxd: get_version()
========================

.. code-block:: c

    char * get_version()

Using SWIG's get_version
========================

.. code-block:: python

    print adder.get_version()

Using Cython's get_version
==========================

.. code-block:: python

    print cy_adder.get_version()

Cython and C Strings
====================

"In many use cases, C strings (a.k.a. character pointers) are slow and
cumbersome. [...] Generally speaking: unless you know what you are doing,
avoid using C strings where possible and use Python string objects
instead."

    - Cython documentation, General Notes about C strings

.. class:: handout

    C Strings, the source of all good buffer overflows. Let's see what
    the Cython documentation says about C strings.

SWIG and C Strings
==================

"The problems (and perils) of using char * are well-known. However, SWIG
is not in the business of enforcing morality."

    - SWIG documentation, Section 8.3 C String Handling

.. class:: handout

    Speaking as a C programmer *and* a Python programmer, C strings are a
    nightmare. Really, it's not fair to to even call them strings. They
    are fixed-size, mutable, arrays of bytes.

SWIG and C Strings, part 2
==========================

By default, i.e. without typemaps, strings passed from scripting language to
SWIG must be read-only.

adder.c: greeting_sr()
======================


.. code-block:: c

    #define GREETING "Hello, "
    int
    greeting_sr(char * name, char * out, int len) {
        if (len < (strlen(GREETING) + strlen(name) + 1)) {
            out[0] = 0;
            return 1;
        }
        strcpy(out, GREETING);
        strcat(out, name);
        return 0;
    }

adder.h: greeting_sr()
======================

adder.i: greeting_sr()
======================

cy_adder.pxd: greeting_sr()
===========================

cy_adder.pyx: greeting_sr()
===========================

TODO: It looks like in too much of greeting_sr is re-implemented
in Cython.



Gross Generalization, SWIG
==========================

.. class:: handout

    SWIG is...

    Wrapping C libraries for Python and Ruby and Perl and PHP and
    Scheme and ... ...and noticing that there is a lot in common and
    maybe that can be wrapped up and automated.

    If I take something like a header file, with a few hints, I could
    automatically create these wrappers.

Gross Generalization, Cython
============================

.. class:: handout

    Cython is...

    If I take something like a header, I can, line-by-line, reference
    objects as C or Python objects, and convert between them.

    Sort of like in-line assembly.

SWIG, good stuff
================

If you write C library code, and you want to provide bindings (wrappers)
for Python, Java, and Ruby, SWIG can do that.

SWIG, bad
=========

not DRY
learning curve of typemaps

Cython Advantages and Disadvantages (1 minute)
==============================================

    Evolve the interface.
    Do performance optimizations "just in time"
    Cython headers



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

.. class:: handout

    There are a lot of details in doing this kind of work.

    Can you paint yourself into a corner?

    Fear: At the beginning of a project, when you're deciding what tools
    you're going to invest your time in, there's a concern that you will
    pick a tool that gets you 90% of the way there.

    Neither of these tools will do that, I think?

Alternatives to Cython and SWIG (2 minutes)
===========================================

writing extensions with the Python C/API

    http://docs.python.org/2/extending/

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

code and slides
    https://github.com/mkohler/swig_cython

restructedText to LibreOffice Impress
    https://github.com/mattharrison/rst2odp.git

Writing Shared Libraries by Ulrich Drepper

.. class:: handout

    The code and slides are available on github.

    Let me thank Matt Harrison for his
    restructed-text-to-libreoffice-impress tool. I am loving
    restructuredText, and his tool let me create this presentation with
    restructuredText and the comfort of my favorite editor.

End
===

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

Other Bits I Did Not Mention
============================

SWIG stands for Simplified Wrapper and Interface Generator

Neither Cython nor SWIG require source code. A binary libary and a
header file are sufficient.

SWIG and Cython support Python 3.

SWIG is not just for Python. It will create C wrappers for a dozen languages.

Cython has preliminary support for PyPy's C extension API. SWIG does not.

Topics to Considerg Adding
==========================

SWIG typemaps

DRY and maintainability. How much of the header do you have copy?

Performance comparison

Safety comparison

UTF strings
    To a first approximation, ANSI C doesn't do Unicode...so

Show what happens when you do the wrong thing.
    i.e. send the wrong kind of parameters into a C function

Is SWIG a language?

    Not really. The SWIG interface file is a way of marking up a C
    header file to do some common conversions.

Limits of SWIG
==============

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
