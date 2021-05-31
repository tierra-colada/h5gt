#ifndef H5GTPY_H
#define H5GTPY_H

#ifndef H5GT_USE_EIGEN
#define H5GT_USE_EIGEN // should be defined before including h5gt
#endif

#include <h5gt/H5Object.hpp>
#include <h5gt/H5File.hpp>
#include <h5gt/H5DataSet.hpp>
#include <h5gt/H5DataSpace.hpp>

#include <Eigen/Dense>

#include <pybind11/pybind11.h>
#include <pybind11/eigen.h>
#include <pybind11/stl.h>
//#include <pybind11/stl_bind.h>
#include <pybind11/complex.h>
#include <pybind11/operators.h>

using namespace h5gt;

namespace py = pybind11;

/* to generate .pyi the returned type should be declared before it
 * is called. For example `createGroup` returns `Group` so I need to
 * place `py::class_<Group, Object>(m, "Group")`
 * before calling `.def("createGroup", ...` ) */

/* py::arithmetic() -> create an enumeration that also supports
 * rudimentary arithmetic and bit-level operations like
 * comparisons, and, or, xor, negation, etc. */

/* export_values() -> export the enum entries into the parent scope,
 * which should be skipped for newer C++11-style strongly typed enums */

/* py::const_ -> is neccessary when binding overoaded functions */

/* All declarations are here. And all functions are invoked in `h5gt.cpp` */

namespace h5gtpy {

  // ENUM -> H5Enum_py.cpp
//   void ObjectType_py(py::enum_<ObjectType> &py_obj);
//   void LinkType_py(py::enum_<LinkType> &py_obj);
//   void PropertyType_py(py::enum_<PropertyType> &py_obj);
//   void DataTypeClass_py(py::enum_<DataTypeClass> &py_obj);
//   void OpenFlag_py(py::enum_<File::OpenFlag> &py_obj);

   // PROPERTYLIST -> H5PropertyList_py.cpp
//   template <PropertyType T>
//   void PropertyList_py(py::class_<PropertyList<T> >& py_obj);
//   void LinkCreateProps_py(py::class_<LinkCreateProps, PropertyList<PropertyType::LINK_CREATE> > &py_obj);
//   void LinkAccessProps_py(py::class_<LinkAccessProps, PropertyList<PropertyType::LINK_ACCESS> > &py_obj);
//   void GroupCreateProps_py(py::class_<GroupCreateProps, PropertyList<PropertyType::GROUP_CREATE> > &py_obj);
//   void GroupAccessProps_py(py::class_<GroupAccessProps, PropertyList<PropertyType::GROUP_ACCESS> > &py_obj);
//   void DataSetCreateProps_py(py::class_<DataSetCreateProps, PropertyList<PropertyType::DATASET_CREATE> > &py_obj);
//   void DataSetAccessProps_py(py::class_<DataSetAccessProps, PropertyList<PropertyType::DATASET_ACCESS> > &py_obj);
//   void DataTypeCreateProps_py(py::class_<DataTypeCreateProps, PropertyList<PropertyType::DATATYPE_CREATE> > &py_obj);
//   void DataTypeAccessProps_py(py::class_<DataTypeAccessProps, PropertyList<PropertyType::DATATYPE_ACCESS> > &py_obj);

   // OBJECT -> H5Object_py.cpp
//   void ObjectInfo_py(py::class_<ObjectInfo> &py_obj);
//   void LinkInfo_py(py::class_<LinkInfo> &py_obj);
//   void Object_py(py::class_<Object> &py_obj);

   // SLICE -> H5SliceTraits_py.cpp
//   void ElementSet_py(py::class_<ElementSet> &py_obj);
//   template <typename Derivate>
//   void SliceTraits_py(py::class_<SliceTraits<Derivate> > &py_obj);

   // SELECTION -> H5Selection_py.cpp
//   void Selection_py(py::class_<Selection, SliceTraits<Selection> > &py_obj);

  // DATATYPE -> H5DataType_py.cpp
//  void DataType_py(py::class_<DataType, Object>& py_obj);
//  template <typename T>
//  void AtomicType_py(py::class_<AtomicType<T>, DataType>& py_obj);

  // DATASPACE -> H5DataSpace_py.cpp
//  void DataSpace_py(py::class_<DataSpace, Object> &py_obj);

  // NODETRAITS -> H5NodeTraits_py.cpp
//  template <typename Derivate>
//  void NodeTraits_py(py::class_<NodeTraits<Derivate> >& py_obj);

  // FILE -> H5File_py.cpp
//  void File_py(py::class_<File, Object, NodeTraits<File> > &py_obj);

  // GROUP -> H5Group_py.cpp
//  void Group_py(py::class_<Group, Object, NodeTraits<Group> > &py_obj);

  //DATASET -> H5DataSet_py.cpp
//  void DataSet_py(py::class_<DataSet, Object, SliceTraits<DataSet> > &py_obj);


} // h5gtpy


#endif // H5GTPY_H
