/*
 *  Copyright (c), 2017, Adrien Devresse <adrien.devresse@epfl.ch>
 *
 *  Distributed under the Boost Software License, Version 1.0.
 *    (See accompanying file LICENSE_1_0.txt or copy at
 *          http://www.boost.org/LICENSE_1_0.txt)
 *
 */
#ifndef H5FILEDRIVER_HPP
#define H5FILEDRIVER_HPP

#include "H5PropertyList.hpp"

#ifdef H5_HAVE_PARALLEL
#include <H5FDmpi.h>
#endif

namespace h5gt {

///
/// \brief file driver base concept
///
class FileDriver : public FileAccessProps {};


#ifdef H5GT_PARALLEL

///
/// \brief MPIIO Driver for Parallel HDF5
///
class MPIOFileDriver : public FileDriver {
public:
  explicit MPIOFileDriver(MPI_Comm comm, MPI_Info info);

  void getFaplMPIO(MPI_Comm *comm, MPI_Info *info);
};

#endif

}  // namespace h5gt

#include "bits/H5FileDriver_misc.hpp"

#endif // H5FILEDRIVER_HPP
