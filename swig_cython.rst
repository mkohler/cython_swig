Cython vs. SWIG, Fight!

Cython and SWIG are excellent, and yet very different, tools for using C libraries from Python. The goal of this talk is to introduce both tools, discuss their strengths, their weaknesses, and the situations that clearly favor one tool over the other.



Introduction (2 minutes)
========================

What is SWIG? (2 minutes)
=========================

    What goes in?
    What comes out?
    Is SWIG a language?

An Aside: Anatomy of C Libraries (3 minutes)
============================================

    header files
    function parameters
    return values
    structs, arrays and pointers

Alternatives to Cython and SWIG (2 minutes)
===========================================

    writing extensions with the Python C/API
    ctypes

SWIG Code Walkthrough (5 minutes)
=================================

Typemaps (2 minutes)
====================

What is Cython? (2 minutes)
===========================

    What goes in?
    What comes out?

Cython, the Language (2 minutes)
================================

Cython: How is that possible? (2 minutes)
=========================================

    build process
    toolchain diagram

Cython Code Walkthrough (5 minutes)
===================================

SWIG Advantages and Disadvantages(1 minute)
===========================================

    create extensions for other languages
    autotools support
    learning curve of typemaps

Cython Advantages and Disadvantages (1 minute)
==============================================

    Evolve the interface.
    Do performance optimizations "just in time"
    Cython headers

Chart of Use Cases and Tool Recommendation (1 minute)
=====================================================
