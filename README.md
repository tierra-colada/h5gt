# h5gt
[![Build Status](https://travis-ci.com/tierra-colada/h5gt.svg?branch=main)](https://travis-ci.com/tierra-colada/h5gt)

**h5gt** is a C++ wrapper of [HDF5](https://www.hdfgroup.org/solutions/hdf5/) library based on [HighFive](https://github.com/BlueBrain/HighFive.git) project. It also includes python bindings that you can install with cmake and it is called **h5gtpy**.

***h5gtpy** uses [pybind11](https://github.com/pybind/pybind11) to make python bindings and there may be some perfomance penalty when copying matrices from C++ `Eigen3` matrices to `numpy` (or `std::vector` to `py::list`). I have not done any perfomance testing.*

***This project is aimed at developers who uses C++, python and HDF5 library. If you only need python HDF5 library you should better refer to [h5py](https://github.com/h5py/h5py) project.***

## Building h5gt
As **h5gt** is header only library you don't need to buid it (but of course you can do that). See `src/examples` and `tests/unit` for usage examples.

## Building h5gtpy
To build with python support you need:

* set `H5GT_BUILD_h5gtpy` to `ON`
* provide python to cmake (I do it with `PYTHON_EXECUTABLE=/path/to/python.exe`)
* enable Eigen3 support `H5GT_USE_EIGEN=ON` (`Eigen3_ROOT` is enough to find eigen)
* specify `HDF5_RUNTIME_DIR` containing HDF5-runtime (probably it is enough to have it in `PATH` env)
* **build HDF5 as SHARED C library**

then do something like:

```shell
cmake .. -DH5GT_BUILD_h5gtpy=ON -DPYTHON_EXECUTABLE=/path/to/python.exe -DH5GT_USE_EIGEN=ON -DEigen3_ROOT=/path/to/eigen3 -DHDF5_DIR=/path/to/hdf5-config.cmake -DHDF5_RUNTIME_DIR=/path/to/hdf5-runtime -DCOPY_H5GEOPY_RUNTIME_DEPS=ON
cmake --build . 
```
If your HDF5 depends on ZLIB or MPI you may also need to add it.

To run **h5gtpy** python must know where HDF5-runtime is. You may choose whether to copy HDF5-runtime to site-packages/h5gtpy dir or add this dir to path env.


## Installation h5gtpy

`h5gtpy` installation is done the same way one usually install cmake project:

`cmake --install . --prefix /some/path --config Release`

Then you can find h5gtpy in `site-packages` of your python env.

After installation you need to make sure that linked (on WIndows it is `hdf5.dll`) module is in the `PATH` env variable.

## Usage
Importing is done via:
```python
from h5gtpy import h5gt
```

A test is also given in `site-packages/h5gtpy`.
To run test:
```python
python -m unittest discover h5gtpy.tests
```

## Integrating with VS Code
There is stub file `h5gtpy/h5gt.pyi`. You need to set VS Code setting `"python.analysis.stubPath": "some/relative/path/to/stub-storage"`: where `stub-storage` is supposed to contain folder `h5gtpy` with `h5gt.pyi` file.

I'm some kind of a uncomfortable to distribute stub file, so to be sure that stub file is updated you can generate your own stub file:
```shell
pip install pybind11-stubgen
pybind11-stubgen --no-setup-py h5gtpy._h5gt
```

## Integrating with PyCharm
You don't need to provide stub file to PyCharm as it handles this by its own.

## Supported HDF5 versions
1.10, 1.12 (recommended)

## Supported platforms
Windows 10, Linux, OSX