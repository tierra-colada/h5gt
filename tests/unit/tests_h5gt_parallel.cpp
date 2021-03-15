/*
 *  Copyright (c), 2017-2019, Blue Brain Project - EPFL
 *
 *  Distributed under the Boost Software License, Version 1.0.
 *    (See accompanying file LICENSE_1_0.txt or copy at
 *          http://www.boost.org/LICENSE_1_0.txt)
 *
 */

#include <cstdlib>
#include <iostream>
#include <string>
#include <typeinfo>
#include <vector>

#include <h5gt/H5File.hpp>
#include <h5gt/H5DataSet.hpp>
#include <h5gt/H5DataSpace.hpp>
#include <h5gt/H5Group.hpp>

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "tests_h5gt.hpp"

using namespace h5gt;

template <typename T>
void selectionArraySimpleTestParallel() {

  int mpi_rank, mpi_size;
  MPI_Comm_size(MPI_COMM_WORLD, &mpi_size);
  MPI_Comm_rank(MPI_COMM_WORLD, &mpi_rank);

  typedef typename std::vector<T> Vector;

  std::ostringstream filename;
  filename << "h5_rw_select_parallel_test_" << typeNameHelper<T>() << "_test.h5";

  const auto size_x = static_cast<size_t>(mpi_size);
  const auto offset_x = static_cast<size_t>(mpi_rank);
  const auto count_x = static_cast<size_t>(mpi_size - mpi_rank);

  const std::string DATASET_NAME("dset");

  Vector values(size_x);

  ContentGenerate<T> generator;
  std::generate(values.begin(), values.end(), generator);

  // Create a new file using the default property lists.
  File file(filename.str(), File::ReadWrite | File::Create | File::Truncate,
            MPIOFileDriver(MPI_COMM_WORLD, MPI_INFO_NULL));

  DataSet dataset =
      file.createDataSet<T>(DATASET_NAME, DataSpace::From(values));

  dataset.write(values);

  file.flush();

  // read it back
  Vector result;
  std::vector<size_t> offset;
  offset.push_back(offset_x);
  std::vector<size_t> size;
  size.push_back(count_x);

  Selection slice = dataset.select(offset, size);

  EXPECT_EQ(slice.getSpace().getDimensions()[0], size_x);
  EXPECT_EQ(slice.getMemSpace().getDimensions()[0], count_x);

  slice.read(result);

  EXPECT_EQ(result.size(), count_x);

  for (size_t i = offset_x; i < count_x; ++i) {
    // std::cout << result[i] << " ";
    EXPECT_EQ(values[i + offset_x], result[i]);
  }
}

TEST(H5GTParallel, selectionArraySimple) {
  MPI_Init(NULL, NULL);
//  selectionArraySimpleTestParallel<std::string>();
  selectionArraySimpleTestParallel<int>();
  selectionArraySimpleTestParallel<unsigned>();
  selectionArraySimpleTestParallel<size_t>();
  selectionArraySimpleTestParallel<ptrdiff_t>();
  selectionArraySimpleTestParallel<float>();
  selectionArraySimpleTestParallel<double>();
  selectionArraySimpleTestParallel<std::complex<double> >();
  MPI_Finalize();
}
