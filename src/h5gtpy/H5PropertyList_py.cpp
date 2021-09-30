#include "../../include/h5gtpy/H5PropertyList_py.h"

namespace h5gtpy {

namespace ext {

void setChunk_wrap1(
    DataSetCreateProps& self,
    const std::vector<hsize_t>& dims){
  self.setChunk(dims);
}

void setChunk_wrap2(
    DataSetCreateProps& self,
    const hsize_t& dim){
  self.setChunk(dim);
}

std::tuple<size_t, size_t, double> getChunkCache(
    DataSetAccessProps& self){
  size_t numSlots;
  size_t cacheSize;
  double w0;
  self.getChunkCache(numSlots, cacheSize, w0);
  return std::make_tuple(std::move(numSlots), std::move(cacheSize), std::move(w0));
}

} // ext


void LinkCreateProps_py(py::class_<LinkCreateProps, PropertyList<PropertyType::LINK_CREATE> > &py_obj){
  py_obj
      .def(py::init<>())
      .def("setCreateIntermediateGroup", &LinkCreateProps::setCreateIntermediateGroup);
}

void LinkAccessProps_py(py::class_<LinkAccessProps, PropertyList<PropertyType::LINK_ACCESS> > &py_obj){
  py_obj
      .def(py::init<>())
      .def("setExternalLinkPrefix", &LinkAccessProps::setExternalLinkPrefix,
           py::arg("prefix"));
}

void FileCreateProps_py(py::class_<FileCreateProps, PropertyList<PropertyType::FILE_CREATE> > &py_obj){
  py_obj
      .def(py::init<>());
}

void FileAccessProps_py(py::class_<FileAccessProps, PropertyList<PropertyType::FILE_ACCESS> > &py_obj){
  py_obj
      .def(py::init<>());
}

void GroupCreateProps_py(py::class_<GroupCreateProps, PropertyList<PropertyType::GROUP_CREATE> > &py_obj){
  py_obj
      .def(py::init<>());
}

void GroupAccessProps_py(py::class_<GroupAccessProps, PropertyList<PropertyType::GROUP_ACCESS> > &py_obj){
  py_obj
      .def(py::init<>())
      .def("setExternalLinkPrefix", &GroupAccessProps::setExternalLinkPrefix,
           py::arg("prefix"));
}

void DataSetCreateProps_py(py::class_<DataSetCreateProps, PropertyList<PropertyType::DATASET_CREATE> > &py_obj){
  py_obj
      .def(py::init<>())
      .def_static("FromId", &DataSetCreateProps::FromId,
                  py::arg("id"),
                  py::arg_v("increaseRefCount", false, "false"),
                  "Create new object from ID")
      .def("setShuffle", &DataSetCreateProps::setShuffle)
      .def("setChunk", &ext::setChunk_wrap1,
           py::arg("dims"))
      .def("setChunk", &ext::setChunk_wrap2,
           py::arg("dim"))
      .def("getChunk", &DataSetCreateProps::getChunk,
           py::arg("max_ndims"))
      .def("getLayoutType", &DataSetCreateProps::getLayoutType)
      .def("isCompact", &DataSetCreateProps::isCompact)
      .def("isContiguous", &DataSetCreateProps::isContiguous)
      .def("isChunked", &DataSetCreateProps::isChunked)
      .def("isVirtual", &DataSetCreateProps::isVirtual);
}

void DataSetAccessProps_py(py::class_<DataSetAccessProps, PropertyList<PropertyType::DATASET_ACCESS> > &py_obj){
  py_obj
      .def(py::init<>())
      .def_static("FromId", &DataSetAccessProps::FromId,
                  py::arg("id"),
                  py::arg_v("increaseRefCount", false, "false"),
                  "Create new object from ID")
      .def("setChunkCache", &DataSetAccessProps::setChunkCache,
           py::arg("numSlots"),
           py::arg("cacheSize"),
           py::arg_v("w0", static_cast<double>(H5D_CHUNK_CACHE_W0_DEFAULT), "static_cast<double>(H5D_CHUNK_CACHE_W0_DEFAULT)"))
      .def("setExternalLinkPrefix", &DataSetAccessProps::setExternalLinkPrefix,
           py::arg("prefix"))
      .def("getChunkCache", &ext::getChunkCache);
}

void DataTypeCreateProps_py(py::class_<DataTypeCreateProps, PropertyList<PropertyType::DATATYPE_CREATE> > &py_obj){
  py_obj
      .def(py::init<>());
}

void DataTypeAccessProps_py(py::class_<DataTypeAccessProps, PropertyList<PropertyType::DATATYPE_ACCESS> > &py_obj){
  py_obj
      .def(py::init<>());
}


} // h5gtpy
