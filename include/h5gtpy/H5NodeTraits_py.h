#include "h5gtpy.h"

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
DataSet createLink_wrap(
    NodeTraits<Derivate>& self,
    const DataSet& target,
    const std::string& linkName,
    const LinkType& linkType,
    const LinkCreateProps& linkCreateProps = LinkCreateProps(),
    const LinkAccessProps& linkAccessProps = LinkAccessProps(),
    const DataSetAccessProps& dsetAccessProps = DataSetAccessProps()){
  return self.createLink(target, linkName, linkType, linkCreateProps, linkAccessProps, dsetAccessProps);
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
           "Check a dataset or group exists in the current node / group")
      .def("hasObject", &NodeTraits<Derivate>::hasObject,
           py::arg("objName"),
           py::arg("objectType"),
           py::arg_v("linkAccessProps", LinkAccessProps(), "LinkAccessProps()"),
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
      .def("createLink", py::overload_cast<
           const File&,
           const std::string&,
           const LinkType&,
           const LinkCreateProps&,
           const LinkAccessProps&,
           const GroupAccessProps&>(&NodeTraits<Derivate>::template createLink<File>),
           py::arg("file"),// "target object",
           py::arg("linkName"),// "name for a new link",
           py::arg("linkType"),
           py::arg_v("linkCreateProps", LinkCreateProps(), "LinkCreateProps()"),
           py::arg_v("linkAccesProps", LinkAccessProps(), "LinkAccessProps()"),
           py::arg_v("groupAccessProps", GroupAccessProps(), "GroupAccessProps()"),
           "Creates link to a `File`")
      .def("createLink", py::overload_cast<
           const Group&,
           const std::string&,
           const LinkType&,
           const LinkCreateProps&,
           const LinkAccessProps&,
           const GroupAccessProps&>(&NodeTraits<Derivate>::template createLink<Group>),
           py::arg("group"),// "target object",
           py::arg("linkName"),// "name for a new link",
           py::arg("linkType"),
           py::arg_v("linkCreateProps", LinkCreateProps(), "LinkCreateProps()"),
           py::arg_v("linkAccesProps", LinkAccessProps(), "LinkAccessProps()"),
           py::arg_v("groupAccessProps", GroupAccessProps(), "GroupAccessProps()"),
           "Creates link to a `Group`")
      .def("createLink", &ext::createLink_wrap<Derivate>,
           py::arg("dset"),// "target object",
           py::arg("linkName"),// "name for a new link",
           py::arg("linkType"),
           py::arg_v("linkCreateProps", LinkCreateProps(), "LinkCreateProps()"),
           py::arg_v("linkAccesProps", LinkAccessProps(), "LinkAccessProps()"),
           py::arg_v("dsetAccessProps", DataSetAccessProps(), "DataSetAccessProps()"),
           "Creates link to a `DataSet`");
}


} // h5gtpy
