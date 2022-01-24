#include "../../include/h5gtpy/H5Object_py.h"

namespace h5gtpy {

void ObjectInfo_py(py::class_<ObjectInfo> &py_obj){
  py_obj
  #if (H5Oget_info_vers < 3)
      .def("getAddress", &ObjectInfo::getAddress)
  #else
      .def("getHardLinkToken", &ObjectInfo::getHardLinkToken)
  #endif
      .def("getFileNumber", &ObjectInfo::getFileNumber)
      .def("getHardLinkRefCount", &ObjectInfo::getHardLinkRefCount)
      .def("getAccessTime", &ObjectInfo::getAccessTime)
      .def("getModificationTime", &ObjectInfo::getModificationTime)
      .def("getChangeTime", &ObjectInfo::getChangeTime)
      .def("getCreationTime", &ObjectInfo::getCreationTime)
      .def("getNumAttr", &ObjectInfo::getNumAttr);

}

void LinkInfo_py(py::class_<LinkInfo> &py_obj){
  py_obj
      .def("getLinkType", &LinkInfo::getLinkType)
      .def("creationOrderValid", &LinkInfo::creationOrderValid)
      .def("getCreationOrder", &LinkInfo::getCreationOrder)
      .def("getLinkNameCharacterSet", &LinkInfo::getLinkNameCharacterSet)
  #if (H5Lget_info_vers < 2)
      .def("getAddress", &LinkInfo::getAddress)
  #else
      .def("getHardLinkToken", &LinkInfo::getHardLinkToken)
  #endif
      .def("getSoftLinkSize", &LinkInfo::getSoftLinkSize);
}

void Object_py(py::class_<Object> &py_obj){
  py_obj
      .def("flush", &Object::flush)
      .def("isValid", &Object::isValid)
      .def("refresh", &Object::refresh,
           "Usually you don't need to use it.")
      .def("getId", &Object::getId,
           py::arg_v("increaseRefCount", false, "false"))
      .def("getFileId", &Object::getFileId,
           py::arg_v("increaseRefCount", false, "false"))
      .def("getFileName", &Object::getFileName)
      .def("getPath", &Object::getPath)
      .def("getIdRefCount", &Object::getIdRefCount)
      .def("getObjectInfo", &Object::getObjectInfo)
      .def("getObjectType", &Object::getObjectType);
}


} // h5gtpy
