.. include:: <s5defs.txt>

=======================
Cython vs. SWIG, Fight!
=======================

:Author: Mark Kohler
:Date: 2013-03-16

Using C libraries in Python

.. class:: handout

    Hello, welcome to Cython vs SWIG, Fight!.

    This talk is about what happens when you realize that not
    _everything_ is in the standard library. In fact, there may be
    things you want that you can't even get from the _cheese_ shop.

    If what you want is to use a C library from Python, then you're in
    luck, because _Cython_ and _SWIG_ are excellent tools for building
    Python extension modules by wrapping C libraries.

Pre-fight
=========

- import statement
- libadder library
- passing ints
- passing structs
- C strings
- memory management
- fear and magic
- generalizations

.. class:: handout

    To start this talk, I'm going to to quickly discuss the Python
    import statement and what it hides.

    Then, I'll review the structure of C libraries. When I say "C
    libraries", I mean libraries written in C, designed to be used by
    programs written in C.

    From there, I'll show you a very small C library, the LIBADDER
    library.

    The main part of this talk will consist of adding functions to this
    library, and showing how to make those functions accessible from
    Python. As we do this, you can decide whether you like the SWIG way
    or the Cython way better, and who should win the fight?

    At the end, I'll talk about fear and magic, make some gross
    generalizations, and take questions.

Code first and ask questions later
==================================

#. C code
#. SWIG code
#. SWIG demonstration
#. Cython code
#. Cython demonstration

.. class:: handout

    I want to show a lot of code in this talk, and in an attempt to keep
    you, and especially me, from getting lost, I will show the source
    files for each new feature in the same order, and the name of the
    file I am showing will always be in the title of the slide.

    First, I will show you the C code, then the SWIG code, and then an
    example of using the SWIG-built extension module. Next, I will show the
    Cython code and finally, what it looks like to use the Cython-built
    extension module.

    As we add features, I'll repeat the process: C code, SWIG code,
    SWIG demonstration, Cython code, Cython demonstration.


You are Here
============

- **import statement**
- libadder library
- passing ints
- passing structs
- C strings
- memory management
- fear and magic
- generalizations

.. class:: handout

    Let's get started with the import statement.

import this
===========

>>> import socket
>>> import datetime
>>> import time

- What is socket.__file__?
- What is datetime.__file__?
- What is time.__file__?

.. class:: handout

    First, I have a question. What does import *import*?

    Each of the import statements on the slide imports a module. But
    what exactly is being imported? Or to put it another way, after the
    import, what does the module object's dunder file object contain?

    This is a bit of a trick question, because the answer depends on the
    platform, but I'll give you a hint, there are 3 possibilities.

import socket
=============


.. code-block:: python

    >>> socket.__file__
    '/usr/lib/python2.7/socket.pyc'
    >>>

.. class:: handout

    The first case is the most straight-forward. The import statement
    caused the Python interpreter to load a .pyc file into memory. I'm
    sure you've seen PYC files before. They contain architecture-neutral
    python byte-codes. This is _pure_ python code.

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

    And this is the case we care about. The import statement caused a
    Python extension module to be loaded into memory. The ".so" stands
    for Shared Object, and it is architecture-specific machine code. In
    this example, the shared object uses the x86-64 instruction set.
    Nevertheless, when you import it, it looks and feels like Python
    code.

    This is a killer feature! Applications can import modules containing
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

    There's one more case, and it isn't relevant to SWIG or Cython, but
    I thought I should mention it anyway, as it confused the hell out of
    me the first time I went to look for a Python module on disk, and I
    couldn't find a corresponding .py or .pyc or .so file.

    So, where did this module come from?

    The answer to this riddle is that some Python extension modules are
    linked with the Python interpreter when it is built. And those
    module don't have dunder file attributes.

    You'll have to go find or re-create the build environment for the
    Python interpreter if you want to find the source for this module.

You are Here
============

- import statement
- **libadder library**
- passing ints
- passing structs
- C strings
- memory management
- fear and magic
- generalizations

