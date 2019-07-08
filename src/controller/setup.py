from distutils.core import setup, Extension

gomoku_module = Extension('GomokuModule', sources=['Gomoku.cpp', 'Player.cpp', 'Minmax.cpp'], language='C++', extra_compile_args=['-std=c++14', '-g'])
# player_module = Extension('PlayerModule', sources=['Player.cpp'], language='C++')

setup(name='GomokuModule', version='2.1.8', description='Awesome', ext_modules=[gomoku_module],)
# setup(name='PlayerModule', version='2.1.8', description='Awesome', ext_modules=[player_module],)

