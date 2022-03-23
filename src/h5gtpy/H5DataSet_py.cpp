#include "../../include/h5gtpy/H5DataSet_py.h"

namespace h5gtpy {

void DataSet_py(py::class_<DataSet, Object, SliceTraits<DataSet>, AnnotateTraits<DataSet> > &py_obj){
  py_obj
      .def("getStorageSize", &DataSet::getStorageSize,
           "returns the amount of storage allocated for a dataset")
      .def("getOffset", &DataSet::getOffset,
           "returns DataSet address in file")
      .def("getDataType", &DataSet::getDataType,
           "return the datatype associated with this dataset")
      .def("getLinkInfo", &DataSet::getLinkInfo,
           "return the link info object")
      .def("getSpace", &DataSet::getSpace,
           "return the dataspace associated with this dataset")
      .def("getMemSpace", &DataSet::getMemSpace,
           "same as getSpace for DataSet, compatibility with Selection")
      .def("unpackSoftLink", &DataSet::unpackSoftLink,
           "For soft link that returns path to target that"
"link points to. Otherwise it works the same way as `getPath()`")
      .def("unlink", &DataSet::unlink)
      .def("rename", &DataSet::rename,
           py::arg("destPath"),
           py::arg_v("linkCreateProps", LinkCreateProps(), "LinkCreateProps()"),
           py::arg_v("linkAccessProps", LinkAccessProps(), "LinkAccessProps()"))
      .def("getParent", &DataSet::getParent,
           py::arg_v("groupAccessProps", GroupAccessProps(), "GroupAccessProps()"))
      .def("getCreateProps", &DataSet::getCreateProps)
      .def("getAccessProps", &DataSet::getAccessProps)
      .def("resize", &DataSet::resize,
           py::arg("dims"),
           "This requires that the dataset was created with chunking, and you would"
"generally want to have set a larger maxdims setting")
      .def("getDimensions", &DataSet::getDimensions,
           "Get the dimensions of the whole DataSet."
"This is a shorthand for getSpace().getDimensions()")
      .def("getElementCount", &DataSet::getElementCount,
           "Get the total number of elements in the current dataset."
"E.g. 2x2x2 matrix has size 8. This is a shorthand for getSpace().getTotalCount()")
      .def_static("FromId", &DataSet::FromId,
                  py::arg("id"),
                  py::arg_v("increaseRefCount", false, "false"),
                  "Create new object from ID")
      .def(py::self == py::self)
      .def(py::self != py::self);
}


} // h5gtpy
