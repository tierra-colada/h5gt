#ifndef H5CONVERTERCOMPOUND_HPP
#define H5CONVERTERCOMPOUND_HPP

#include <algorithm>
#include <cassert>
#include <functional>
#include <numeric>
#include <sstream>
#include <string>
#include <array>

#include <H5Dpublic.h>
#include <H5Ppublic.h>

#include "../H5DataType.hpp"
#include "../H5Reference.hpp"
#include "H5Utils.hpp"

namespace h5gt {

namespace details {


// COMPOUND CONVERTERS
// ===============

// apply conversion operations to basic scalar type
template <typename Scalar, class Enable>
struct compound_converter {
  inline compound_converter(const DataSpace&, const DataType&) noexcept {

    static_assert((std::is_arithmetic<Scalar>::value ||
        std::is_enum<Scalar>::value ||
        std::is_same<std::vector<const char*>, Scalar>::value ||
        std::is_same<std::string, Scalar>::value ||
        std::is_same<std::complex<float>, Scalar>::value ||
        std::is_same<std::complex<double>, Scalar>::value),
        "supported datatype should be an arithmetic value, a "
                      "std::string or a container/array");
  }

  inline Scalar* transform_read(Scalar& datamem) const noexcept {
    return &datamem;
  }

  inline const Scalar* transform_write(const Scalar& datamem) const noexcept {
    return &datamem;
  }

  inline void process_result(Scalar&) const noexcept {}
};



// apply conversion to compound
template <typename T, class Enable>
struct compound_converter<std::string, void> {
  inline compound_converter(const DataSpace& space, const DataType& type) noexcept
    : _c_vec(nullptr)
    , _space(space)
    , _type {
      assert(type.getClass() == DataTypeClass::CompoundType);
      _nmembers = H5Tget_nmembers(type.getId());
    }

  // create a C vector adapted to HDF5
  // fill last element with NULL to identify end
  inline value_type* transform_read(std::string&) noexcept {
    return &_c_vec;
  }

  inline const value_type* transform_write(const std::string& str) noexcept {
    _c_vec = str.c_str();
    return &_c_vec;
  }

  inline void process_result(std::string& str) {
    assert(_c_vec != nullptr);
    str = std::string(_c_vec);

    if (_c_vec != nullptr) {
      AtomicType<std::string> str_type;
      (void)H5Dvlen_reclaim(str_type.getId(false), _space.getId(false), H5P_DEFAULT,
                            &_c_vec);
    }
  }

  T _c_vec;
  const DataSpace& _space;
  const DataType& _type;
  int _nmembers;
};

} // details

} // h5gt


#endif // H5CONVERTERCOMPOUND_MISC_HPP