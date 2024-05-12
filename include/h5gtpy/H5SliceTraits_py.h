#include "h5gt_py.h"

namespace h5gtpy {

namespace ext {

template <typename Derivate, typename T>
void write_vector_raw(SliceTraits<Derivate>& self, std::vector<T>& v){
  self.write_raw(v.data(), AtomicType<T>());
};

template <typename Derivate, typename T>
void write_eigen_raw(SliceTraits<Derivate>& self, py::EigenDRef<Eigen::MatrixX<T>> M){
  self.write_raw(M.data(), AtomicType<T>());
};

template <typename Derivate, typename T>
std::vector<T> read_vector_raw(SliceTraits<Derivate>& self){
  static_assert(!std::is_const<T>::value,
      "read() requires a non-const structure to read data into");
  const auto& slice = static_cast<const Derivate&>(self);

  std::vector<T> v;
  v.resize(slice.getSpace().getElementCount());

  slice.read(v.data(), slice.getDataType());
  return v;
}

template <typename Derivate, typename T>
Eigen::MatrixX<T> read_eigen_raw(SliceTraits<Derivate>& self){
  static_assert(!std::is_const<T>::value,
      "read() requires a non-const structure to read data into");
  const auto& slice = static_cast<const Derivate&>(self);

  std::vector<size_t> dims = slice.getSpace().getDimensions();

  Eigen::MatrixX<T> M;
  if (dims.size() == 1){
    M.resize(dims[0], 1);
  } else if (dims.size() == 2){
    M.resize(dims[1], dims[0]);
  } else {
    M.resize(slice.getSpace().getElementCount(), 1);
  }

  slice.read(M.data(), slice.getDataType());
  return M;
}


} // ext


void ElementSet_py(py::class_<ElementSet> &py_obj);

template <typename Derivate>
void SliceTraits_py(py::class_<SliceTraits<Derivate> >& py_obj) {
  py_obj
      .def("select", py::overload_cast<
           const std::vector<size_t>&,
           const std::vector<size_t>&,
           const std::vector<size_t>&,
           const std::vector<size_t>&>(
             &SliceTraits<Derivate>::select, py::const_),
           py::arg("offset"),
           py::arg("count"),
           py::arg_v("stride", std::vector<size_t>(), "list()"),
           py::arg_v("block", std::vector<size_t>(), "list()"))
      .def("select", py::overload_cast<
           const ElementSet&>(
             &SliceTraits<Derivate>::select, py::const_),
           py::arg("elements"))
      .def("select_rows", py::overload_cast<
           const std::vector<size_t>&, size_t, size_t>(
             &SliceTraits<Derivate>::select_rows, py::const_),
           py::arg("ind"),
           py::arg_v("offset", 0, "0"),
           py::arg_v("count", 0, "0"))
      .def("select_cols", py::overload_cast<
           const std::vector<size_t>&, size_t, size_t>(
             &SliceTraits<Derivate>::select_cols, py::const_),
           py::arg("ind"),
           py::arg_v("offset", 0, "0"),
           py::arg_v("count", 0, "0"))


      // WRITE PY_TYPES
      .def("write", &SliceTraits<Derivate>::template write<bool>,
           py::arg("data"), "An exception is raised if the numbers of dimension of the buffer and of the dataset are different")
      .def("write", &SliceTraits<Derivate>::template write<long long>)
      .def("write", &SliceTraits<Derivate>::template write<double>)
      .def("write", &SliceTraits<Derivate>::template write<std::complex<double>>)
      .def("write", &SliceTraits<Derivate>::template write<std::string>)

      // WRITE VECTOR_TYPES
      .def("write", &SliceTraits<Derivate>::template write<std::vector<long long>>)
      .def("write", &SliceTraits<Derivate>::template write<std::vector<double>>)
      .def("write", &SliceTraits<Derivate>::template write<std::vector<std::complex<double>>>)
      .def("write", &SliceTraits<Derivate>::template write<std::vector<std::string>>)

      // WRITE EIGEN_TYPES
      .def("write", &SliceTraits<Derivate>::template write<Eigen::MatrixX<bool>>)
      .def("write", &SliceTraits<Derivate>::template write<Eigen::MatrixX<char>>)
      .def("write", &SliceTraits<Derivate>::template write<Eigen::MatrixX<signed char>>)
      .def("write", &SliceTraits<Derivate>::template write<Eigen::MatrixX<unsigned char>>)
      .def("write", &SliceTraits<Derivate>::template write<Eigen::MatrixX<short>>)
      .def("write", &SliceTraits<Derivate>::template write<Eigen::MatrixX<unsigned short>>)
      .def("write", &SliceTraits<Derivate>::template write<Eigen::MatrixX<int>>)
      .def("write", &SliceTraits<Derivate>::template write<Eigen::MatrixX<unsigned>>)
      .def("write", &SliceTraits<Derivate>::template write<Eigen::MatrixX<long>>)
      .def("write", &SliceTraits<Derivate>::template write<Eigen::MatrixX<unsigned long>>)
      .def("write", &SliceTraits<Derivate>::template write<Eigen::MatrixX<long long>>)
      .def("write", &SliceTraits<Derivate>::template write<Eigen::MatrixX<unsigned long long>>)
      .def("write", &SliceTraits<Derivate>::template write<Eigen::MatrixX<float>>)
      .def("write", &SliceTraits<Derivate>::template write<Eigen::MatrixX<double>>)
      .def("write", &SliceTraits<Derivate>::template write<Eigen::MatrixX<std::complex<float>>>)
      .def("write", &SliceTraits<Derivate>::template write<Eigen::MatrixX<std::complex<double>>>)


      // WRITE_VECTOR_RAW
      .def("write_raw", &ext::write_vector_raw<Derivate, long long>,
           py::arg("data"),
           "No dimensionality checks will be performed")
      .def("write_raw", &ext::write_vector_raw<Derivate, double>,
           py::arg("data"))
      .def("write_raw", &ext::write_vector_raw<Derivate, std::complex<double>>,
           py::arg("data"))
//      .def("write_raw", &ext::write_vector_raw<Derivate, const char*>,  // without any error it doesn't write anything. Doesn't work
//           py::arg("data"))

      // WRITE_EIGEN_RAW
      .def("write_raw", &ext::write_eigen_raw<Derivate, bool>,
           py::arg("data"))
      .def("write_raw", &ext::write_eigen_raw<Derivate, char>,
           py::arg("data"))
      .def("write_raw", &ext::write_eigen_raw<Derivate, signed char>,
           py::arg("data"))
      .def("write_raw", &ext::write_eigen_raw<Derivate, unsigned char>,
           py::arg("data"))
      .def("write_raw", &ext::write_eigen_raw<Derivate, short>,
           py::arg("data"))
      .def("write_raw", &ext::write_eigen_raw<Derivate, unsigned short>,
           py::arg("data"))
      .def("write_raw", &ext::write_eigen_raw<Derivate, int>,
           py::arg("data"))
      .def("write_raw", &ext::write_eigen_raw<Derivate, unsigned>,
           py::arg("data"))
      .def("write_raw", &ext::write_eigen_raw<Derivate, long long>,
           py::arg("data"))
      .def("write_raw", &ext::write_eigen_raw<Derivate, unsigned long long>,
           py::arg("data"))
      .def("write_raw", &ext::write_eigen_raw<Derivate, float>,
           py::arg("data"))
      .def("write_raw", &ext::write_eigen_raw<Derivate, double>,
           py::arg("data"))
      .def("write_raw", &ext::write_eigen_raw<Derivate, std::complex<float>>,
           py::arg("data"))
      .def("write_raw", &ext::write_eigen_raw<Derivate, std::complex<double>>,
           py::arg("data"))


      // READ
      .def("readBool", &ext::read_eigen_raw<Derivate, bool>)
      .def("readChar", &ext::read_eigen_raw<Derivate, char>)
      .def("readSChar", &ext::read_eigen_raw<Derivate, signed char>)
      .def("readUChar", &ext::read_eigen_raw<Derivate, unsigned char>)
      .def("readShort", &ext::read_eigen_raw<Derivate, short>)
      .def("readUShort", &ext::read_eigen_raw<Derivate, unsigned short>)
      .def("readInt", &ext::read_eigen_raw<Derivate, int>)
      .def("readUInt", &ext::read_eigen_raw<Derivate, unsigned>)
      .def("readLong", &ext::read_eigen_raw<Derivate, long>)
      .def("readULong", &ext::read_eigen_raw<Derivate, unsigned long>)
      .def("readLLong", &ext::read_eigen_raw<Derivate, long long>)
      .def("readULLong", &ext::read_eigen_raw<Derivate, unsigned long long>)
      .def("readFloat", &ext::read_eigen_raw<Derivate, float>)
      .def("readDouble", &ext::read_eigen_raw<Derivate, double>)
      .def("readCFloat", &ext::read_eigen_raw<Derivate, std::complex<float>>)
      .def("readCDouble", &ext::read_eigen_raw<Derivate, std::complex<double>>)
      .def("readStr", &ext::read_vector_raw<Derivate, const char*>);
}

} // h5gtpy
