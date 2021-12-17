#include "../../include/h5gtpy/H5DataType_py.h"

namespace h5gtpy {

void DataType_py(py::class_<DataType, Object>& py_obj){
  py_obj
      .def(py::init<>())
      .def("isTypeEqual", &DataType::isTypeEqual,
           "isTypeEqual Unlike `==` operator this only checks if"
"the data types are equal and do not check if they belong"
"to the same h5 file")
      .def("getClass", &DataType::getClass,
           "the fundamental type.")
      .def("getSize", &DataType::getSize,
           "Notice that the size of variable length sequences may have limited applicability"
"given that it refers to the size of the control structure. For info see"
"https://support.hdfgroup.org/HDF5/doc/RM/RM_H5T.html#Datatype-GetSize")
      .def("string", &DataType::string,
           "Returns a friendly description of the type (e.g. Float32)")
      .def("isVariableStr", &DataType::isVariableStr,
           "Returns whether the type is a variable-length string")
      .def("isFixedLenStr", &DataType::isFixedLenStr,
           "Returns whether the type is a fixed-length string")
      .def("empty", &DataType::empty,
           "Check the DataType was default constructed. Such value might represent auto-detection of the datatype from a buffer")
      .def("isReference", &DataType::isReference,
           "Returns whether the type is a Reference")
      .def_static("FromId", &DataType::FromId,
                  py::arg("id"),
                  py::arg_v("increaseRefCount", false, "false"),
                  "Create new object from ID")
      .def(py::self == py::self)
      .def(py::self != py::self);
}

void CompoundMember_py(py::class_<CompoundType::member_def>& py_obj) {
  py_obj
      .def(py::init<std::string, DataType, size_t>(),
           py::arg("t_name"),
           py::arg("baseType"),
           py::arg_v("offset", 0, "0"))
      .def_readwrite("name", &CompoundType::member_def::name)
      .def_readwrite("base_type", &CompoundType::member_def::base_type)
      .def_readwrite("offset", &CompoundType::member_def::offset);
}

void CompoundType_py(py::class_<CompoundType, DataType>& py_obj) {
  py_obj
      .def(py::init<const CompoundType&>())
      .def(py::init<const std::vector<CompoundType::member_def>&, size_t>(),
           py::arg("members"),
           py::arg_v("size", 0, "0"))
      .def("commit", &CompoundType::commit,
           py::arg("object"),
           py::arg("name"))
      .def("getMembers", &CompoundType::getMembers);
}


} // h5gtpy
