.. include:: <s5defs.txt>

=======================
Cython vs. SWIG, Fight!
=======================

:Author: Mark Kohler
:Date: 2013-03-16

Wrapping C libraries for Python

.. class:: handout

    Hello, welcome to Cython vs SWIG, Fight!.

    This talk is about what happens when you realize that not
    _everything_ is in the standard library. In fact, there may be
    things YOU want that you can't even get from the CHEESE shop.

    If what you want, is to use a C library from Python, then you're in
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
- generalizations

.. class:: handout

    XXX Why are they fighting?

    To start this talk, I'm going to quickly discuss the Python import
    statement and what it hides.

    From there, I'll show you a VERY small C library, the LIBADDER
    library.

    The main part of this talk will consist of adding functions to this
    library, and showing how to make those functions accessible from
    Python. As we do this, you can decide whether you like the SWIG way
    or the Cython way better, and WHO should win the fight.

    At the end, I'll make some gross generalizations, and take
    questions.

Code first and ask questions later
==================================

#. C code
#. SWIG code
#. SWIG demonstration
#. Cython code
#. Cython demonstration

.. class:: handout

    I want to show a lot of code in this talk, and in an attempt to keep
    you, and especially me, from getting lost, I will always show the
    source files in the same order, and the NAME of the file will always
    be in the title of the slide.

    First, I will show you the C code, then the SWIG interface file, and
    then a transcript of USING the SWIG-built extension module. Next, I
    will show the Cython code and finally, a transcript of using the
    Cython-built extension module.

    As we add features, I'll repeat the process: C code, SWIG code,
    SWIG demonstration, Cython code, Cython demonstration.


We are Here
============

- **import statement**
- libadder library
- passing ints
- passing structs
- C strings
- memory management
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
    import, what does the module object's dunder file object point to?

    This is a bit of a trick question, because the answer depends on the
    platform, but I'll give you a hint, there are 3 possibilities.

import socket
=============

.. code-block:: python

    >>> socket.__file__
    '/usr/lib/python2.7/socket.pyc'
    >>>

.. class:: handout

    The first possibility is the most straight-forward. The import
    statement caused the Python interpreter to load a .pyc file into
    memory. I'm sure you've seen PYC files before. They contain
    python byte-code.

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

    The second possibility is the one we care about today. The import
    statement caused a Python extension module to be loaded into memory.
    The ".so" stands for Shared Object, and it is machine code.
    Nevertheless, when we import it, it looks and feels like Python
    code.

    This is a killer feature! Applications can import modules containing
    either Python code, or compiled machine code, and the interface is
    exactly the same.

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

    There's one more possibility, and it isn't relevant to SWIG or
    Cython, but I think I should mention it anyway, as it confused me
    the first time I went to look for a Python module on disk, and I
    couldn't find a corresponding .py or .pyc or .so file.

    So, where DID this module get imported from?

    The answer to this riddle is that some Python extension modules are
    linked with the Python interpreter when IT is built. If a module is
    linked to the interpreter binary, then it WON'T have a dunder file
    attribute.

    You'll have to create a build environment for the Python interpreter
    to find the source for a module like this.

We are Here
============

- import statement
- **libadder library**
- passing ints
- passing structs
- C strings
- memory management
- generalizations

.. class:: handout

    Now, let's talk about our example C library, libadder.

adder.c: add()
==============

.. code-block:: c

    int
    add(int x, int y) {
        return x + y;
    }

.. class:: handout

    ADD is a C function which adds two integers, and returns their
    sum. Unremarkable...but we'd like to use it from Python.

adder.h: add()
==============

.. code-block:: c

    int add(int, int);

.. class:: handout

    For C libraries, the interface to a function is declared in a
    separate file, a HEADER file.

    As you may notice, it consists entirely of information that is
    also in adder.c.

    This kind of repetition is something to watch out for, because
    keeping the same information in two places makes for troublesome
    code maintenance...and we're going to see a lot more of that kind of
    repetition in the SWIG and Cython workflows.


