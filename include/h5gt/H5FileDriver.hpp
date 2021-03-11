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

namespace h5gt {

///
/// \brief file driver base concept
///
class FileDriver : public FileAccessProps {};

///
/// \brief MPIIO Driver for Parallel HDF5
///
class MPIOFileDriver : public FileDriver {
public:
  explicit MPIOFileDriver(MPI_Comm comm, MPI_Info info);

  void getFaplMPIO(MPI_Comm *comm, MPI_Info *info);
};

}  // namespace h5gt

#include "bits/H5FileDriver_misc.hpp"

#endif // H5FILEDRIVER_HPP
