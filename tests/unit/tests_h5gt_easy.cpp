/*
 *  Copyright (c), 2017, Adrien Devresse <adrien.devresse@epfl.ch>
 *
 *  Distributed under the Boost Software License, Version 1.0.
 *    (See accompanying file LICENSE_1_0.txt or copy at
 *          http://www.boost.org/LICENSE_1_0.txt)
 *
 */

#include <complex>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <memory>
#include <random>
#include <string>
#include <typeinfo>
#include <vector>

#include <stdio.h>

#include <h5gt/H5Easy.hpp>

#ifdef H5GT_USE_XTENSOR
#include <xtensor/xrandom.hpp>
#endif

#include <gtest/gtest.h>
#include <gmock/gmock.h>

TEST(H5GTEasy, H5Easy_Compression)
{
  {
    H5Easy::DumpOptions options = H5Easy::DumpOptions(H5Easy::Compression());
    ASSERT_EQ(options.compress(), true);
    ASSERT_EQ(options.getCompressionLevel(), 9);
  }

  {
    H5Easy::DumpOptions options(H5Easy::Compression(true));
    ASSERT_EQ(options.compress(), true);
    ASSERT_EQ(options.getCompressionLevel(), 9);
  }

  {
    H5Easy::DumpOptions options(H5Easy::Compression(false));
    ASSERT_EQ(options.compress(), false);
    ASSERT_EQ(options.getCompressionLevel(), 0);
  }

  {
    H5Easy::DumpOptions options(H5Easy::Compression(8));
    ASSERT_EQ(options.compress(), true);
    ASSERT_EQ(options.getCompressionLevel(), 8);
  }
}

TEST(H5GTEasy, H5Easy_scalar)
{
  H5Easy::File file("test.h5", H5Easy::File::Overwrite);

  double a = 1.2345;
  int b = 12345;
  std::string c = "12345";

  H5Easy::dump(file, "/path/to/a", a);
  H5Easy::dump(file, "/path/to/b", b);
  H5Easy::dump(file, "/path/to/c", c);
  H5Easy::dump(file, "/path/to/c", c, H5Easy::DumpMode::Overwrite);

  double a_r = H5Easy::load<double>(file, "/path/to/a");
  int b_r = H5Easy::load<int>(file, "/path/to/b");
  std::string c_r = H5Easy::load<std::string>(file, "/path/to/c");

  ASSERT_EQ(a == a_r, true);
  ASSERT_EQ(b == b_r, true);
  ASSERT_EQ(c == c_r, true);
}

TEST(H5GTEasy, H5Easy_vector1d)
{
  H5Easy::File file("test.h5", H5Easy::File::Overwrite);

  std::vector<size_t> a = {1,2,3,4,5};

  H5Easy::dump(file, "/path/to/a", a);

  std::vector<size_t> a_r = H5Easy::load<std::vector<size_t>>(file, "/path/to/a");

  ASSERT_EQ(a == a_r, true);
}

TEST(H5GTEasy, H5Easy_vector2d)
{
  H5Easy::File file("test.h5", H5Easy::File::Overwrite);

  std::vector<std::vector<size_t>> a({{0, 1}, {2, 3}, {4, 5}});

  H5Easy::dump(file, "/path/to/a", a);

  decltype(a) a_r = H5Easy::load<decltype(a)>(file, "/path/to/a");

  ASSERT_EQ(a == a_r, true);
}

TEST(H5GTEasy, H5Easy_vector2d_compression)
{
  H5Easy::File file("test.h5", H5Easy::File::Overwrite);

  std::vector<std::vector<size_t>> a({{0, 1}, {2, 3}, {4, 5}});

  H5Easy::dump(file, "/path/to/a", a,
               H5Easy::DumpOptions(H5Easy::Compression(9)));

  H5Easy::dump(file, "/path/to/a", a,
               H5Easy::DumpOptions(H5Easy::Compression(), H5Easy::DumpMode::Overwrite));

  decltype(a) a_r = H5Easy::load<decltype(a)>(file, "/path/to/a");

  ASSERT_EQ(a == a_r, true);
}

TEST(H5GTEasy, H5Easy_vector3d)
{
  H5Easy::File file("test.h5", H5Easy::File::Overwrite);

  using type = std::vector<std::vector<std::vector<size_t>>>;

  type a({{{0, 1}, {2, 3}}, {{4, 5}, {6, 7}}, {{8, 9}, {10, 11}}});

  H5Easy::dump(file, "/path/to/a", a);

  type a_r = H5Easy::load<type>(file, "/path/to/a");

  ASSERT_EQ(a == a_r, true);
}

