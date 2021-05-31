#include "../../include/h5gtpy/H5File_py.h"

namespace h5gtpy {

void File_py(py::class_<File, Object, NodeTraits<File>, AnnotateTraits<File> > &py_obj){
  py_obj
      .def(py::init<const std::string&, File::OpenFlag>(),
           py::arg("file_name"),
           py::arg("open_flag"),
           "Open or create new h5 file")
      .def("flush", &File::flush,
           "Flushes all buffers associated with a file to disk")
      .def_static("FromId", &File::FromId,
                  py::arg("id"),
                  py::arg_v("increaseRefCount", false, "false"),
                  "Create new object from ID")
      .def(py::self == py::self)
      .def(py::self != py::self);
}


} // h5gtpy
