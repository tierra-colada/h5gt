#include "h5gt_py.h"

namespace h5gtpy {

void DataSet_py(py::class_<DataSet, Object, SliceTraits<DataSet>, AnnotateTraits<DataSet> > &py_obj);


} // h5gtpy
