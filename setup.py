from glob import glob
from setuptools import setup
from pybind11.setup_helpers import Pybind11Extension, build_ext

ext_modules = [
    Pybind11Extension(
        "h5gtpy",
        sorted(glob("src/h5gtpy/*.cpp")),  # Sort source files for reproducibility
    ),
    Pybind11Extension()
]

classifiers = [
    'Development Status :: 5 - Production/Stable',
    'Intended Audience :: Developers',
    'Intended Audience :: Science/Research',
    'License :: OSI Approved :: BSL',
    'Operating System :: OS Independent',
    'Programming Language :: Python :: 3',
    'Programming Language :: C',
    'Programming Language :: C++',
    'Topic :: Scientific/Engineering',
    'Topic :: Database',
    'Topic :: Software Development :: Libraries :: Python Modules'
]

setup(name='h5gtpy',
      version='0.1.0',
      author="Kerim Khemraev",
      author_email="tierracolada@gmail.com",
      maintainer="Kerim Khemraev",
      maintainer_email="tierracolada@gmail.com",
      description="h5gt: HDF5 wrapper originally written in C++ and translated to python",
      license='BSL',
      classifiers=classifiers,
      #py_modules='h5gtpy',
      # packages=['h5gtpy'],
      url="https://www.hdfgroup.org",
      cmdclass={"build_ext": build_ext},
      ext_modules=ext_modules
      )