See libraries
=============

1. Compile

    **adder.c + adder.h** --> adder.o

2. Link

    adder.o --> libadder.so

.. class:: handout

    This slide shows the build process for libadder. The names in bold
    are the human-generated files. (pause)

    LIBADDER.SO is the final shared object, ready to be linked with
    C programs and other shared libraries.

    Now let's look at some code.

We are Here
============

- import statement
- libadder library
- **passing ints**
- passing structs
- C strings
- memory management
- generalizations

.. class:: handout

    Now, we will use SWIG, and then Cython, to create Python extension modules
    that wrap libadder's add function.

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

    The first line is the name of the extension module we want to build,
    the third line is the name of the C header file, AND the last line
    is the contents of the C header file.

    Now, what do we do with this file? Let's look at the SWIG build
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
    and it generates two files: the _wrap.c file and a Python file.
    These files work together to convert between Python objects and C
    interfaces.

    Then we compile the SWIG-generated C file, link it to the shared
    object we started with, and PRESTO, we have our Python extension
    module.

demo of SWIG's add()
====================

.. code-block:: text

    >>> import adder
    >>> adder.add(2, 3)
    5
    >>>

.. class:: handout

    And here's how it looks when we use it. We import the python file
    that SWIG generated, and THAT imports the _adder.so object, and we
    now have access to we C library. Magic!

    Now let's do the same thing, but with Cython.

c_adder.pxd: Cython interface file
==================================

.. code-block:: cython

    cdef extern from "adder.h":
        int add(int x, int y)

.. class:: handout

    Like with SWIG, we start by creating an interface file. In
    Cython-land, the interface file is a dot-PXD file.

    And...it sure looks a lot like the SWIG interface file. The
    boilerplate is different, but it references a C header file, and
    it is ALMOST a copy-and-paste of the C header file. (We have to
    remove the semi-colon.)


cy_adder.pyx:  Cython source file
=================================

.. code-block:: cython

    cimport c_adder

    def add(x, y):
        return c_adder.add(x, y)

.. class:: handout

    So, SWIG and Cython both require us to create an interface file,
    but in Cython, we also need to create a PYX file.

    I'm not going to explain the syntax of this file yet. Let's squint a
    little, pretend the file is Python code, and see if we can make any
    sense of it.

    Yes, except for that cimport line, which looks a lot like a regular
    Python import, this is Python code. The big difference is what we do
    with it. The next step is to to feed it to Cython, to be translated to C.

    Let's do it!

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

    Compile and link that, and we get an so. file that we can import
    at the Python prompt. Let's try it out.

demo of Cython's add()
======================

.. code-block:: text

    >>> import cy_adder
    >>> cy_adder.add(2, 3)
    5
    >>>

.. class:: handout

    Import the shared object we just built, and boom, we are running
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

    Like with SWIG, we start with a C header file, and a shared object.

    WE write a PXD file, which is CYTHON's interface file format.
    We also need to write a PYX file, which we will soon see is a hybrid
    of the C and Python languages.

    From those files, Cython generates a C file. Compile and link
    Cython's C file, and we have a Python extension module that we can
    import and use.

We are Here
============

- libadder library
- passing ints
- **passing structs**
- C strings
- memory management
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

    Here's a header file with a struct, PAIR, defined,

    Below the struct is the declaration for pair_add, which takes a
    single argument, a pointer to a PAIR.

adder.c: pair_add()
===================

.. code-block:: c

    int
    pair_add(PAIR * ppair) {
        return ppair->x + ppair->y;
    }

.. class:: handout

    pair_add returns the sum of the integers in the struct pointed to by
    ppair.

    That's pretty straightfoward C code. But how can we use pair_add from
    Python? What do we tell SWIG?

adder.i: pair_add()
===================