TEST(H5GTEasy, H5Easy_Attribute_scalar)
{
  H5Easy::File file("test.h5", H5Easy::File::Overwrite);

  double a = 1.2345;
  int b = 12345;
  std::string c = "12345";

  H5Easy::dump(file, "/path/to/a", a);
  H5Easy::dumpAttribute(file, "/path/to/a", "a", a);
  H5Easy::dumpAttribute(file, "/path/to/a", "a", a, H5Easy::DumpMode::Overwrite);
  H5Easy::dumpAttribute(file, "/path/to/a", "b", b);
  H5Easy::dumpAttribute(file, "/path/to/a", "c", c);

  double a_r = H5Easy::loadAttribute<double>(file, "/path/to/a", "a");
  int b_r = H5Easy::loadAttribute<int>(file, "/path/to/a", "b");
  std::string c_r = H5Easy::loadAttribute<std::string>(file, "/path/to/a", "c");

  ASSERT_EQ(a == a_r, true);
  ASSERT_EQ(b == b_r, true);
  ASSERT_EQ(c == c_r, true);
}

#ifdef H5GT_USE_XTENSOR
TEST(H5GTEasy, H5Easy_extend1d)
{
  H5Easy::File file("test.h5", H5Easy::File::Overwrite);

  for (size_t i = 0; i < 10; ++i) {
    H5Easy::dump(file, "/path/to/A", i, {i});
  }

  xt::xarray<size_t> A = xt::arange<size_t>(10);

  xt::xarray<size_t> A_r = H5Easy::load<xt::xarray<size_t>>(file, "/path/to/A");

  size_t Amax = H5Easy::load<size_t>(file, "/path/to/A", {9});

  ASSERT_EQ(xt::allclose(A, A_r), true);
  ASSERT_EQ(Amax == 9, true);
}

TEST(H5GTEasy, H5Easy_extend2d)
{
  H5Easy::File file("test.h5", H5Easy::File::Overwrite);

  for (size_t i = 0; i < 10; ++i) {
    for (size_t j = 0; j < 5; ++j) {
      H5Easy::dump(file, "/path/to/A", i * 5 + j, {i, j});
    }
  }

  xt::xarray<size_t> A = xt::arange<size_t>(10 * 5);

  A.reshape({10, 5});

  xt::xarray<size_t> A_r = H5Easy::load<xt::xarray<size_t>>(file, "/path/to/A");

  size_t Amax = H5Easy::load<size_t>(file, "/path/to/A", {9, 4});

  ASSERT_EQ(xt::allclose(A, A_r), true);
  ASSERT_EQ(Amax == 49, true);
}

TEST(H5GTEasy, H5Easy_xtensor)
{
  H5Easy::File file("test.h5", H5Easy::File::Overwrite);

  xt::xtensor<double, 2> A = 100. * xt::random::randn<double>({20, 5});
  xt::xtensor<int, 2> B = A;

  H5Easy::dump(file, "/path/to/A", A);
  H5Easy::dump(file, "/path/to/B", B);

  xt::xtensor<double,2> A_r = H5Easy::load<xt::xtensor<double,2>>(file, "/path/to/A");
  xt::xtensor<int, 2> B_r = H5Easy::load<xt::xtensor<int, 2>>(file, "/path/to/B");

  ASSERT_EQ(xt::allclose(A, A_r), true);
  ASSERT_EQ(xt::all(xt::equal(B, B_r)), true);
}

TEST(H5GTEasy, H5Easy_xarray)
{
  H5Easy::File file("test.h5", H5Easy::File::Overwrite);

  xt::xarray<double> A = 100. * xt::random::randn<double>({20, 5});
  xt::xarray<int> B = A;

  H5Easy::dump(file, "/path/to/A", A);
  H5Easy::dump(file, "/path/to/B", B);

  xt::xarray<double> A_r = H5Easy::load<xt::xarray<double>>(file, "/path/to/A");
  xt::xarray<int> B_r = H5Easy::load<xt::xarray<int>>(file, "/path/to/B");

  ASSERT_EQ(xt::allclose(A, A_r), true);
  ASSERT_EQ(xt::all(xt::equal(B, B_r)), true);
}

TEST(H5GTEasy, H5Easy_xtensor_compress)
{
  H5Easy::File file("test.h5", H5Easy::File::Overwrite);

  xt::xtensor<double, 2> A = 100. * xt::random::randn<double>({20, 5});
  xt::xtensor<int, 2> B = A;

  H5Easy::dump(file, "/path/to/A", A,
               H5Easy::DumpOptions(H5Easy::Compression()));

  H5Easy::dump(file, "/path/to/A", A,
               H5Easy::DumpOptions(H5Easy::Compression(), H5Easy::DumpMode::Overwrite));

  H5Easy::dump(file, "/path/to/B", B,
               H5Easy::DumpOptions(H5Easy::Compression()));

  xt::xtensor<double,2> A_r = H5Easy::load<xt::xtensor<double,2>>(file, "/path/to/A");
  xt::xtensor<int, 2> B_r = H5Easy::load<xt::xtensor<int, 2>>(file, "/path/to/B");

  ASSERT_EQ(xt::allclose(A, A_r), true);
  ASSERT_EQ(xt::all(xt::equal(B, B_r)), true);
}

