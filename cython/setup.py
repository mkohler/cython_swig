from setuptools import setup
from setuptools.extension import Extension
from Cython.Distutils import build_ext


adder_module = Extension('cy_adder', ['cy_adder.pyx'],
                         include_dirs=['../libadder'],
                         libraries=['adder'],
                         library_dirs=['../install/usr/local/lib'])

setup(name='cy_adder',
      version='0.1',
      cmdclass={'build_ext': build_ext},
      ext_modules=[adder_module])
