#include "h5gt_py.h"

namespace h5gtpy {

void DataType_py(py::class_<DataType, Object>& py_obj);

template <typename T>
void AtomicType_py(py::class_<AtomicType<T>, DataType>& py_obj) {
  py_obj
      .def(py::init<>());
}


} // h5gtpy
