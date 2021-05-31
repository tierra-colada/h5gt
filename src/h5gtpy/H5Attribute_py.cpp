#include "../../include/h5gtpy/H5Attribute_py.h"

namespace h5gtpy {

namespace ext {

template <typename T>
void write_vector_raw_attr(Attribute& self, std::vector<T>& v){
  self.write_raw(v.data(), AtomicType<T>());
};

template <typename T>
void write_eigen_raw_attr(Attribute& self, py::EigenDRef<Eigen::MatrixX<T>> M){
  self.write_raw(M.data(), AtomicType<T>());
};

template <typename T>
std::vector<T> read_vector_raw_attr(Attribute& self){
  std::vector<T> v;
  v.resize(self.getSpace().getElementCount());

  self.read(v.data(), self.getDataType());
  return v;
}

template <typename T>
Eigen::MatrixX<T> read_eigen_raw_attr(Attribute& self){
  std::vector<size_t> dims = self.getSpace().getDimensions();

  Eigen::MatrixX<T> M;
  if (dims.size() == 1){
    M.resize(dims[0], 1);
  } else if (dims.size() == 2){
    M.resize(dims[0], dims[1]);
  } else {
    size_t rows = dims[0];
    size_t cols = dims[1];
    for (size_t i = 2; i < dims.size(); i++){
      rows *= 2;
    }
    M.resize(rows, cols);
  }

  self.read(M.data(), self.getDataType());
  return M;
}


} // ext


void Attribute_py(py::class_<Attribute, Object> &py_obj){
  py_obj
      .def("getName", &Attribute::getName,
           "return the name of the current attribute")
      .def("getStorageSize", &Attribute::getStorageSize,
           "returns the amount of storage allocated for an attribute")
      .def("getDataType", &Attribute::getDataType,
           "return the datatype associated with this attribute")
      .def("getSpace", &Attribute::getSpace,
           "return the dataspace associated with this attribute")
      .def("getMemSpace", &Attribute::getMemSpace,
           "same as getSpace for attribute, compatibility with Selection")


      // WRITE PY_TYPES
      .def("write", &Attribute::write<bool>,
           py::arg("data"), "An exception is raised if the numbers of dimension of the buffer and of the dataset are different")
      .def("write", &Attribute::write<long long>)
      .def("write", &Attribute::write<double>)
      .def("write", &Attribute::write<std::complex<double>>)
      .def("write", &Attribute::write<std::string>)

      // WRITE VECTOR_TYPES
      .def("write", &Attribute::write<std::vector<long long>>)
      .def("write", &Attribute::write<std::vector<double>>)
      .def("write", &Attribute::write<std::vector<std::complex<double>>>)
      .def("write", &Attribute::write<std::vector<std::string>>)

      // WRITE EIGEN_TYPES
      .def("write", &Attribute::write<Eigen::MatrixX<bool>>)
      .def("write", &Attribute::write<Eigen::MatrixX<char>>)
      .def("write", &Attribute::write<Eigen::MatrixX<signed char>>)
      .def("write", &Attribute::write<Eigen::MatrixX<unsigned char>>)
      .def("write", &Attribute::write<Eigen::MatrixX<short>>)
      .def("write", &Attribute::write<Eigen::MatrixX<unsigned short>>)
      .def("write", &Attribute::write<Eigen::MatrixX<int>>)
      .def("write", &Attribute::write<Eigen::MatrixX<unsigned>>)
      .def("write", &Attribute::write<Eigen::MatrixX<long>>)
      .def("write", &Attribute::write<Eigen::MatrixX<unsigned long>>)
      .def("write", &Attribute::write<Eigen::MatrixX<long long>>)
      .def("write", &Attribute::write<Eigen::MatrixX<unsigned long long>>)
      .def("write", &Attribute::write<Eigen::MatrixX<float>>)
      .def("write", &Attribute::write<Eigen::MatrixX<double>>)
      .def("write", &Attribute::write<Eigen::MatrixX<std::complex<float>>>)
      .def("write", &Attribute::write<Eigen::MatrixX<std::complex<double>>>)


      // write_vector_raw_attr
      .def("write_raw", &ext::write_vector_raw_attr<long long>,
           py::arg("data"),
           "No dimensionality checks will be performed")
      .def("write_raw", &ext::write_vector_raw_attr<double>,
           py::arg("data"))
      .def("write_raw", &ext::write_vector_raw_attr<std::complex<double>>,
           py::arg("data"))
//      .def("write_raw", &ext::write_vector_raw_attr<const char*>,  // without any error it doesn't write anything. Doesn't work
//           py::arg("data"))

      // write_eigen_raw_attr
      .def("write_raw", &ext::write_eigen_raw_attr<bool>,
           py::arg("data"))
      .def("write_raw", &ext::write_eigen_raw_attr<char>,
           py::arg("data"))
      .def("write_raw", &ext::write_eigen_raw_attr<signed char>,
           py::arg("data"))
      .def("write_raw", &ext::write_eigen_raw_attr<unsigned char>,
           py::arg("data"))
      .def("write_raw", &ext::write_eigen_raw_attr<short>,
           py::arg("data"))
      .def("write_raw", &ext::write_eigen_raw_attr<unsigned short>,
           py::arg("data"))
      .def("write_raw", &ext::write_eigen_raw_attr<int>,
           py::arg("data"))
      .def("write_raw", &ext::write_eigen_raw_attr<unsigned>,
           py::arg("data"))
      .def("write_raw", &ext::write_eigen_raw_attr<long long>,
           py::arg("data"))
      .def("write_raw", &ext::write_eigen_raw_attr<unsigned long long>,
           py::arg("data"))
      .def("write_raw", &ext::write_eigen_raw_attr<float>,
           py::arg("data"))
      .def("write_raw", &ext::write_eigen_raw_attr<double>,
           py::arg("data"))
      .def("write_raw", &ext::write_eigen_raw_attr<std::complex<float>>,
           py::arg("data"))
      .def("write_raw", &ext::write_eigen_raw_attr<std::complex<double>>,
           py::arg("data"))


      // READ
      .def("readBool", &ext::read_eigen_raw_attr<bool>)
      .def("readChar", &ext::read_eigen_raw_attr<char>)
      .def("readSChar", &ext::read_eigen_raw_attr<signed char>)
      .def("readUChar", &ext::read_eigen_raw_attr<unsigned char>)
      .def("readShort", &ext::read_eigen_raw_attr<short>)
      .def("readUShort", &ext::read_eigen_raw_attr<unsigned short>)
      .def("readInt", &ext::read_eigen_raw_attr<int>)
      .def("readUInt", &ext::read_eigen_raw_attr<unsigned>)
      .def("readLong", &ext::read_eigen_raw_attr<long>)
      .def("readULong", &ext::read_eigen_raw_attr<unsigned long>)
      .def("readLLong", &ext::read_eigen_raw_attr<long long>)
      .def("readULLong", &ext::read_eigen_raw_attr<unsigned long long>)
      .def("readFloat", &ext::read_eigen_raw_attr<float>)
      .def("readDouble", &ext::read_eigen_raw_attr<double>)
      .def("readCFloat", &ext::read_eigen_raw_attr<std::complex<float>>)
      .def("readCDouble", &ext::read_eigen_raw_attr<std::complex<double>>)
      .def("readStr", &ext::read_vector_raw_attr<const char*>);
}


} // h5gtpy
