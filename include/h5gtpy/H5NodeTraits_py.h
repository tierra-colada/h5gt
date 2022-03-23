#include "h5gt_py.h"

namespace h5gtpy {

namespace ext {

template <typename Derivate>
DataSet createDataSet_wrap1(
    NodeTraits<Derivate>& self,
    const std::string& dset_name,
    const DataSpace& space,
    const DataType& type,
    const LinkCreateProps& linkCreateProps = LinkCreateProps(),
    const DataSetCreateProps& dsetCreateProps = DataSetCreateProps(),
    const DataSetAccessProps& dsetAccessProps = DataSetAccessProps()){
  return self.createDataSet(dset_name, space, type, linkCreateProps, dsetCreateProps, dsetAccessProps);
}

template <typename Derivate>
DataSet createDataSet_wrap2(
    NodeTraits<Derivate>& self,
    const std::string& dset_name,
    const std::vector<size_t>& dims,
    const DataType& type,
    const LinkCreateProps& linkCreateProps = LinkCreateProps(),
    const DataSetCreateProps& dsetCreateProps = DataSetCreateProps(),
    const DataSetAccessProps& dsetAccessProps = DataSetAccessProps()){
  return self.createDataSet(dset_name, DataSpace(dims), type, linkCreateProps, dsetCreateProps, dsetAccessProps);
}

template <typename Derivate>
DataSet createDataSet_wrap3(
    NodeTraits<Derivate>& self,
    const std::string& dset_name,
    const size_t& dim,
    const DataType& type,
    const LinkCreateProps& linkCreateProps = LinkCreateProps(),
    const DataSetCreateProps& dsetCreateProps = DataSetCreateProps(),
    const DataSetAccessProps& dsetAccessProps = DataSetAccessProps()){
  return self.createDataSet(dset_name, DataSpace(dim), type, linkCreateProps, dsetCreateProps, dsetAccessProps);
}

template <typename Derivate>
Group copy_wrap1(
    NodeTraits<Derivate>& self,
    const Group& obj, const std::string& objNewName,
    const ObjectCopyProps& copyProps = ObjectCopyProps(),
    const LinkCreateProps& linkCreateProps = LinkCreateProps(),
    const GroupAccessProps& groupAccessProps = GroupAccessProps()){
  return self.copy(obj, objNewName, copyProps, linkCreateProps, groupAccessProps);
}

template <typename Derivate>
DataSet copy_wrap2(
    NodeTraits<Derivate>& self,
    const DataSet& obj, const std::string& objNewName,
    const ObjectCopyProps& copyProps = ObjectCopyProps(),
    const LinkCreateProps& linkCreateProps = LinkCreateProps(),
    const DataSetAccessProps& dsetAccessProps = DataSetAccessProps()){
  return self.copy(obj, objNewName, copyProps, linkCreateProps, dsetAccessProps);
}

template <typename Derivate>
DataType copy_wrap3(
    NodeTraits<Derivate>& self,
    const DataType& obj, const std::string& objNewName,
    const ObjectCopyProps& copyProps = ObjectCopyProps(),
    const LinkCreateProps& linkCreateProps = LinkCreateProps(),
    const DataTypeAccessProps& dtypeAccessProps = DataTypeAccessProps()){
  return self.copy(obj, objNewName, copyProps, linkCreateProps, dtypeAccessProps);
}

template <typename Derivate>
DataSet createLink_wrap1(
    NodeTraits<Derivate>& self,
    const DataSet& target,
    const std::string& linkName,
    const LinkType& linkType,
    const std::string& targetPath = "",
    const LinkCreateProps& linkCreateProps = LinkCreateProps(),
    const LinkAccessProps& linkAccessProps = LinkAccessProps(),
    const DataSetAccessProps& dsetAccessProps = DataSetAccessProps()){
  return self.createLink(target, linkName, linkType, targetPath, linkCreateProps, linkAccessProps, dsetAccessProps);
}

template <typename Derivate>
DataType createLink_wrap2(
    NodeTraits<Derivate>& self,
    const DataType& target,
    const std::string& linkName,
    const LinkType& linkType,
    const std::string& targetPath = "",
    const LinkCreateProps& linkCreateProps = LinkCreateProps(),
    const LinkAccessProps& linkAccessProps = LinkAccessProps(),
    const DataTypeAccessProps& dtypeAccessProps = DataTypeAccessProps()){
  return self.createLink(target, linkName, linkType, targetPath, linkCreateProps, linkAccessProps, dtypeAccessProps);
}

template <typename Derivate>
std::tuple<std::string, std::string> unpackExternalLink(
    NodeTraits<Derivate>& self,
    const std::string& objName){
  std::string fileName_out;
  std::string objName_out = self.unpackExternalLink(objName, fileName_out);
  return std::make_tuple(fileName_out, objName_out);
}


} // ext


template <typename Derivate>
void NodeTraits_py(py::class_<NodeTraits<Derivate> >& py_obj) {
  py_obj
      .def("createDataSet", &ext::createDataSet_wrap1<Derivate>,
           py::arg("dset_name"),
           py::arg("space"),
           py::arg("type"),
           py::arg_v("linkCreateProps", LinkCreateProps(), "LinkCreateProps()"),
           py::arg_v("dsetCreateProps", DataSetCreateProps(), "DataSetCreateProps()"),
           py::arg_v("dsetAccessProps", DataSetAccessProps(), "DataSetAccessProps()"))
      .def("createDataSet", &ext::createDataSet_wrap2<Derivate>,
           py::arg("dset_name"),
           py::arg("dims"),
           py::arg("type"),
           py::arg_v("linkCreateProps", LinkCreateProps(), "LinkCreateProps()"),
           py::arg_v("dsetCreateProps", DataSetCreateProps(), "DataSetCreateProps()"),
           py::arg_v("dsetAccessProps", DataSetAccessProps(), "DataSetAccessProps()"))
      .def("createDataSet", &ext::createDataSet_wrap3<Derivate>,
           py::arg("dset_name"),
           py::arg("dim"),
           py::arg("type"),
           py::arg_v("linkCreateProps", LinkCreateProps(), "LinkCreateProps()"),
           py::arg_v("dsetCreateProps", DataSetCreateProps(), "DataSetCreateProps()"),
           py::arg_v("dsetAccessProps", DataSetAccessProps(), "DataSetAccessProps()"))
      .def("getDataType", &NodeTraits<Derivate>::getDataType,
           py::arg("dtype_name"),
           py::arg_v("dtypeAccessProps", DataTypeAccessProps(), "DataTypeAccessProps()"))
      .def("getDataSet", &NodeTraits<Derivate>::getDataSet,
           py::arg("dset_name"),
           py::arg_v("dsetAccessProps", DataSetAccessProps(), "DataSetAccessProps()"),
           "Get an existing dataset in the current file")
      .def("createGroup", &NodeTraits<Derivate>::createGroup,
           py::arg("group_name"),
           py::arg_v("linkCreateProps", LinkCreateProps(), "LinkCreateProps()"),
           py::arg_v("groupCreateProps", GroupCreateProps(), "GroupCreateProps()"),
           py::arg_v("groupAccessProps", GroupAccessProps(), "GroupAccessProps()"),
           "Create a new group, and eventually intermediate groups")
      .def("getGroup", &NodeTraits<Derivate>::getGroup,
           py::arg("group_name"),
           py::arg_v("groupAccessProps", GroupAccessProps(), "GroupAccessProps()"),
           "return the number of leaf objects of the node / group")
      .def("getNumberObjects", &NodeTraits<Derivate>::getNumberObjects)
      .def("getObjectName", &NodeTraits<Derivate>::getObjectName,
           py::arg("index"),
           py::arg_v("linkAccessProps", LinkAccessProps(), "LinkAccessProps()"))
      .def("getLinkInfo", &NodeTraits<Derivate>::getLinkInfo,
           py::arg("objName"))
      .def("unpackSoftLink", &NodeTraits<Derivate>::unpackSoftLink,
           py::arg("objName"),
           "Return target object name")
      .def("unpackExternalLink", &ext::unpackExternalLink<Derivate>,
           py::arg("objName"),
           "Return target file name and object name")
      .def("rename", &NodeTraits<Derivate>::rename,
           py::arg("src_path"),
           py::arg("dest_path"),
           py::arg_v("linkCreateProps", LinkCreateProps(), "LinkCreateProps()"),
           py::arg_v("linkAccessProps", LinkAccessProps(), "LinkAccessProps()"),
           "Moves an object and its content within an HDF5 file.")
      .def("listObjectNames", &NodeTraits<Derivate>::listObjectNames)
      .def("exist", &NodeTraits<Derivate>::exist,
           py::arg("obj_name"),
           py::arg_v("linkAccessProps", LinkAccessProps(), "LinkAccessProps()"),
           py::arg_v("raise_errors", false, "False"),
           "Check a dataset or group exists in the current node / group")
      .def("resolved", &NodeTraits<Derivate>::resolved,
           py::arg("obj_name"),
           py::arg_v("linkAccessProps", LinkAccessProps(), "LinkAccessProps()"),
           py::arg_v("raise_errors", false, "False"),
           "Check a dataset or group in the current node / group may be resolved or not")
      .def("existAndResolved", &NodeTraits<Derivate>::existAndResolved,
           py::arg("obj_name"),
           py::arg_v("linkAccessProps", LinkAccessProps(), "LinkAccessProps()"),
           py::arg_v("raise_errors", false, "False"),
           "Call 'exist()' with subsequent call to 'resolved()' if successful")
      .def("hasObject", &NodeTraits<Derivate>::hasObject,
           py::arg("objName"),
           py::arg("objectType"),
           py::arg_v("linkAccessProps", LinkAccessProps(), "LinkAccessProps()"),
           py::arg_v("raise_errors", false, "False"),
           "Advanced version of `exist` that also checks the type of object")
      .def("unlink", &NodeTraits<Derivate>::unlink,
           py::arg("obj_name"),
           py::arg_v("linkAccessProps", LinkAccessProps(), "LinkAccessProps()"))
      .def("getLinkType", &NodeTraits<Derivate>::getLinkType,
           py::arg("obj_name"),
           py::arg_v("linkAccessProps", LinkAccessProps(), "LinkAccessProps()"),
           "Unlink the given dataset or group. NOTE: unlink doesn't frees memory. Use `h5repack` to free unused space")
      .def("getObjectType", &NodeTraits<Derivate>::getObjectType,
           py::arg("obj_name"),
           py::arg_v("linkAccessProps", LinkAccessProps(), "LinkAccessProps()"),
           "A shorthand to get the kind of object pointed to (group, dataset, type...)")
      .def("copy", &ext::copy_wrap1<Derivate>,
           py::arg("group"),
           py::arg("objNewName"),
           py::arg_v("copyProps", ObjectCopyProps(), "ObjectCopyProps()"),
           py::arg_v("linkCreateProps", LinkCreateProps(), "LinkCreateProps()"),
           py::arg_v("groupAccessProps", LinkAccessProps(), "LinkAccessProps()"))
      .def("copy", &ext::copy_wrap2<Derivate>,
           py::arg("dset"),
           py::arg("objNewName"),
           py::arg_v("copyProps", ObjectCopyProps(), "ObjectCopyProps()"),
           py::arg_v("linkCreateProps", LinkCreateProps(), "LinkCreateProps()"),
           py::arg_v("dsetAccessProps", DataSetAccessProps(), "DataSetAccessProps()"))
      .def("copy", &ext::copy_wrap3<Derivate>,
           py::arg("dtype"),
           py::arg("objNewName"),
           py::arg_v("copyProps", ObjectCopyProps(), "ObjectCopyProps()"),
           py::arg_v("linkCreateProps", LinkCreateProps(), "LinkCreateProps()"),
           py::arg_v("dtypeAccessProps", DataTypeAccessProps(), "DataTypeAccessProps()"))
      .def("createLink", py::overload_cast<
           const File&,
           const std::string&,
           const LinkType&,
           const std::string&,
           const LinkCreateProps&,
           const LinkAccessProps&,
           const GroupAccessProps&>(&NodeTraits<Derivate>::template createLink<File>),
           py::arg("file"),// "target object",
           py::arg("linkName"),// "name for a new link",
           py::arg("linkType"),
           py::arg_v("targetPath", std::string(), "str()"),
           py::arg_v("linkCreateProps", LinkCreateProps(), "LinkCreateProps()"),
           py::arg_v("linkAccesProps", LinkAccessProps(), "LinkAccessProps()"),
           py::arg_v("groupAccessProps", GroupAccessProps(), "GroupAccessProps()"),
           "Creates link to a `File`")
      .def("createLink", py::overload_cast<
           const Group&,
           const std::string&,
           const LinkType&,
           const std::string&,
           const LinkCreateProps&,
           const LinkAccessProps&,
           const GroupAccessProps&>(&NodeTraits<Derivate>::template createLink<Group>),
           py::arg("group"),// "target object",
           py::arg("linkName"),// "name for a new link",
           py::arg("linkType"),
           py::arg_v("targetPath", std::string(), "str()"),
           py::arg_v("linkCreateProps", LinkCreateProps(), "LinkCreateProps()"),
           py::arg_v("linkAccesProps", LinkAccessProps(), "LinkAccessProps()"),
           py::arg_v("groupAccessProps", GroupAccessProps(), "GroupAccessProps()"),
           "Creates link to a `Group`")
      .def("createLink", &ext::createLink_wrap1<Derivate>,
           py::arg("dset"),// "target object",
           py::arg("linkName"),// "name for a new link",
           py::arg("linkType"),
           py::arg_v("targetPath", std::string(), "str()"),
           py::arg_v("linkCreateProps", LinkCreateProps(), "LinkCreateProps()"),
           py::arg_v("linkAccesProps", LinkAccessProps(), "LinkAccessProps()"),
           py::arg_v("dsetAccessProps", DataSetAccessProps(), "DataSetAccessProps()"),
           "Creates link to a `DataSet`")
      .def("createLink", &ext::createLink_wrap2<Derivate>,
           py::arg("dtype"),// "target object",
           py::arg("linkName"),// "name for a new link",
           py::arg("linkType"),
           py::arg_v("targetPath", std::string(), "str()"),
           py::arg_v("linkCreateProps", LinkCreateProps(), "LinkCreateProps()"),
           py::arg_v("linkAccesProps", LinkAccessProps(), "LinkAccessProps()"),
           py::arg_v("dtypeAccessProps", DataTypeAccessProps(), "DataTypeAccessProps()"),
           "Creates link to a `DataType`");
}


} // h5gtpy
