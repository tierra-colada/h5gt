/*
 *  Copyright (c), 2017, Adrien Devresse <adrien.devresse@epfl.ch>
 *
 *  Distributed under the Boost Software License, Version 1.0.
 *    (See accompanying file LICENSE_1_0.txt or copy at
 *          http://www.boost.org/LICENSE_1_0.txt)
 *
 */
#ifndef H5UTILS_HPP
#define H5UTILS_HPP

// internal utilities functions
#include <algorithm>
#include <array>
#include <cstddef> // __GLIBCXX__
#include <exception>
#include <string>
#include <type_traits>
#include <vector>

#ifndef H5GT_MAX_PATH_LEN
#define H5GT_MAX_PATH_LEN 510
#endif
#ifdef H5GT_USE_BOOST
# include <boost/multi_array.hpp>
# include <boost/numeric/ublas/matrix.hpp>
#endif
#ifdef H5GT_USE_EIGEN
# include <Eigen/Eigen>
#endif

#include <H5public.h>

#include "../H5Exception.hpp"

namespace h5gt {

// If ever used, recognize dimensions of FixedLenStringArray
template <std::size_t N>
class FixedLenStringArray;


template <typename T>
using unqualified_t = typename std::remove_const<typename std::remove_reference<T>::type
>::type;

namespace details {
template <typename T>
struct inspector {
  using type = T;
  using base_type = unqualified_t<T>;

  static constexpr size_t ndim = 0;
  static constexpr size_t recursive_ndim = ndim;

  static std::array<size_t, recursive_ndim> getDimensions(const type& /* val */) {
    return std::array<size_t, recursive_ndim>();
  }
};

template <size_t N>
struct inspector<FixedLenStringArray<N>> {
  using type = FixedLenStringArray<N>;
  using base_type = FixedLenStringArray<N>;

  static constexpr size_t ndim = 1;
  static constexpr size_t recursive_ndim = ndim;

  static std::array<size_t, recursive_ndim> getDimensions(const type& val) {
    return std::array<size_t, recursive_ndim>{val.size()};
  }
};

template <typename T>
struct inspector<std::vector<T>> {
  using type = std::vector<T>;
  using value_type = T;
  using base_type = typename inspector<value_type>::base_type;

  static constexpr size_t ndim = 1;
  static constexpr size_t recursive_ndim = ndim + inspector<value_type>::recursive_ndim;

  static std::array<size_t, recursive_ndim> getDimensions(const type& val) {
    std::array<size_t, recursive_ndim> sizes{val.size()};
    size_t index = ndim;
    for (const auto& s: inspector<value_type>::getDimensions(val[0])) {
      sizes[index++] = s;
    }
    return sizes;
  }
};

template <typename T>
struct inspector<T*> {
  using type = T*;
  using value_type = T;
  using base_type = typename inspector<value_type>::base_type;

  static constexpr size_t ndim = 1;
  static constexpr size_t recursive_ndim = ndim + inspector<value_type>::recursive_ndim;

  static std::array<size_t, recursive_ndim> getDimensions(const type& /* val */) {
    throw std::string("Not possible to have size of a T*");
  }
};

template <typename T, size_t N>
struct inspector<T[N]> {
  using type = T[N];
  using value_type = T;
  using base_type = typename inspector<value_type>::base_type;

  static constexpr size_t ndim = 1;
  static constexpr size_t recursive_ndim = ndim + inspector<value_type>::recursive_ndim;

  static std::array<size_t, recursive_ndim> getDimensions(const type& val) {
    std::array<size_t, recursive_ndim> sizes{N};
    size_t index = ndim;
    for (const auto& s: inspector<value_type>::getDimensions(val[0])) {
      sizes[index++] = s;
    }
    return sizes;
  }
};

template <typename T, size_t N>
struct inspector<std::array<T, N>> {
  using type = std::array<T, N>;
  using value_type = T;
  using base_type = typename inspector<value_type>::base_type;

  static constexpr size_t ndim = 1;
  static constexpr size_t recursive_ndim = ndim + inspector<value_type>::recursive_ndim;

  static std::array<size_t, recursive_ndim> getDimensions(const type& val) {
    std::array<size_t, recursive_ndim> sizes{N};
    size_t index = ndim;
    for (const auto& s: inspector<value_type>::getDimensions(val[0])) {
      sizes[index++] = s;
    }
    return sizes;
  }
};

#ifdef H5GT_USE_EIGEN
template <typename T, int M, int N>
struct inspector<Eigen::Matrix<T, M, N>> {
  using type = Eigen::Matrix<T, M, N>;
  using value_type = T;
  using base_type = typename inspector<value_type>::base_type;

  static constexpr size_t ndim = 2;
  static constexpr size_t recursive_ndim = ndim + inspector<value_type>::recursive_ndim;

  static std::array<size_t, recursive_ndim> getDimensions(const type& val) {
    std::array<size_t, recursive_ndim> sizes{static_cast<size_t>(val.rows()), static_cast<size_t>(val.cols())};
    size_t index = ndim;
    for (const auto& s: inspector<value_type>::getDimensions(val.data()[0])) {
      sizes[index++] = s;
    }
    return sizes;
  }
};
#endif

#ifdef H5GT_USE_BOOST
template <typename T, size_t Dims>
struct inspector<boost::multi_array<T, Dims>> {
  using type = boost::multi_array<T, Dims>;
  using value_type = T;
  using base_type = typename inspector<value_type>::base_type;

