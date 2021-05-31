#include "../../include/h5gtpy/H5SliceTraits_py.h"

namespace h5gtpy {

void ElementSet_py(py::class_<ElementSet> &py_obj){
  py_obj
      .def(py::init<const std::vector<std::size_t>& >())
      .def(py::init<const std::vector<std::vector<std::size_t> >& >());
}


} // h5gtpy