.. class:: handout

    Now, let's talk about our example C library, libadder.

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
    than Monty Python references. Thus, __adder__

    The slide shows the build process for libadder. The names in bold
    are human-generated files. The others are machine-generated. (pause)

    adder.c is the source code. We will look at it next.

    adder.h describes the interface to _adder.c_

    and libadder.so is the final shared object, ready to be linked with
    C programs and other shared libraries.

    Now let's look at some code.

adder.c: add()
==============

.. code-block:: c

    int
    add(int x, int y) {
        return x + y;
    }

.. class:: handout

    This is a C function which adds to two integers, and returns their
    sum. Unremarkable...but we'd like to use it from Python.

adder.h: add()
==============

.. code-block:: c

    int add(int, int);

.. class:: handout

    In C, the interface to a function is typically declared in a separate file,
    a *header* file. Here is the header file for our libadder.

    It has three parts:
        the *name* of the function,
        the *types* of the parameters,
        and the *type* of the return value.

    As you may notice, it consists entirely of information that is
    also in adder.c.

    This kind of repetition is something to watch out for, because
    keeping the same information in two places makes for troublesome
    code maintenance...and we're going to see a lot more of that kind of
    repetition in the SWIG and Cython workflows.

    Now that we have covered the C build process, let's look at SWIG.

You are Here
============

- import statement
- libadder library
- **passing ints**
- passing structs
- C strings
- memory management
- fear and magic
- generalizations

.. class:: handout

    Now, let's talk about our example C library, libadder.


adder.i (SWIG interface file)
=============================

.. code-block:: c

    %module adder
    %{
    #include "adder.h"
    %}

    int add(int, int);

.. class:: handout

    To use SWIG, you, the programmer, have to create _one_ file, the
    SWIG interface, or .i file. _This_ is adder.i, the SWIG interface
    file I created for libadder.

    If you look at the last line a bit, you may think, "Hey, I've seen
    this before" and yes, it's identical to the C header file.

    So we tell SWIG the name of the extension module we want to build,
    the name of the C header file, AND copy-and-paste the contents of
    the C header file.

    Now, what do you do with this file? Let's look at the SWIG build
    diagram.

SWIG build diagram
==================

0. Start with: **adder.h**, libadder.so

1. SWIG

    **adder.h + adder.i** --> adder_wrap.c + adder.py

2. Compile

    **adder.h** + adder-wrap.c --> adder_wrap.o

3. Link

    libadder.so + adder_wrap.o --> _adder.so

.. class:: handout

    The SWIG tool takes the C header file, and the SWIG interface file,
    and it generates two files:

       the _wrap.c file and a Python file. These files work together to
       to convert between Python objects and C interfaces.

    Then you compile the SWIG-generated C file, link it to the shared
    object you started with, and PRESTO, you have your Python extension
    module.

demo of SWIG's add()
====================

.. code-block:: text

    >>> import adder
    >>> adder.add(2, 3)
    5
    >>>

.. class:: handout

    And here's how it looks when you use it. You import the python file
    that SWIG generated, and THAT imports the _adder.so object, and you
    now have access to your C library. Magic!

    Now let's do the same thing, but with Cython.

c_adder.pxd: Cython interface file
==================================

.. code-block:: cython

    cdef extern from "adder.h":
        int add(int x, int y)

.. class:: handout

    ...And we start with an interface file. In Cython-land, the
    interface file is a dot-PXD file.

    And...it sure looks a lot like the SWIG interface file. The
    boilerplate is different, but it references a C header file, and
    it is ALMOST a copy-and-paste of the C header file. (You have to
    remove the semi-colon.)

cy_adder.pyx:  Cython source file
=================================

.. code-block:: cython

    cimport c_adder

    def add(x, y):
        return c_adder.add(x, y)

.. class:: handout

    Now, this is new. A PYX file. This file does not have an analog in
    the SWIG workflow. Let's squint a little, pretend this is Python,
    and see if we can make any sense of it.

    Yes, except for that cimport line, which looks a lot like a regular
    Python import, this file could be Python code. The big difference is
    what we do next. Feed it to Cython, to be translated to C.

    Well that wasn't too bad. And we can build a Python extension module
    from this? Let's do it.

Cython build diagram
====================

1. Cython

    **adder.h + c_adder.pxd + cy_adder.pyx** --> cy_adder.c

2. Compile

   **adder.h** + cy_adder.c --> cy_adder.o