  static constexpr size_t ndim = Dims;
  static constexpr size_t recursive_ndim = ndim + inspector<value_type>::recursive_ndim;

  static std::array<size_t, recursive_ndim> getDimensions(const type& val) {
    std::array<size_t, recursive_ndim> sizes;
    for (size_t i = 0; i < ndim; ++i) {
      sizes[i] = val.shape()[i];
    }

    size_t index = ndim;
    for (const auto& s: inspector<value_type>::getDimensions(val.data()[0])) {
      sizes[index++] = s;
    }
    return sizes;
  }
};

template <typename T>
struct inspector<boost::numeric::ublas::matrix<T>> {
  using type = boost::numeric::ublas::matrix<T>;
  using value_type = T;
  using base_type = typename inspector<value_type>::base_type;

  static constexpr size_t ndim = 2;
  static constexpr size_t recursive_ndim = ndim + inspector<value_type>::recursive_ndim;

  static std::array<size_t, recursive_ndim> getDimensions(const type& val) {
    std::array<size_t, recursive_ndim> sizes{val.size1(), val.size2()};
    size_t index = ndim;
    for (const auto& s: inspector<value_type>::getDimensions(val(0, 0))) {
      sizes[index++] = s;
    }
    return sizes;
  }
};
#endif


// Find the type of an eventual char array, otherwise void
template <typename>
struct type_char_array {
  typedef void type;
};

template <typename T>
struct type_char_array<T*> {
  typedef typename std::conditional<
  std::is_same<unqualified_t<T>, char>::value,
  char*,
  typename type_char_array<T>::type
  >::type type;
};

template <typename T, std::size_t N>
struct type_char_array<T[N]> {
  typedef typename std::conditional<
  std::is_same<unqualified_t<T>, char>::value,
  char[N],
  typename type_char_array<T>::type
  >::type type;
};


// check if the type is a container ( only vector supported for now )
template <typename>
struct is_container {
  static const bool value = false;
};

template <typename T>
struct is_container<std::vector<T> > {
  static const bool value = true;
};

// check if the type is a basic C-Array
template <typename>
struct is_c_array {
  static const bool value = false;
};

template <typename T>
struct is_c_array<T*> {
  static const bool value = true;
};

template <typename T, std::size_t N>
struct is_c_array<T[N]> {
  static const bool value = true;
};


// converter function for hsize_t -> size_t when hsize_t != size_t
template <typename Size>
inline std::vector<std::size_t> to_vector_size_t(const std::vector<Size>& vec) {
  static_assert(std::is_same<Size, std::size_t>::value == false,
      " hsize_t != size_t mandatory here");
  std::vector<size_t> res(vec.size());
  std::transform(vec.cbegin(), vec.cend(), res.begin(), [](Size e) {
    return static_cast<size_t>(e);
  });
  return res;
}

// converter function for hsize_t -> size_t when size_t == hsize_t
inline std::vector<std::size_t> to_vector_size_t(const std::vector<std::size_t>& vec) {
  return vec;
}

// read name from a H5 object using the specified function
template<typename T>
inline std::string get_name(T fct) {
  char buffer[H5GT_MAX_PATH_LEN + 1];
  ssize_t retcode = fct(buffer, static_cast<hsize_t>(H5GT_MAX_PATH_LEN) + 1);
  if (retcode < 0) {
    HDF5ErrMapper::ToException<GroupException>("Error accessing object name");
  }
  const size_t length = static_cast<std::size_t>(retcode);
  if (length <= H5GT_MAX_PATH_LEN) {
    return std::string(buffer, length);
  }
  std::vector<char> bigBuffer(length + 1, 0);
  fct(bigBuffer.data(), static_cast<hsize_t>(length) + 1);
  return std::string(bigBuffer.data(), length);
}

/// \brief splitPathSplit path of type /path///to/where/things/happen//
/// to output vector {"path", "to", "where", "things", "happen"}.
/// Path that starts from `/` is treated as absolute
/// \param path
/// \return
inline std::vector<std::string> splitPath(
    std::string path){
  std::vector<std::string> results;

  size_t cutAt;
  while ((cutAt = path.find_first_of('/')) != path.npos){
    if(cutAt > 0)
      results.push_back(path.substr (0,cutAt));
    path = path.substr(cutAt+1);
  }

  if (path.length () > 0)
    results.push_back(path);

  return results;
}

/// \brief splitPathToParentAndObj Return path to parent
/// and object name. E.g. if `path = /a/s` then it returns
/// `/a` as path and `s` as object name.
/// If path is empty or `path = /` then both output path
/// and object name are empty strings.
/// \param path
/// \param objName
/// \return
inline std::string splitPathToParentAndObj(
  const std::string& path, 
  std::string& objName) {
  if (path.empty())
    return std::string();

  std::vector<std::string> pathVec = splitPath(path);
  if(pathVec.empty()){
    objName.clear();
    return std::string();
  }

  std::string parentPath;
  if (path[0] == '/')
    parentPath = '/';

  for (size_t i = 0; i < pathVec.size()-1; i++){
    if (i < pathVec.size()-2)
      parentPath += pathVec[i] + '/';
    else
      parentPath += pathVec[i];
  }

  objName = pathVec.back();
  return parentPath;
}

}  // namespace details
}  // namespace h5gt

#endif // H5UTILS_HPP
