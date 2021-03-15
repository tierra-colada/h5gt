/*
 *  Copyright (c), 2017-2019, Blue Brain Project - EPFL
 *
 *  Distributed under the Boost Software License, Version 1.0.
 *    (See accompanying file LICENSE_1_0.txt or copy at
 *          http://www.boost.org/LICENSE_1_0.txt)
 *
 */

#include <string>
#include <iostream>

#include <h5gt/H5DataSet.hpp>
#include <h5gt/H5File.hpp>


#ifdef H5GT_USE_BOOST
#include <boost/multi_array.hpp>
#endif

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "tests_h5gt.hpp"

using namespace h5gt;


/// \brief Test for 2D old-style arrays (T array[x][y])
template <typename T>
void readWrite2DArrayTest() {
  std::ostringstream filename;
  filename << "h5_rw_2d_array_" << typeNameHelper<T>() << "_test.h5";
  const std::string DATASET_NAME("dset");
  const size_t x_size = 100;
  const size_t y_size = 10;

  // Create a new file using the default property lists.
  File file(filename.str(), File::ReadWrite | File::Create | File::Truncate);

  // Create the data space for the dataset.
  std::vector<size_t> dims{x_size, y_size};

  DataSpace dataspace(dims);

  // Create a dataset with arbitrary type
  DataSet dataset = file.createDataSet<T>(DATASET_NAME, dataspace);

  T array[x_size][y_size];

  ContentGenerate<T> generator;
  generate2D(array, x_size, y_size, generator);

  dataset.write(array);

  T result[x_size][y_size];
  dataset.read(result);

  for (size_t i = 0; i < x_size; ++i) {
    for (size_t j = 0; j < y_size; ++j) {
      ASSERT_EQ(result[i][j], array[i][j]);
    }
  }
}

TEST(H5GTMultiArray, ReadWrite2DArray) {
  readWrite2DArrayTest<int>();
  readWrite2DArrayTest<unsigned>();
  readWrite2DArrayTest<size_t>();
  readWrite2DArrayTest<ptrdiff_t>();
  readWrite2DArrayTest<float>();
  readWrite2DArrayTest<double>();
}

template <typename T>
void readWriteArrayTest() {
  const size_t x_size = 200;
  typename std::array<T, x_size> vec;
  ContentGenerate<T> generator;
  std::generate(vec.begin(), vec.end(), generator);

  typename std::array<T, x_size> result;
  auto dataset = readWriteDataset<T>(vec, result, 1, "std-array");

  ASSERT_THAT(result, ::testing::ElementsAreArray(vec));

  typename std::array<T, 1> tooSmall;
  EXPECT_THROW(dataset.read(tooSmall), DataSpaceException);
}

TEST(H5GTMultiArray, readWriteArray) {
  readWriteArrayTest<int>();
  readWriteArrayTest<unsigned>();
  readWriteArrayTest<size_t>();
  readWriteArrayTest<ptrdiff_t>();
  readWriteArrayTest<float>();
  readWriteArrayTest<double>();
}


template <typename T, typename VectorSubT>
void readWriteVectorNDTest(std::vector<VectorSubT>& ndvec,
                           const std::vector<size_t>& dims) {
  fillVec(ndvec, dims, ContentGenerate<T>());

  std::vector<VectorSubT> result;
  readWriteDataset<T>(ndvec, result, dims.size(), "vector");

  ASSERT_TRUE(checkLength(result, dims));
  ASSERT_TRUE(ndvec == result);
}

TEST(H5GTMultiArray, readWritSimpleVector) {
  std::vector<int> vec1;
  readWriteVectorNDTest<int>(vec1, {50});
  std::vector<ptrdiff_t> vec2;
  readWriteVectorNDTest<ptrdiff_t>(vec2, {50});
  std::vector<float> vec3;
  readWriteVectorNDTest<float>(vec3, {50});
  std::vector<double> vec4;
  readWriteVectorNDTest<double>(vec4, {50});
}

TEST(H5GTMultiArray, readWrite2DVector) {
  std::vector<std::vector<int> > _2dvec1;
  readWriteVectorNDTest<int>(_2dvec1, {10, 8});
  std::vector<std::vector<ptrdiff_t> > _2dvec2;
  readWriteVectorNDTest<ptrdiff_t>(_2dvec2, {10, 8});
  std::vector<std::vector<float> > _2dvec3;
  readWriteVectorNDTest<float>(_2dvec3, {10, 8});
  std::vector<std::vector<double> > _2dvec4;
  readWriteVectorNDTest<double>(_2dvec4, {10, 8});
}