TEST(H5GTEasy, H5Easy_Attribute_xtensor)
{
  H5Easy::File file("test.h5", H5Easy::File::Overwrite);

  xt::xtensor<double, 2> A = 100. * xt::random::randn<double>({20, 5});
  xt::xtensor<int, 2> B = A;

  H5Easy::dump(file, "/path/to/A", A);
  H5Easy::dumpAttribute(file, "/path/to/A", "A", A);
  H5Easy::dumpAttribute(file, "/path/to/A", "B", B);

  xt::xtensor<double,2> A_r = H5Easy::loadAttribute<xt::xtensor<double,2>>(file, "/path/to/A", "A");
  xt::xtensor<int, 2> B_r = H5Easy::loadAttribute<xt::xtensor<int, 2>>(file, "/path/to/A", "B");

  ASSERT_EQ(xt::allclose(A, A_r), true);
  ASSERT_EQ(xt::all(xt::equal(B, B_r)), true);
}
#endif

#ifdef H5GT_USE_EIGEN
TEST(H5GTEasy, H5Easy_Eigen_MatrixX)
{
  H5Easy::File file("test.h5", H5Easy::File::Overwrite);

  Eigen::MatrixXd A = 100. * Eigen::MatrixXd::Random(20, 5);
  Eigen::MatrixXi B = A.cast<int>();

  H5Easy::dump(file, "/path/to/A", A);
  H5Easy::dump(file, "/path/to/B", B);

  Eigen::MatrixXd A_r = H5Easy::load<Eigen::MatrixXd>(file, "/path/to/A");
  Eigen::MatrixXi B_r = H5Easy::load<Eigen::MatrixXi>(file, "/path/to/B");

  ASSERT_EQ(A.isApprox(A_r), true);
  ASSERT_EQ(B.isApprox(B_r), true);
}

TEST(H5GTEasy, H5Easy_Eigen_ArrayXX)
{
  H5Easy::File file("test.h5", H5Easy::File::Overwrite);

  Eigen::ArrayXXf A = 100. * Eigen::ArrayXXf::Random(20, 5);
  Eigen::ArrayXXi B = A.cast<int>();

  H5Easy::dump(file, "/path/to/A", A);
  H5Easy::dump(file, "/path/to/B", B);

  Eigen::ArrayXXf A_r = H5Easy::load<Eigen::MatrixXf>(file, "/path/to/A");
  Eigen::ArrayXXi B_r = H5Easy::load<Eigen::MatrixXi>(file, "/path/to/B");

  ASSERT_EQ(A.isApprox(A_r), true);
  ASSERT_EQ(B.isApprox(B_r), true);
}

TEST(H5GTEasy, H5Easy_Eigen_ArrayX)
{
  H5Easy::File file("test.h5", H5Easy::File::Overwrite);

  Eigen::ArrayXf A = Eigen::ArrayXf::Random(50);
  Eigen::ArrayXi B = A.cast<int>();

  H5Easy::dump(file, "/path/to/A", A);
  H5Easy::dump(file, "/path/to/B", B);

  Eigen::ArrayXf A_r = H5Easy::load<Eigen::ArrayXf>(file, "/path/to/A");
  Eigen::ArrayXi B_r = H5Easy::load<Eigen::ArrayXi>(file, "/path/to/B");

  ASSERT_EQ(A.isApprox(A_r), true);
  ASSERT_EQ(B.isApprox(B_r), true);
}


TEST(H5GTEasy, H5Easy_Eigen_VectorX)
{
  H5Easy::File file("test.h5", H5Easy::File::Overwrite);

  Eigen::VectorXd A = 100. * Eigen::VectorXd::Random(20);
  Eigen::VectorXi B = A.cast<int>();

  H5Easy::dump(file, "/path/to/A", A);
  H5Easy::dump(file, "/path/to/B", B);

  Eigen::VectorXd A_r = H5Easy::load<Eigen::VectorXd>(file, "/path/to/A");
  Eigen::VectorXi B_r = H5Easy::load<Eigen::VectorXi>(file, "/path/to/B");

  ASSERT_EQ(A.isApprox(A_r), true);
  ASSERT_EQ(B.isApprox(B_r), true);
}

