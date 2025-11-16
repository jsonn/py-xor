# -*- coding: utf-8 -*-

from distutils.core import setup
from distutils.extension import Extension

setup(
    name='xor',
    description='efficient xor support for bytes-like object',
    version='0.1',
    author='Jörg Sonnenberger',
    author_email='joerg@bec.de',
    license='MIT',
    ext_modules=[Extension('xor', ['xor.c'])],
)