TEST(H5GTMultiArray, readWrite3DVector) {
  std::vector<std::vector<std::vector<int> > > _3dvec1;
  readWriteVectorNDTest<int>(_3dvec1, {10, 5, 4});
  std::vector<std::vector<std::vector<ptrdiff_t> > > _3dvec2;
  readWriteVectorNDTest<ptrdiff_t>(_3dvec2, {10, 5, 4});
  std::vector<std::vector<std::vector<float> > > _3dvec3;
  readWriteVectorNDTest<float>(_3dvec3, {10, 5, 4});
  std::vector<std::vector<std::vector<double> > > _3dvec4;
  readWriteVectorNDTest<double>(_3dvec4, {10, 5, 4});
}

TEST(H5GTMultiArray, readWrite4DVector) {
  std::vector<std::vector<std::vector<std::vector<int> > > > _4dvec1;
  readWriteVectorNDTest<int>(_4dvec1, {5, 4, 3, 2});
  std::vector<std::vector<std::vector<std::vector<ptrdiff_t> > > > _4dvec2;
  readWriteVectorNDTest<ptrdiff_t>(_4dvec2, {5, 4, 3, 2});
  std::vector<std::vector<std::vector<std::vector<float> > > > _4dvec3;
  readWriteVectorNDTest<float>(_4dvec3, {5, 4, 3, 2});
  std::vector<std::vector<std::vector<std::vector<double> > > > _4dvec4;
  readWriteVectorNDTest<double>(_4dvec4, {5, 4, 3, 2});
}



#ifdef H5GT_USE_BOOST

template <typename T>
void MultiArray3DTest() {
  typedef typename boost::multi_array<T, 3> MultiArray;

  std::ostringstream filename;
  filename << "h5_rw_multiarray_" << typeNameHelper<T>() << "_test.h5";

  const int size_x = 10, size_y = 10, size_z = 10;
  const std::string DATASET_NAME("dset");
  MultiArray array(boost::extents[size_x][size_y][size_z]);

  ContentGenerate<T> generator;
  std::generate(array.data(), array.data() + array.num_elements(), generator);

  // Create a new file using the default property lists.
  File file(filename.str(), File::ReadWrite | File::Create | File::Truncate);

  DataSet dataset = file.createDataSet<T>(DATASET_NAME,
                                          DataSpace::From(array));

  dataset.write(array);

  // read it back
  MultiArray result;

  dataset.read(result);

  for (long i = 0; i < size_x; ++i) {
    for (long j = 0; j < size_y; ++j) {
      for (long k = 0; k < size_z; ++k) {
        ASSERT_EQ(array[i][j][k], result[i][j][k]);
      }
    }
  }
}

TEST(H5GTMultiArray, MultiArray3D) {
  MultiArray3DTest<int>();
  MultiArray3DTest<unsigned>();
  MultiArray3DTest<size_t>();
  MultiArray3DTest<ptrdiff_t>();
  MultiArray3DTest<float>();
  MultiArray3DTest<double>();
}

template <typename T>
void ublas_matrix_Test() {
  using Matrix = boost::numeric::ublas::matrix<T>;

  std::ostringstream filename;
  filename << "h5_rw_multiarray_" << typeNameHelper<T>() << "_test.h5";

  const size_t size_x = 10, size_y = 10;
  const std::string DATASET_NAME("dset");

  Matrix mat(size_x, size_y);

  ContentGenerate<T> generator;
  for (std::size_t i = 0; i < mat.size1(); ++i) {
    for (std::size_t j = 0; j < mat.size2(); ++j) {
      mat(i, j) = generator();
    }
  }

  // Create a new file using the default property lists.
  File file(filename.str(), File::ReadWrite | File::Create | File::Truncate);

  DataSet dataset = file.createDataSet<T>(DATASET_NAME, DataSpace::From(mat));

  dataset.write(mat);

  // read it back
  Matrix result;

  dataset.read(result);

  for (size_t i = 0; i < size_x; ++i) {
    for (size_t j = 0; j < size_y; ++j) {
      ASSERT_EQ(mat(i, j), result(i, j));
    }
  }
}

TEST(H5GTMultiArray, ublas_matrix) {
  ublas_matrix_Test<int>();
  ublas_matrix_Test<unsigned>();
  ublas_matrix_Test<size_t>();
  ublas_matrix_Test<ptrdiff_t>();
  ublas_matrix_Test<float>();
  ublas_matrix_Test<double>();
}

#endif