.. code-block:: c

    typedef struct _PAIR {
        int x;
        int y;
    } PAIR;

    int pair_add(PAIR * ppair);

.. class:: handout

    Here's what we add to our SWIG interface file for pair_add. We just
    copy-and-paste the relevant part of the C header file.

    Remembering the SWIG build diagram, we use this to build the adder
    extension module. Let's see how we use it.

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

    Well, that's interesting. Where did that adder.PAIR() come from? I
    don't remember defining that.

    The answer is, SWIG did it. If you put a struct in a SWIG interface
    file, SWIG creates an associated Python class. The fields in the
    Python object corresponds to the fields in the struct.

    In this example, we are passing a pointer-to-a-struct to a function,
    but this same mechanism can be used with functions that return a
    pointer to a structure.

    Okay, now let's do it with Cython.

c_adder.pxd: pair_add()
=======================

.. code-block:: cython

    ctypedef struct PAIR:
        int x
        int y

    int pair_add(PAIR * ppair)

.. class:: handout

    Again, the Cython interface file contains the same information as
    the C header file. We just need to replace typedef with ctypedef,
    and remove the braces and semi-colons.

cy_adder.pyx: pair_add()
========================

.. code-block:: cython

    def pair_add(x, y):
        cdef c_adder.PAIR my_pair
        my_pair.x = x
        my_pair.y = y
        return c_adder.pair_add(&my_pair)

.. class:: handout

    (5s pause) This is where we start to see the true nature of Cython,
    where on a line-by-line basis, or even within a line, we can switch
    between Python and C. I'll explain this function line-by-line.

    Line 1 is a normal Python function definition.

    In Line 2, we define a C variable, my_pair of type PAIR.

    In lines 3-4, the Python objects x and y, are unwrapped, and their
    values are copied to the x and y fields with the my_pair struct.

    Finally, on the last line, now that we have a C struct all ready, we
    can call pair add, passing it a pointer to a C struct. And yes, the
    ampersand on the last line, works as the "address-of" operator, just
    like in C.

    When the function returns an int, Cython will wrap it up in a Python
    object, to be returned to the calling Python function.

    Let me let that sink in a bit. The first time I saw something like
    this it took me a while to believe it. (PAUSE)

    ...Okay, let's see it in action.

demo of Cython's pair_add()
===========================

.. code-block:: text

    >>> import cy_adder
    >>> cy_adder.pair_add(3, 4)
    7
    >>>

.. class:: handout

    And it works. Once it is compiled and linked, the Cython code you just saw
    can take arguments from the Python interpreter, allocate a struct, fill it
    in, and pass it to the pair_add function, which has no idea the struct came
    from a Python program.

    Let's keep going. We need to see some of the complexity behind the magic.

We are Here
============

- import statement
- libadder library
- passing ints
- passing structs
- **C strings**
- memory management
- generalizations

.. class:: handout

    And a good way to dive right into a sea of complexity is to start thinking
    about Python and C strings.

    Yes...C Strings, the source of all good buffer overflows.

    Compared to Python strings, C strings are so amazingly primitive,
    it's hard to evan call them strings. They are fixed-size, mutable,
    arrays of bytes. Nevertheless, there are many C libraries that use C
    strings as part of their interface. And we need to understand how to
    handle them from Python.

    We'll start with a simple case.

adder.h: get_version()
======================

.. code-block:: c

    char * get_version(void);

.. class:: handout

    All good libraries need to report their version. So we'll make a
    function that returns its version as a C string.

adder.c: get_version()
======================

.. code-block:: c

    static char version[] = "v1.0";

    char *
    get_version(void) {
        return version;
    }

.. class:: handout

    Here's the implementation. The important thing to notice is that the string
    itself is stored as a static variable.

    In this way,  we can bypass the memory management of C strings. We'll get
    to that soon enough.

adder.i: get_version()
======================

.. code-block:: c

    char * get_version(void);