3. Link

   lib_adder.so + cy_adder.o --> cy_adder.so

.. class:: handout

    Cython takes our C header file, our interface file, and this third
    file we just learned about, the PYX file, and generates a single C
    file, cy_adder.c in this case.

    Compile and link that, and you get an so. file that you can import
    at the Python prompt. Let's see it.

demo of Cython's add()
======================

.. code-block:: text

    >>> import cy_adder
    >>> cy_adder.add(2, 3)
    5
    >>>

.. class:: handout

    Import the shared object we just built, and boom, you are running
    C code from Python. Great!

    How did that happen again?

    Let's review.

Cython build review
====================

0. Given: adder.h, libadder.so

1. Cython

    **adder.h + c_adder.pxd + cy_adder.pyx** --> cy_adder.c

2. Compile

   **adder.h** + cy_adder.c --> cy_adder.o

3. Link

   libadder.so + cy_adder.o --> cy_adder.so

.. class:: handout

    Like with SWIG, you start with a C header file, and a shared object.

    *You* write a PXD file, which is *Cython's* interface file format.
    You also need to write a PYX file, in some kind of C-Python hybrid
    language.

    From those files, Cython generates a C file. Compile and link
    Cython's C file, and you have a Python extension module that you can
    import and use.

You are Here
============

- libadder library
- passing ints
- **passing structs**
- C strings
- memory management
- fear and magic
- generalizations

.. class:: handout

    Now let's learn how to use a C function that requires you to pass it
    a pointer to a struct.

adder.h: pair_add()
===================

.. code-block:: c

    typedef struct _PAIR {
        int x;
        int y;
    } PAIR;

    int pair_add(PAIR * ppair);

.. class:: handout

    We have an interface 

    That's a very C thing to do.

   How does that translate to Python? 

adder.c: pair_add()
===================

.. code-block:: c

    int
    pair_add(PPAIR ppair) {
        return ppair->x + ppair->y;
    }

.. class:: handout

    So what does we tell SWIG?

adder.i: pair_add()
===================

.. code-block:: c

    typedef struct _PAIR {
        int x;
        int y;
    } PAIR;

    int pair_add(PPAIR);

.. class:: handout

    Again, just like the C header file.

    Now, let's see it in action.

demo of SWIG's pair_add()
=========================

.. code-block:: text

    >>> import adder
    >>> my_pair = adder.PAIR()
    >>> my_pair.x = 3
    >>> my_pair.y = 4
    >>> adder.pair_add(my_pair)
    7
    >>>

.. class:: handout

    Well, that's interesting. Where did that adder.PAIR() come from?

    If you put a struct in the SWIG interface file, SWIG creates an
    associated Python class. The field in the Python object correspond
    to the fields in the struct.

    In this example, we are passing a pointer-to-a-struct to a function,
    but this same mechanism can be used with functions that return a
    pointer to a structure.

    Okay, now let's do it with Cython.

adder.pxd: pair_add()
=====================

.. code-block:: cython

    ctypedef struct PAIR:
        int x
        int y

    int pair_add(PAIR * ppair)

.. class:: handout

    Okay, here we have something that looks just like our C header file,
    but without braces or semi-colons.

adder.pyx: pair_add()
=====================

.. code-block:: cython

    def pair_add(x, y):
        cdef c_adder.PAIR pair
        pair.x = x
        pair.y = y
        return c_adder.pair_add(&pair)

.. class:: handout

    What is this? This looks like Python but that looks more like C.

    We're declaring a type.

    We're assigning these untype things to a typed thing.

    Whoa, there's an ampersand in my Python.

    Takes a Python object and returns a Python object.

    This is where we start to get the real flavor of Cython, where on a
    line-by-line basis, or even within a line, you can switch between Python and C.

    "pair.x = x"

    If that's not magic I don't know what is.

demo of Cython's pair_add()
===========================

.. code-block:: text

    >>> import cy_adder
    >>> cy_adder.pair_add(3, 4)
    7
    >>>

.. class:: handout


You are Here
============

- import statement
- libadder library
- passing ints
- passing structs
- **C strings**
- memory management
- fear and magic
- generalizations

adder.h: get_version()
======================

.. code-block:: c

    char * get_version(void);

.. class:: handout