TEST(H5GTEasy, H5Easy_Eigen_MatrixXRowMajor)
{
  typedef Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor> MatrixXd;
  typedef Eigen::Matrix<int   , Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor> MatrixXi;

  H5Easy::File file("test.h5", H5Easy::File::Overwrite);

  MatrixXd A = 100. * MatrixXd::Random(20, 5);
  MatrixXi B = A.cast<int>();

  H5Easy::dump(file, "/path/to/A", A);
  H5Easy::dump(file, "/path/to/B", B);

  MatrixXd A_r = H5Easy::load<MatrixXd>(file, "/path/to/A");
  MatrixXi B_r = H5Easy::load<MatrixXi>(file, "/path/to/B");

  ASSERT_EQ(A.isApprox(A_r), true);
  ASSERT_EQ(B.isApprox(B_r), true);
}

TEST(H5GTEasy, H5Easy_Eigen_VectorXRowMajor)
{
  typedef Eigen::Matrix<double, 1, Eigen::Dynamic, Eigen::RowMajor> VectorXd;
  typedef Eigen::Matrix<int   , 1, Eigen::Dynamic, Eigen::RowMajor> VectorXi;

  H5Easy::File file("test.h5", H5Easy::File::Overwrite);

  VectorXd A = 100. * VectorXd::Random(20);
  VectorXi B = A.cast<int>();

  H5Easy::dump(file, "/path/to/A", A);
  H5Easy::dump(file, "/path/to/B", B);

  VectorXd A_r = H5Easy::load<VectorXd>(file, "/path/to/A");
  VectorXi B_r = H5Easy::load<VectorXi>(file, "/path/to/B");

  ASSERT_EQ(A.isApprox(A_r), true);
  ASSERT_EQ(B.isApprox(B_r), true);
}

TEST(H5GTEasy, H5Easy_Eigen_Map)
{
  H5Easy::File file("test.h5", H5Easy::File::Overwrite);

  std::vector<int> A = {1, 2, 3, 4, 5, 6, 7, 8, 9};
  Eigen::Map<Eigen::VectorXi> mapped_vector(A.data(), static_cast<int>(A.size()));

  H5Easy::dump(file, "/path/to/A", mapped_vector);

  std::vector<int> A_r = H5Easy::load<std::vector<int>>(file, "/path/to/A");

  ASSERT_EQ(A == A_r, true);
}

TEST(H5GTEasy, H5Easy_Attribute_Eigen_MatrixX)
{
  H5Easy::File file("test.h5", H5Easy::File::Overwrite);

  Eigen::MatrixXd A = 100. * Eigen::MatrixXd::Random(20, 5);
  Eigen::MatrixXi B = A.cast<int>();

  H5Easy::dump(file, "/path/to/A", A);
  H5Easy::dumpAttribute(file, "/path/to/A", "A", A);
  H5Easy::dumpAttribute(file, "/path/to/A", "B", B);

  Eigen::MatrixXd A_r = H5Easy::loadAttribute<Eigen::MatrixXd>(file, "/path/to/A", "A");
  Eigen::MatrixXi B_r = H5Easy::loadAttribute<Eigen::MatrixXi>(file, "/path/to/A", "B");

  ASSERT_EQ(A.isApprox(A_r), true);
  ASSERT_EQ(B.isApprox(B_r), true);
}
#endif

#ifdef H5GT_USE_OPENCV
TEST(H5GTEasy, H5Easy_OpenCV_Mat_)
{
  H5Easy::File file("test.h5", H5Easy::File::Overwrite);

  using T = typename cv::Mat_<double>;

  T A(3, 4, 0.0);
  A(0, 0) = 0.0;
  A(0, 1) = 1.0;
  A(0, 2) = 2.0;
  A(0, 3) = 3.0;
  A(1, 0) = 4.0;
  A(1, 1) = 5.0;
  A(1, 2) = 6.0;
  A(1, 3) = 7.0;
  A(2, 0) = 8.0;
  A(2, 1) = 9.0;
  A(2, 2) = 10.0;
  A(2, 3) = 11.0;

  H5Easy::dump(file, "/path/to/A", A);
  H5Easy::dumpAttribute(file, "/path/to/A", "attr", A);

  T A_r = H5Easy::load<T>(file, "/path/to/A");
  T B_r = H5Easy::loadAttribute<T>(file, "/path/to/A", "attr");

  std::vector<double> a(A.begin(), A.end());
  std::vector<double> a_r(A_r.begin(), A_r.end());
  std::vector<double> b_r(A_r.begin(), A_r.end());

  ASSERT_EQ(a == a_r, true);
  ASSERT_EQ(a == b_r, true);
}
#endif
