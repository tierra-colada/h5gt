#include "../../include/h5gtpy/H5Group_py.h"

namespace h5gtpy {

void Group_py(py::class_<Group, Object, NodeTraits<Group>, AnnotateTraits<Group> > &py_obj){
  py_obj
      .def("getLinkInfo", &Group::getLinkInfo,
           "returns link info object")
      .def("unpackSoftLink", &Group::unpackSoftLink,
           "For soft link that returns path to target that"
"link points to. Otherwise it works the same way as `getPath()`")
      .def("getParent", &Group::getParent,
           py::arg_v("groupAccessProps", GroupAccessProps(), "GroupAccessProps()"),
           "returns parent `Group`")
      .def_static("FromId", &Group::FromId,
                  py::arg("id"),
                  py::arg_v("increaseRefCount", false, "false"),
                  "Create new object from ID")
      .def(py::self == py::self, "Check if objects reside in the same file and equal to each other")
      .def(py::self != py::self);
}


} // h5gtpy