.. class:: handout

    We add this line to our SWIG interface file. Again this is a copy and paste
    of the C header file.

demo of SWIG's get_version()
============================

.. code-block:: text

    >>> import adder
    >>> adder.get_version()
    'v1.0'
    >>> _.__class__
    <type 'str'>
    >>>

.. class:: handout

    And here is get_version in use.

    Conveniently, SWIG automatically converts the C string to a Python string,
    a real Python string. 

    So that seems to work pretty well. Let's try it with Cython.

c_adder.pxd: get_version()
==========================

.. code-block:: c

    char * get_version()

.. class:: handout

    We start with a Cython interface file. Here in Cython land, with its
    modernist sensibilities, we don't need the void or the semi-colon.
    Otherwise, it is just a copy and paste of the C header file.

cy_adder.pyx: get_version()
===========================

.. code-block:: python

    def get_version():
        return c_adder.get_version()

.. class:: handout

    And our addition to the Cython source file is a trivial pass-thru function.

demo of Cython's get_version()
==============================

.. code-block:: text

    >>> import cy_adder
    >>> cy_adder.get_version()
    'v1.0'
    >>> _.__class__
    <type 'str'>
    >>>

.. class:: handout

    And the Cython version works just like the SWIG version. So that's boring.

Cython and C Strings
====================

"C strings are slow and cumbersome"

"...avoid using C strings where possible"

"...more likely to introduce bugs"

.. class:: handout

    But if converting between C strings and Python strings is so easy, why do I
    findwarnings like this in the Cython documentation?

SWIG and C Strings
==================

"The problems (and perils) of using char * are well-known. However, SWIG
is not in the business of enforcing morality."

    SWIG documentation, Section 8.3 C String Handling

.. class:: handout

    And this in the SWIG documentation.

We are Here
===========

- import statement
- libadder library
- passing ints
- passing structs
- C strings
- **memory management**
- generalizations

.. class:: handout

    The problem is...memory management. Python's is automatic. C's is manual.

    And that may sound academic, until you start thinking about what you like
    to do with strings: split them, combine them, copy them, pass them around
    to your friends.

    And while you're having all that fun, who's keeping track of the memory
    you're using?

    To explore these issues, we'll add a function to libadder that takes a C
    string as a parameter, and produces a C string as output.

adder.h: sgreeting()
======================

.. code-block:: c

    int sgreeting(char * name, char * outp, int buflen);

.. class:: handout

    Here is the declaration for sgreeting, a function that creates a
    greeting. If you pass it "Monty", it produces "Hello, Monty".

    sgreeting works a bit like sprintf. That is, sgreeting produces a
    greeting but it does not return it. It returns the number of
    characters in the output string, or a 0 if there was an error.

    So where does sgreeting put the greeting? The CALLER of sgreeting
    passes a buffer to sgreeting via the pointer OUTP, and sgreeting puts
    the greeting in that buffer.

    This is a common pattern in C libraries. The caller allocates the
    memory, and passes called functions a buffer and length.

adder.c: sgreeting()
======================

.. class:: small

    .. code-block:: c

        static char hello[] = "Hello, ";

        int
        sgreeting(char * name, char * outp, int buflen) {
            if (outp && buflen) {
                if (buflen < (strlen(hello) +
                              strlen(name) + 1)) {
                    outp[0] = 0;
                    return 0;
                }
                strcpy(outp, hello);
                strcat(outp, name);
            }
            return strlen(hello) + strlen(name);
        }

.. class:: handout

    Here's the implementation of sgreeting.

    The first conditional allows callers to find out how much space to
    allocate. The caller passes in a null pointer for the output buffer,
    and sgreeting returns the number of bytes in the output string.

    The second conditional checks if the buffer is long enough for
    the output, returning zero if the buffer is not long enough.

    Then it concatenates the string hello, and its input argument in the
    outp buffer.

    Understanding the implementation isn't vital as long as you
    understand the interface. And the key thing to understand about the
    interface is that the CALLER of sgreeting function must allocate
    the sgreeting's output buffer.

    Let's use SWIG to make this function available to Python.

