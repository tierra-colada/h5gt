#include "../../include/h5gtpy/H5Annotate_traits_py.h"
#include "../../include/h5gtpy/H5Attribute_py.h"
#include "../../include/h5gtpy/H5DataSet_py.h"
#include "../../include/h5gtpy/H5DataSpace_py.h"
#include "../../include/h5gtpy/H5DataType_py.h"
#include "../../include/h5gtpy/H5Enum_py.h"
#include "../../include/h5gtpy/H5File_py.h"
#include "../../include/h5gtpy/H5Group_py.h"
#include "../../include/h5gtpy/H5NodeTraits_py.h"
#include "../../include/h5gtpy/H5Object_py.h"
#include "../../include/h5gtpy/H5PropertyList_py.h"
#include "../../include/h5gtpy/H5Selection_py.h"
#include "../../include/h5gtpy/H5SliceTraits_py.h"


using py_types = std::tuple<
bool,
long long,
double,
std::complex<double>,
std::string
>;

using c_types = std::tuple<
bool,
char, signed char, unsigned char,
short, unsigned short,
int, unsigned,
long, unsigned long,
long long, unsigned long long,
float, double,
std::complex<float>, std::complex<double>,
std::string
>;

using numpy_types = std::tuple< // same as `c_types` but without `std::string`
bool,
char, signed char, unsigned char,
short, unsigned short,
int, unsigned,
long, unsigned long,
long long, unsigned long long,
float, double,
std::complex<float>, std::complex<double>
>;

using py_list_types = std::tuple<
std::vector<long long>,
std::vector<double>,
std::vector<std::complex<double>>,
std::vector<std::string>
>;

using eigen_types = std::tuple<
Eigen::MatrixX<bool>,
Eigen::MatrixX<char>, Eigen::MatrixX<signed char>, Eigen::MatrixX<unsigned char>,
Eigen::MatrixX<short>, Eigen::MatrixX<unsigned short>,
Eigen::MatrixX<int>, Eigen::MatrixX<unsigned>,
Eigen::MatrixX<long>, Eigen::MatrixX<unsigned long>,
Eigen::MatrixX<long long>, Eigen::MatrixX<unsigned long long>,
Eigen::MatrixX<float>, Eigen::MatrixX<double>,
Eigen::MatrixX<std::complex<float>>, Eigen::MatrixX<std::complex<double>>
>;

std::vector<std::string> read_eigen_names{
  "readBool",
  "readChar", "readSChar", "readUChar",
  "readShort", "readUShort",
  "readInt", "readUInt",
  "readLong", "readULong",
  "readLLong", "readULLong",
  "readFloat", "readDouble",
  "readCFloat", "readCDouble",
};

class Point {

private:
  double m_x;
  double m_y;
  double m_z;

public:
  Point()= default;
  Point(double x, double y, double z){
    m_x = x;
    m_y = y;
    m_z = z;
  }

  double getX() const{return m_x;}
  double getY() const{return m_y;}
  double getZ() const{return m_z;}
};

