#include "h5gtpy.h"

namespace h5gtpy {

void ObjectType_py(py::enum_<ObjectType> &py_obj);
void LinkType_py(py::enum_<LinkType> &py_obj);
void PropertyType_py(py::enum_<PropertyType> &py_obj);
void DataTypeClass_py(py::enum_<DataTypeClass> &py_obj);
void OpenFlag_py(py::enum_<File::OpenFlag> &py_obj);


} // h5gtpy