adder.i: sgreeting()
======================

.. code-block:: c

    %include "cstring.i"
    %cstring_output_maxsize(char * outp, int buflen);

    int sgreeting(char * name, char * outp, int buflen);

.. class:: handout

    Up to this point, we have seen functions that meshed well with
    SWIG's default behavior. That is why our SWIG interface files have
    been copies of the C header file.

    sgreeting is different.

    The input parameter, NAME, isn't a problem. By default, SWIG will
    automatically convert a Python string to a read-only C string, which
    is what we want.

    The problem is OUTP, the output pointer. We need to tell SWIG to
    allocate a buffer for sgreeting, and pass a pointer to the buffer
    in OUTP and the length of the buffer in BUFLEN. Fortunately, SWIG
    CAN do this. That is what the first two lines do.

    The cstring_output_maxsize macro tells SWIG that, the wrapped
    version of sgreeting will pass in a maximum length, and SWIG will
    be responsible for allocating a buffer of that length, and be
    responsible for freeing it. Fancy!

    Let's see how we use the wrapped function.

demo of SWIG's sgreeting()
============================

.. class:: text

    >>> import adder
    >>> adder.sgreeting("Monty", 100)
    [12, 'Hello, Monty']
    >>>

.. class:: handout

    I hope this usage is a little surprising. We started with a
    function, sgreeting, that took 3 arguments, and returned its
    status, but here are we, at the python prompt, using a function that
    takes 2 arguments and returns a list.

    We talked about the change in input parameters in the previous
    slide. SWIG created a Python function for us that takes an input
    string and a maximum length for the output. Then internally, it
    uses that maximum length to allocate a buffer, and passes that
    buffer and its length to the original sgreeting function.

    The behavior we didn't talk about is that, if you tell SWIG that a
    function parameter is actually for OUTPUT, then SWIG will
    de-reference that pointer, put the contents in a list that the
    wrapped function returns.

    So, in our example, the C version of sgreeting returned 12, and
    "Hello, Monty" was grabbed from the OUTP pointer.

c_adder.pxd: sgreeting()
===========================

.. code-block:: python

    int sgreeting(char * name, char * output, int buflen)

.. class:: handout

    Moving on to Cython, here is the Cython interface file for
    sgreeting. As with other examples, it is almost identical to the C
    header file.

cy_adder.pyx: sgreeting()
===========================

.. class:: small

    .. code-block:: c

        def sgreeting(name):
            c_str_len = c_adder.sgreeting(name,
                                            <char * > 0,
                                            0)
            py_str = ' ' * (c_str_len + 1)
            cdef char * c_str = py_str
            c_adder.sgreeting(name,
                                c_str,
                                len(py_str))
            return c_str

.. class:: handout

    And here's the Cython code. The first thing to notice is how long it
    is. The reason for that is that we are doing memory allocation for
    the C function we're going to call.

    Going line-by-line, we call sgreeting with the bona fide name, but
    a null pointer. This lets us find out how long the greeting WILL be,
    and thus how much memory we need to allocate.

    In the next two lines, we create a Python string that is long enough
    to hold the greeting, and then create a C pointer from it. When you
    assign a Python string to a C string, the C pointer points to the
    buffer of the Python string itself. In this way, we can piggy-back
    on Python's memory management, in that we have a C buffer that will
    be freed when the variable PY_STR is freed.

    Now that the hard part is done, we can call our C function, and get
    our greeting placed in C_STR. Finally, the last line is in implicit
    conversion from c_str to a Python string.

demo of Cython's sgreeting()
==============================

.. code-block:: text

    >>> import cy_adder
    >>> cy_adder.sgreeting("Monty")
    'Hello, Monty'
    >>>

