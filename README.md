# h5gt
[![Build Status](https://travis-ci.com/TierraColada/h5gt.svg?branch=main)](https://travis-ci.com/TierraColada/h5gt)

HDF5 C++ wrapper based on [HighFive project](https://github.com/BlueBrain/HighFive.git)

Includes python bindings that you can install with cmake.
It uses **pybind11** to make python bindings and there may be some perfomance penalty when copying matrices from C++ `Eigen3` matrices to `numpy` (or `std::vector` to `py::list`). I have not done any perfomance testing.

This project is aimed for developers who uses C++, python and **HDF5** library. If you only need python HDF5 library please refer to [h5py](https://github.com/h5py/h5py) project.

## Building h5gtpy
To build with python support you need:

* set `H5GT_BUILD_h5gtpy` to `ON`
* provide python to cmake (I do it with `PYTHON_EXECUTABLE=/path/to/python.exe`)
* enable Eigen3 support `H5GT_USE_EIGEN=ON` (`Eigen3_ROOT` is enough to find eigen)
* **dynamically build HDF5 C library**

then do something like:

```shell
cmake .. -DH5GT_BUILD_h5gtpy=ON -DPYTHON_EXECUTABLE=/path/to/python.exe -DH5GT_USE_EIGEN=ON -DEigen3_ROOT=/path/to/eigen3 -DHDF5_DIR=/path/to/hdf5-config.cmake
cmake --build . 
```
If your **HDF5** depends on **ZLIB** or **MPI** you also need to add it.


## Installation h5gtpy

`h5gtpy` installation is done the same way one usually install cmake project:

`cmake --install . --prefix /some/path --config Release`

Then you can find h5gtpy in **site-packages** of your python env.

After installation you need to make sure that linked (on WIndows it is `hdf5.dll`) module is in the `PATH` env variable.

## Usage
Test is given in **h5gtpy** folder in **site-packages**
Importing is done via:

`from h5gtpy import h5gt`

## Integrating with VS Code
There is stub file `h5gtpy/h5gt.pyi`. You need to set VS Code setting `"python.analysis.stubPath": "some/relative/path/to/stub-storage"`: where **stub-storage** is supposed to contain folder `h5gtpy` with `h5gt.pyi` file.

I'm some kind of a uncomfortable to distribute stub file, so to be sure that stub file is updated you can generate your own stub file:
```shell
pip install pybind11-stubgen
pybind11-stubgen --no-setup-py h5gtpy._h5gt
```

## Integrating with PyCharm
You don't need to provide stub file to PyCharm as it handles this by its own.