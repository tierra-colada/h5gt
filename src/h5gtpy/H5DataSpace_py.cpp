#include "../../include/h5gtpy/H5DataSpace_py.h"

namespace h5gtpy {

void DataSpace_py(py::class_<DataSpace, Object> &py_obj){
  py_obj
      .def(py::init<const std::vector<size_t>&>(),
           py::arg("dims"),
           "Each element in vector `dims` defines new dimension: size(dim1) = vec[0], size(dim2) = vec[1], etc..")
      .def(py::init<const std::vector<size_t>&,
           const std::vector<size_t>&>(),
           py::arg("dims"),
           py::arg("maxdims"),
           "Create a resizable N-dimensional dataspace")
      .def("clone", &DataSpace::clone,
           "Create a new DataSpace with a different id available for modifications")
      .def("getNumberDimensions", &DataSpace::getNumberDimensions,
           "the number of dimensions in the current dataspace")
      .def("getDimensions", &DataSpace::getDimensions,
           "return a vector of N-element, each element is the size of the associated dataset dimension")
      .def("getElementCount", &DataSpace::getElementCount,
           "the total number of elements in the dataspace")
      .def("getMaxDimensions", &DataSpace::getMaxDimensions,
           "return a vector of N-element, each element is the size of the associated dataset maximum dimension")
      .def_static("FromId", &DataSpace::FromId,
                  py::arg("id"),
                  py::arg_v("increaseRefCount", false, "false"),
                  "Create new object from ID");
}



} // h5gtpy