.. class:: handout

    And here we see the benefit of writing all of that code in the
    Cython PYX file. Our wrapped function has a perfectly Pythonic
    interface, with no indication of all the manual memory-management
    machinations that we were just mulling.

We are Here
===========

- import statement
- libadder library
- passing ints
- passing structs
- C strings
- memory management
- **generalizations**

.. class:: handout

    Whew. We've looked at the details of how you use SWIG and Cython,
    and you've probably formed an opinion about which one you would
    prefer to work with.

    Now let's take a step back from the nitty gritty, and look at some
    of their high-level differences.

SWIG Features
=============

.. class:: incremental

    - Multi-language support
    - More DRY than Cython

.. class:: handout

    CLICK. SWIG will wrap C libraries for many languages besides
    Python. So if you are the author of a C library, and you want to
    provide bindings for Python AND Java AND Ruby, SWIG is THE tool.

    CLICK. DRY means Don't Repeat Yourself, and both SWIG and Cython have a
    common weakness in that they require the programmer to maintain
    interface files that are near copies of C header files. In
    addition, as we saw, Cython requires you to create your own wrapper
    functions in a PXD files. That is, (pause), not ideal.

    Nevertheless, SWIG has the best story here. If your C library has a
    SWIG-friendly interface, it is POSSIBLE to use the C header files
    AS the SWIG interface file. Even if you can only do this for 95% of
    your library, it can be a huge code maintenance win.

Cython Advantages
=================

- It's Python **and** it's C

- explore performance trade-offs between C and Python

.. class:: handout

    Cython's primary use is not wrapping C libraries. It is improving
    the performance of Python code. And the pitch is awesome. Cython IS
    Python, with optional, static, types. The more types you add, the
    faster it goes.

    Unlike SWIG, which has its own pattern matching language for glueing
    together C and Python, Cython is C AND Python, in one file. You just
    need to get used to the idea, (pause) and change your build process.

    Allowing one to switch between C and Python, line-by-line, is a
    killer feature, as it lets one smoothely slide between C and Python,
    as external interfaces or performance considerations dictate.

Alternatives to Cython and SWIG
===============================

.. class:: incremental

- Python C/API

    http://docs.python.org/2/extending/

- ctypes

.. class:: handout

    After all this, if you want to run screaming away from SWIG and
    Cython, but you still need to use C libraries from Python, you have
    two choices, and they are both, in a sense, extreme choices compared
    to SWIG and Cython.

    (CLICK) You can use the Python C API, just like many standard library
    extension modules.

    (CLICK) Or you can use ctypes, which is part of the Standard Library, and
    lets you access C libraries with a lot less CEREMONY, but also a lot
    less SAFETY, than SWIG and Cython. If you want to access a C library
    NOW, without building anything, without source code, and without
    even a header file, take a look at ctypes.

Getting Started
===============

.. class:: incremental

    - Start small.
    - Use distutils.

.. class:: handout

    Adoping a powerful tool like SWIG or Cython, can be scary. There is
    a lot to learn, and at the beginning it can be hard to see whether
    it will do what you need.

    (CLICK) Start small.

    Like with any other code, work incrementally. Neither SWIG nor
    Cython require you to wrap an entire library. Wrap one function at a
    time. 

    (CLICK) And use DISTUTILS to build your Python extension. Even if
    you don't use it for anything else. It already has all of the magic
    compiler flags needed for building extensions that 

    Did anyone knows that distutils includes code to parse Makefiles?
    It's in sysconfig.py, and it actually parses the Makefile that builds the
    Python interpreter to find the flags that were used to build the
    interpreter. Someone went to the trouble of writing that code. Use
    it.

Code and Slides
===============

https://github.com/mkohler/cython_swig

mark.kohler@gmail.com

.. class:: handout

    Thank you. I hope you found it useful.
    The code and slides are available on github.
    I am Mark Kohler.
    And I am ready to take questions.
