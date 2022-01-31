#include "h5gt_py.h"

namespace h5gtpy {

template <PropertyType T>
void PropertyList_py(py::class_<PropertyList<T> >& py_obj) {
  py_obj
      .def("getObjectType", &PropertyList<T>::getObjectType)
      .def("getId", &PropertyList<T>::getId,
           py::arg_v("increaseRefCount", false, "false"));
}

void LinkCreateProps_py(py::class_<LinkCreateProps, PropertyList<PropertyType::LINK_CREATE> > &py_obj);
void LinkAccessProps_py(py::class_<LinkAccessProps, PropertyList<PropertyType::LINK_ACCESS> > &py_obj);
void FileCreateProps_py(py::class_<FileCreateProps, PropertyList<PropertyType::FILE_CREATE> > &py_obj);
void FileAccessProps_py(py::class_<FileAccessProps, PropertyList<PropertyType::FILE_ACCESS> > &py_obj);
void GroupCreateProps_py(py::class_<GroupCreateProps, PropertyList<PropertyType::GROUP_CREATE> > &py_obj);
void GroupAccessProps_py(py::class_<GroupAccessProps, PropertyList<PropertyType::GROUP_ACCESS> > &py_obj);
void DataSetCreateProps_py(py::class_<DataSetCreateProps, PropertyList<PropertyType::DATASET_CREATE> > &py_obj);
void DataSetAccessProps_py(py::class_<DataSetAccessProps, PropertyList<PropertyType::DATASET_ACCESS> > &py_obj);
void DataTypeCreateProps_py(py::class_<DataTypeCreateProps, PropertyList<PropertyType::DATATYPE_CREATE> > &py_obj);
void DataTypeAccessProps_py(py::class_<DataTypeAccessProps, PropertyList<PropertyType::DATATYPE_ACCESS> > &py_obj);
void ObjectCopyProps_py(py::class_<ObjectCopyProps, PropertyList<PropertyType::OBJECT_COPY> > &py_obj);


} // h5gtpy
