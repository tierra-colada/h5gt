#include "../../include/h5gtpy/H5Selection_py.h"

namespace h5gtpy {

namespace ext {

DataSet& getDataset_selection_wrap(Selection& self) noexcept{
  return self.getDataset();
}


} // ext


void Selection_py(py::class_<Selection, SliceTraits<Selection> > &py_obj){
  py_obj
      .def(py::init<const DataSpace&>())
      .def("getSpace", &Selection::getSpace,
           "Dataspace associated with this selection")
      .def("getMemSpace", &Selection::getMemSpace,
           "Dataspace associated with the memory representation of this selection")
      .def("getDataset", &ext::getDataset_selection_wrap,
           "parent dataset of this selection")
      .def("getDataType", &Selection::getDataType,
           "return the datatype of the selection");
}


} // h5gtpy
