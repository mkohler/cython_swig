from setuptools import setup
from setuptools.extension import Extension

adder_module = Extension('_adder',
                         sources=['adder.i'],
                         include_dirs=['../libadder'],
                         libraries=['adder'],
                         library_dirs=['../install/usr/local/lib'])

setup(name='swig_adder',
      version='0.1',
      ext_modules=[adder_module])
