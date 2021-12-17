#include "h5gt_py.h"

namespace h5gtpy {

void DataType_py(py::class_<DataType, Object>& py_obj);

template <typename T>
void AtomicType_py(py::class_<AtomicType<T>, DataType>& py_obj) {
  py_obj
      .def(py::init<>());
}

void CompoundMember_py(py::class_<CompoundType::member_def>& py_obj);
void CompoundType_py(py::class_<CompoundType, DataType>& py_obj);

template<typename T>
void EnumMember_py(py::class_<typename EnumType<T>::member_def>& py_obj) {
  py_obj
      .def(py::init<const std::string&, T>(),
           py::arg("t_name"),
           py::arg("t_value"))
      .def_readwrite("name", &EnumType<T>::member_def::name)
      .def_readwrite("value", &EnumType<T>::member_def::value);
}

template<typename T>
void EnumType_py(py::class_<EnumType<T>, DataType>& py_obj) {
  py_obj
      .def(py::init<const EnumType<T>&>())
      .def(py::init<const std::vector<typename EnumType<T>::member_def>&>(),
           py::arg("members"))
      .def("commit", &EnumType<T>::commit,
           py::arg("object"),
           py::arg("name"));
}


} // h5gtpy
