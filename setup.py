from distutils.core import setup, Extension

module = Extension(
  'sfst',
  sources = [ 
  'ext/fst.cpp',
  'ext/operators.cpp',
  'ext/determinise.cpp',
  'ext/hopcroft.cpp',
  'ext/alphabet.cpp',
  'ext/basic.cpp',
  'ext/utf8.cpp',
  'ext/pybinding.cpp'
  ],
  include_dirs = ['./'],
  extra_compile_args=['-std=c++11']
  )

setup (
  name = 'sfst',
  version = '0.01',
  license = 'GPLv2',
  description = 'SFST Python Wrapper',
  ext_modules = [module]
)