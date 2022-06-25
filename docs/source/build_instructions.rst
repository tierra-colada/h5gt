Build instructions
==================

Pre-requisites
--------------
* CMake
* compiler with C++17 support
* python (optional)

Dependencies
------------

* `HDF5 <https://www.hdfgroup.org/solutions/hdf5/>`_
* `Eigen <https://gitlab.com/libeigen/eigen>`_ (header only, optional)

h5gtpy dependencies
-------------------

* python
* `Eigen <https://gitlab.com/libeigen/eigen>`_ (header only, **required**)

Configure & Build
-----------------

.. code:: bash

   git clone https://github.com/tierra-colada/h5gt.git
   cd h5gt
   mkdir build
   cd build
      cmake .. 
     -DH5GT_BUILD_h5gtpy=ON 
     -DPYTHON_EXECUTABLE=/path/to/python.exe 
     -DH5GT_USE_EIGEN=ON 
     -DEigen3_ROOT=/path/to/eigen3 
     -DHDF5_DIR=/path/to/hdf5-config.cmake 
     -DHDF5_RUNTIME_DIRS=/path/to/hdf5-runtime   
     -DZLIB_RUNTIME_DIRS=/path/to/zlib-runtime   
     -DH5GTPY_COPY_RUNTIME_DEPS=OFF
   cmake --build . 
     # CMake settings
     -DCMAKE_BUILD_TYPE=Release
     -G "some generator"
   cmake --build . --config Release
   cmake --install . --prefix /path/to/h5gt-install

.. note::
   If you see that some of the dependencies were not resolved at installation step 
   then change ``<lib>_RUNTIME_DIRS``, rerun cmake & cmake install steps.

.. warning::
   ``H5GTPY_COPY_RUNTIME_DEPS`` copies all the resolved runtimes to
   ``site-packages/h5gtpy`` dir. There might be OS-specific runtimes that 
   nobody wants to copy. Thus this option is not recommended yet.

Supported platforms
-------------------

Windows, Linux, MacOS

.. toctree::
   :maxdepth: 2