All good libraries need to report their version. So we'll make a function that
returns it's version as a C string.

adder.c: get_version()
======================

.. code-block:: c

    static char version[] = "v1.0";

    char *
    get_version(void) {
        return version;
    }

.. class:: handout

    Here's the implementation.

adder.i: get_version()
=============================

.. code-block:: c

    char * get_version(void);

.. class:: handout

    We add this line to our SWIG interface file. Again this is a copy and paste
    of the C header file.

adder.pxd: get_version()
========================

.. code-block:: c

    char * get_version()

.. class:: handout

    And over in Cython land, with more modernist sensibilities, we don't need
    the void or the semi-colon.

Using SWIG's get_version
========================

.. code-block:: text

    >>> import cy_adder
    >>> cy_adder.pair_add(3, 4)
    7
    >>>

.. class:: handout

Using Cython's get_version
==========================

.. code-block:: python

    print cy_adder.get_version()

.. class:: handout

Cython and C Strings
====================

"...avoid using C strings where possible..."

    - Cython documentation, General Notes about C strings

.. class:: handout

    C strings and python strings are not the same thing.
     What does Cython say about C strings?

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

You are Here
============

- import statement
- libadder library
- passing ints
- passing structs
- C strings
- **memory management**
- fear and magic
- generalizations

SWIG and Memory Management
==========================

By default, i.e. without typemaps, strings passed from scripting language to
SWIG must be read-only.

.. class:: handout

    Memory management

    This is where things get real.

adder.h: greeting_sr()
======================

.. code-block:: c

    int greeting_sr(char * name, char * output, int buflen);

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

adder.i: greeting_sr()
======================

c_adder.pxd: greeting_sr()
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

Cython Advantages and Disadvantages
===================================

Advantages
----------

Evolve an interface.

Do performance optimizations "just in time".

Provides a interface layer where you can smoothely slide between C and Python.

Disadvantages
-------------

You have to create and maintain Cython .pxd files for your library.

.. class:: handout

Magic
=====

.. class:: handout

    I've been a C programmer for a long time, and when I first saw how easy it
    was to play with the sockets library, to play with, interactively, from the
    Python prompt, I was amazed.

    It was so different from the world I lived in, and compiling, and linking
    and just plain (pause) waiting for the build to finish.

    These tools give me the same feeling.
    The python way seemed like magic to me.

    Then later, when I saw

Show generated code from SWIG and Cython.

When you look under the covers, it is *still* magic.

It's not like I could re-create SWIG or Cython.

Which brings me to fear.

Fear
====

.. class:: handout

    If you don't understand a tool, it is easy to be scared of it.

    My concern is that I would choose a tool, and get 90% of the way there, and
      then find a routine that I couldn't properly wrap, and I would be stuck.

    I would have trouble incorporating it into the build.

    Once incorporated, the tool would be too hard to understand or debug.

    The tool would take too long to get started.

    There are a lot of details in doing this kind of work.

    Can you paint yourself into a corner?

    Fear: At the beginning of a project, when you're deciding what tools
    you're going to invest your time in, there's a concern that you will
    pick a tool that gets you 90% of the way there.

    Neither of these tools will do that, I think?

    With SWIG, if you can't make an interface work, write a C
    program that uses the interface, and wrap that.

    My point is you don't have to be an

Alternatives to Cython and SWIG
===============================

writing extensions with the Python C/API

    http://docs.python.org/2/extending/

ctypes

.. class:: handout

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

Topics to Consider Adding
=========================

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


No source?
==========

.. class:: handout

    For this discussion, we're going to assume the library is already
    written. Maybe you wrote it, or maybe it's a third-party library,
    and all you have is a header file and a binary, but either way,
    we're going to assume we don't want to change the library's interface.

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

    This is the red pill. If you take it, you leave the world where C
    and Python are separate langauges, and enter a world where C code
    and Python code can be mixed, within a file, even line-by-line
    within a function.

    Take your C header file and (manually) create a .pxd file::
        Copy the file
        Remove semi-colons.
        Convert #defines to variables.
        ints to bints

    Create a .pyx file.
        This is where you are really using the Cython language. It can
        be repetitive, but you also have tons of flexibility in making a
        Pythonic interface.
