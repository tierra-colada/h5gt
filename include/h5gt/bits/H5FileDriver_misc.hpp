/*
 *  Copyright (c), 2017-2018, Adrien Devresse <adrien.devresse@epfl.ch>
 *                            Juan Hernando <juan.hernando@epfl.ch>
 *
 *  Distributed under the Boost Software License, Version 1.0.
 *    (See accompanying file LICENSE_1_0.txt or copy at
 *          http://www.boost.org/LICENSE_1_0.txt)
 *
 */
#ifndef H5FILEDRIVER_MISC_HPP
#define H5FILEDRIVER_MISC_HPP

#include <H5Ppublic.h>

#ifdef H5_HAVE_PARALLEL
#include <H5FDmpi.h>
#endif

namespace h5gt {

inline MPIOFileDriver::MPIOFileDriver(MPI_Comm comm, MPI_Info info) {
  if (H5Pset_fapl_mpio(_hid, comm, info) < 0) {
    HDF5ErrMapper::ToException<FileException>(
          "Unable to set-up MPIO Driver configuration");
  }
}

inline void MPIOFileDriver::getFaplMPIO(MPI_Comm *comm, MPI_Info *info) {
  if (H5Pget_fapl_mpio(_hid, comm, info) < 0) {
    HDF5ErrMapper::ToException<FileException>(
          "Unable to get MPIO comm and info");
  }
}

} // namespace h5gt

#endif // H5FILEDRIVER_MISC_HPP