namespace h5gtpy {

PYBIND11_MODULE(_h5gt, m) {
  m.doc() =
      "HDF5 wrapper originally written in C++: "
  "https://github.com/tierra-colada/h5gt";


  // Simple exception class
  auto a = py::register_exception<Exception>(m, "Exception");

  /*-------------------------------------------------------------*/
  /*-------------------------DECLARATION-------------------------*/
  /*-------------------------------------------------------------*/

  // ENUM -> H5Enum_py.cpp
  auto pyObjType = py::enum_<ObjectType>(m, "ObjectType", py::arithmetic());
  auto pyLinkType = py::enum_<LinkType>(m, "LinkType", py::arithmetic());
  auto pyPropertyType = py::enum_<PropertyType>(m, "PropertyType", py::arithmetic());
  auto pyOpenFlag = py::enum_<File::OpenFlag>(m, "OpenFlag", py::arithmetic());
  auto pyEndian = py::enum_<Endian>(m, "Endian", py::arithmetic());
  auto pyDTypeClass = py::enum_<DataTypeClass>(m, "DataTypeClass", py::arithmetic());

//  auto pyException = py::class_<Exception, std::exception>(m, "Exception", py::arithmetic());
//  auto pyLinkType = py::enum_<LinkType>(m, "LinkType", py::arithmetic());
//  auto pyPropertyType = py::enum_<PropertyType>(m, "PropertyType", py::arithmetic());
//  auto pyOpenFlag = py::enum_<File::OpenFlag>(m, "OpenFlag", py::arithmetic());
//  auto pyDTypeClass = py::enum_<DataTypeClass>(m, "DataTypeClass", py::arithmetic());

  // PROPERTYLIST -> H5PropertyList_py.cpp
  auto pyLinkCreatePropsBase = py::class_<PropertyList<PropertyType::LINK_CREATE> >(m, "_LinkCreateProps");
  auto pyLinkAccessPropsBase = py::class_<PropertyList<PropertyType::LINK_ACCESS> >(m, "_LinkAccessProps");
  auto pyFileCreatePropsBase = py::class_<PropertyList<PropertyType::FILE_CREATE> >(m, "_FileCreateProps");
  auto pyFileAccessPropsBase = py::class_<PropertyList<PropertyType::FILE_ACCESS> >(m, "_FileAccessProps");
  auto pyGroupCreatePropsBase = py::class_<PropertyList<PropertyType::GROUP_CREATE> >(m, "_GroupCreateProps");
  auto pyGroupAccessPropsBase = py::class_<PropertyList<PropertyType::GROUP_ACCESS> >(m, "_GroupAccessProps");
  auto pyDataSetCreatePropsBase = py::class_<PropertyList<PropertyType::DATASET_CREATE> >(m, "_DataSetCreateProps");
  auto pyDataSetAccessPropsBase = py::class_<PropertyList<PropertyType::DATASET_ACCESS> >(m, "_DataSetAccessProps");
  auto pyDataTypeCreatePropsBase = py::class_<PropertyList<PropertyType::DATATYPE_CREATE> >(m, "_DataTypeCreateProps");
  auto pyDataTypeAccessPropsBase = py::class_<PropertyList<PropertyType::DATATYPE_ACCESS> >(m, "_DataTypeAccessProps");
  auto pyObjectCopyPropsBase = py::class_<PropertyList<PropertyType::OBJECT_COPY> >(m, "_ObjectCopyProps");

  auto pyLinkCreateProps = py::class_<LinkCreateProps, PropertyList<PropertyType::LINK_CREATE> >(m, "LinkCreateProps");
  auto pyLinkAccessProps = py::class_<LinkAccessProps, PropertyList<PropertyType::LINK_ACCESS> >(m, "LinkAccessProps");
  auto pyFileCreateProps = py::class_<FileCreateProps, PropertyList<PropertyType::FILE_CREATE> >(m, "FileCreateProps");
  auto pyFileAccessProps = py::class_<FileAccessProps, PropertyList<PropertyType::FILE_ACCESS> >(m, "FileAccessProps");
  auto pyGroupCreateProps = py::class_<GroupCreateProps, PropertyList<PropertyType::GROUP_CREATE> >(m, "GroupCreateProps");
  auto pyGroupAccessProps = py::class_<GroupAccessProps, PropertyList<PropertyType::GROUP_ACCESS> >(m, "GroupAccessProps");
  auto pyDsetCreateProps = py::class_<DataSetCreateProps, PropertyList<PropertyType::DATASET_CREATE> >(m, "DataSetCreateProps");
  auto pyDsetAccessProps = py::class_<DataSetAccessProps, PropertyList<PropertyType::DATASET_ACCESS> >(m, "DataSetAccessProps");
  auto pyDTypeCreateProps = py::class_<DataTypeCreateProps, PropertyList<PropertyType::DATATYPE_CREATE> >(m, "DataTypeCreateProps");
  auto pyDTypeAccessProps = py::class_<DataTypeAccessProps, PropertyList<PropertyType::DATATYPE_ACCESS> >(m, "DataTypeAccessProps");
  auto pyObjectCopyProps = py::class_<ObjectCopyProps, PropertyList<PropertyType::OBJECT_COPY> >(m, "ObjectCopyProps");


  // OBJECT -> H5Object_py.cpp
  auto pyObjectInfo = py::class_<ObjectInfo>(m, "ObjectInfo");
  auto pyLinkInfo = py::class_<LinkInfo>(m, "LinkInfo");
  auto pyObject = py::class_<Object>(m, "Object");

  // SLICE -> H5SliceTraits_py.cpp
  auto pyElementSet = py::class_<ElementSet>(m, "ElementSet");
  auto pySliceTraits_Selection = py::class_<SliceTraits<Selection> >(m, "_SelectionSlice");
  auto pySliceTraits_Dset = py::class_<SliceTraits<DataSet> >(m, "_DsetSlice");

  // SELECTION -> H5Selection_py.cpp
  auto pySelection = py::class_<Selection, SliceTraits<Selection> >(m, "Selection");

  // DATATYPE -> H5DataType_py.cpp
  auto pyDType = py::class_<DataType, Object>(m, "DataType");

  // atomic declarations should be invoked after `DataType_py` as
  // atomic types are children of DataType
  auto pyAtomicBool = py::class_<AtomicType<bool>, DataType>(m, "AtomicBool");
  auto pyAtomicChar = py::class_<AtomicType<char>, DataType>(m, "AtomicChar");
  auto pyAtomicSChar = py::class_<AtomicType<signed char>, DataType>(m, "AtomicSChar");
  auto pyAtomicUChar = py::class_<AtomicType<unsigned char>, DataType>(m, "AtomicUChar");
  auto pyAtomicShort = py::class_<AtomicType<short>, DataType>(m, "AtomicShort");
  auto pyAtomicUShort = py::class_<AtomicType<unsigned short>, DataType>(m, "AtomicUShort");
  auto pyAtomicInt = py::class_<AtomicType<int>, DataType>(m, "AtomicInt");
  auto pyAtomicUInt = py::class_<AtomicType<unsigned>, DataType>(m, "AtomicUInt");
  auto pyAtomicLong = py::class_<AtomicType<long>, DataType>(m, "AtomicLong");
  auto pyAtomicULong = py::class_<AtomicType<unsigned long>, DataType>(m, "AtomicULong");
  auto pyAtomicLLong = py::class_<AtomicType<long long>, DataType>(m, "AtomicLLong");
  auto pyAtomicULLong = py::class_<AtomicType<unsigned long long>, DataType>(m, "AtomicULLong");
  auto pyAtomicFloat = py::class_<AtomicType<float>, DataType>(m, "AtomicFloat");
  auto pyAtomicDouble = py::class_<AtomicType<double>, DataType>(m, "AtomicDouble");
  auto pyAtomicCFloat = py::class_<AtomicType<std::complex<float>>, DataType>(m, "AtomicCFloat");
  auto pyAtomicCDouble = py::class_<AtomicType<std::complex<double>>, DataType>(m, "AtomicCDouble");
  auto pyAtomicStr = py::class_<AtomicType<std::string>, DataType>(m, "AtomicStr");

  // CPMPOUND TYPE
  auto pyCompoundMember = py::class_<CompoundType::member_def>(m, "CompoundMember");
  auto pyCompoundType = py::class_<CompoundType, DataType>(m, "CompoundType");

  // ENUM TYPES
  auto pyEnumMemberInt = py::class_<EnumType<int>::member_def>(m, "EnumMemberInt");
  auto pyEnumMemberUInt = py::class_<EnumType<unsigned>::member_def>(m, "EnumMemberUInt");
  auto pyEnumMemberLLong = py::class_<EnumType<long long>::member_def>(m, "EnumMemberLLong");
  auto pyEnumMemberULLong = py::class_<EnumType<unsigned long long>::member_def>(m, "EnumMemberULLong");

  auto pyEnumTypeInt = py::class_<EnumType<int>, DataType>(m, "EnumTypeInt");
  auto pyEnumTypeUInt = py::class_<EnumType<unsigned>, DataType>(m, "EnumTypeUInt");
  auto pyEnumTypeLLong = py::class_<EnumType<long long>, DataType>(m, "EnumTypeLLong");
  auto pyEnumTypeULLong = py::class_<EnumType<unsigned long long>, DataType>(m, "EnumTypeULLong");

  // DATASPACE -> H5DataSpace_py.cpp
  auto pyDSpace = py::class_<DataSpace, Object>(m, "DataSpace");

  // NODETRAITS -> H5NodeTraits_py.cpp
  auto pyNodeTraits_File = py::class_<NodeTraits<File> >(m, "_FileNode"); // _Name <- underlined because user don't need to use it
  auto pyNodeTraits_Group = py::class_<NodeTraits<Group> >(m, "_GroupNode");

  // ANNOTATETRAITS -> H5Annotate_traits_py.h
  auto pyAnnotate_File = py::class_<AnnotateTraits<File> >(m, "_AnnotateFile");
  auto pyAnnotate_Group = py::class_<AnnotateTraits<Group> >(m, "_AnnotateGroup");
  auto pyAnnotate_Dset = py::class_<AnnotateTraits<DataSet> >(m, "_AnnotateDataSet");

  // FILE -> H5File_py.cpp
  auto pyFile = py::class_<File, Object, NodeTraits<File>, AnnotateTraits<File> >(m, "File");

  // GROUP -> H5Group_py.cpp
  auto pyGroup = py::class_<Group, Object, NodeTraits<Group>, AnnotateTraits<Group> >(m, "Group");

  //DATASET -> H5DataSet_py.cpp
  auto pyDset = py::class_<DataSet, Object, SliceTraits<DataSet>, AnnotateTraits<DataSet> >(m, "DataSet");

  //ATTRIBUTE -> H5Attribute_py.cpp
  auto pyAttr = py::class_<Attribute, Object>(m, "Attribute");



  /*------------------------------------------------------------*/
  /*-------------------------DEFINITION-------------------------*/
  /*------------------------------------------------------------*/

  // ENUM -> H5Enum_py.cpp
  ObjectType_py(pyObjType);
  LinkType_py(pyLinkType);
  PropertyType_py(pyPropertyType);
  OpenFlag_py(pyOpenFlag);
  Endian_py(pyEndian);
  DataTypeClass_py(pyDTypeClass);

  // PROPERTYLIST -> H5PropertyList_py.cpp
  PropertyList_py(pyLinkCreatePropsBase);
  PropertyList_py(pyLinkAccessPropsBase);
  PropertyList_py(pyFileCreatePropsBase);
  PropertyList_py(pyFileAccessPropsBase);
  PropertyList_py(pyGroupCreatePropsBase);
  PropertyList_py(pyGroupAccessPropsBase);
  PropertyList_py(pyDataSetCreatePropsBase);
  PropertyList_py(pyDataSetAccessPropsBase);
  PropertyList_py(pyDataTypeCreatePropsBase);
  PropertyList_py(pyDataTypeAccessPropsBase);

  LinkCreateProps_py(pyLinkCreateProps);
  LinkAccessProps_py(pyLinkAccessProps);
  FileCreateProps_py(pyFileCreateProps);
  FileAccessProps_py(pyFileAccessProps);
  GroupCreateProps_py(pyGroupCreateProps);
  GroupAccessProps_py(pyGroupAccessProps);
  DataSetCreateProps_py(pyDsetCreateProps);
  DataSetAccessProps_py(pyDsetAccessProps);
  DataTypeCreateProps_py(pyDTypeCreateProps);
  DataTypeAccessProps_py(pyDTypeAccessProps);
  ObjectCopyProps_py(pyObjectCopyProps);

  // OBJECT -> H5Object_py.cpp
  ObjectInfo_py(pyObjectInfo);
  LinkInfo_py(pyLinkInfo);
  Object_py(pyObject);

  // SLICE -> H5SliceTraits_py.cpp
  ElementSet_py(pyElementSet);
  SliceTraits_py<Selection>(pySliceTraits_Selection);
  SliceTraits_py<DataSet>(pySliceTraits_Dset);

  // SELECTION -> H5Selection_py.cpp
  Selection_py(pySelection);

  // DATATYPE -> H5DataType_py.cpp
  DataType_py(pyDType);

  /* atomic declarations should be invoked after `DataType_py` as
   * atomic types are children of DataType */
  AtomicType_py(pyAtomicBool);
  AtomicType_py(pyAtomicChar);
  AtomicType_py(pyAtomicSChar);
  AtomicType_py(pyAtomicUChar);
  AtomicType_py(pyAtomicShort);
  AtomicType_py(pyAtomicUShort);
  AtomicType_py(pyAtomicInt);
  AtomicType_py(pyAtomicUInt);
  AtomicType_py(pyAtomicLong);
  AtomicType_py(pyAtomicULong);
  AtomicType_py(pyAtomicLLong);
  AtomicType_py(pyAtomicULLong);
  AtomicType_py(pyAtomicFloat);
  AtomicType_py(pyAtomicDouble);
  AtomicType_py(pyAtomicCFloat);
  AtomicType_py(pyAtomicCDouble);
  AtomicType_py(pyAtomicStr);

  // COMPOUND TYPE
  CompoundMember_py(pyCompoundMember);
  py::bind_vector<std::vector<CompoundType::member_def>>(m, "CompoundMemberArray");
  CompoundType_py(pyCompoundType);

  // ENUM TYPES
  EnumMember_py<int>(pyEnumMemberInt);
  EnumMember_py<unsigned>(pyEnumMemberUInt);
  EnumMember_py<long long>(pyEnumMemberLLong);
  EnumMember_py<unsigned long long>(pyEnumMemberULLong);
  py::bind_vector<std::vector<EnumType<int>::member_def>>(m, "EnumMemberArrayInt");
  py::bind_vector<std::vector<EnumType<unsigned>::member_def>>(m, "EnumMemberArrayUInt");
  py::bind_vector<std::vector<EnumType<long long>::member_def>>(m, "EnumMemberArrayLLong");
  py::bind_vector<std::vector<EnumType<unsigned long long>::member_def>>(m, "EnumMemberArrayULLong");
  EnumType_py(pyEnumTypeInt);
  EnumType_py(pyEnumTypeUInt);
  EnumType_py(pyEnumTypeLLong);
  EnumType_py(pyEnumTypeULLong);

  // DATASPACE -> H5DataSpace_py.cpp
  DataSpace_py(pyDSpace);

  // NODETRAITS -> H5NodeTraits_py.cpp
  NodeTraits_py(pyNodeTraits_File); // _Name <- underlined because user don't need to use it
  NodeTraits_py(pyNodeTraits_Group);

  // ANNOTATETRAITS -> H5Annotate_traits_py.h
  AnnotateTraits_py(pyAnnotate_File);
  AnnotateTraits_py(pyAnnotate_Group);
  AnnotateTraits_py(pyAnnotate_Dset);

  // FILE -> H5File_py.cpp
  File_py(pyFile);

  // GROUP -> H5Group_py.cpp
  Group_py(pyGroup);

  //DATASET -> H5DataSet_py.cpp
  DataSet_py(pyDset);

  //DATASET -> H5DataSet_py.cpp
  Attribute_py(pyAttr);


  py::class_<Point>(m, "Point")
      .def(py::init<double, double, double>())
      .def("getX", &Point::getX)
      .def("getY", &Point::getY)
      .def("getZ", &Point::getZ);

}

} // h5gtpy
