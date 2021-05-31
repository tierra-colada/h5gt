#include "../../include/h5gtpy/H5Enum_py.h"

namespace h5gtpy {

void ObjectType_py(py::enum_<ObjectType> &py_obj) {
  py_obj
      .value("File", ObjectType::File)
      .value("Group", ObjectType::Group)
      .value("UserDataType", ObjectType::UserDataType)
      .value("DataSpace", ObjectType::DataSpace)
      .value("Dataset", ObjectType::Dataset)
      .value("Attribute", ObjectType::Attribute)
      .value("Other", ObjectType::Other);
}

void LinkType_py(py::enum_<LinkType> &py_obj) {
  py_obj
      .value("Hard", LinkType::Hard)
      .value("Soft", LinkType::Soft)
      .value("External", LinkType::External)
      .value("Other", LinkType::Other);
}

void PropertyType_py(py::enum_<PropertyType> &py_obj) {
  py_obj
      .value("OBJECT_CREATE", PropertyType::OBJECT_CREATE)
      .value("FILE_CREATE", PropertyType::FILE_CREATE)
      .value("FILE_ACCESS", PropertyType::FILE_ACCESS)
      .value("DATASET_CREATE", PropertyType::DATASET_CREATE)
      .value("DATASET_ACCESS", PropertyType::DATASET_ACCESS)
      .value("DATASET_XFER", PropertyType::DATASET_XFER)
      .value("GROUP_CREATE", PropertyType::GROUP_CREATE)
      .value("GROUP_ACCESS", PropertyType::GROUP_ACCESS)
      .value("DATATYPE_CREATE", PropertyType::DATATYPE_CREATE)
      .value("DATATYPE_ACCESS", PropertyType::DATATYPE_ACCESS)
      .value("STRING_CREATE", PropertyType::STRING_CREATE)
      .value("ATTRIBUTE_CREATE", PropertyType::ATTRIBUTE_CREATE)
      .value("OBJECT_COPY", PropertyType::OBJECT_COPY)
      .value("LINK_CREATE", PropertyType::LINK_CREATE)
      .value("LINK_ACCESS", PropertyType::LINK_ACCESS);
}

void DataTypeClass_py(py::enum_<DataTypeClass> &py_obj) {
  py_obj
      .value("Time", DataTypeClass::Time)
      .value("Integer", DataTypeClass::Integer)
      .value("Float", DataTypeClass::Float)
      .value("String", DataTypeClass::String)
      .value("BitField", DataTypeClass::BitField)
      .value("Opaque", DataTypeClass::Opaque)
      .value("Compound", DataTypeClass::Compound)
      .value("Reference", DataTypeClass::Reference)
      .value("Enum", DataTypeClass::Enum)
      .value("VarLen", DataTypeClass::VarLen)
      .value("Array", DataTypeClass::Array)
      .value("Invalid", DataTypeClass::Invalid);
}

void OpenFlag_py(py::enum_<File::OpenFlag> &py_obj) {
  py_obj
      .value("ReadOnly", File::ReadOnly)
      .value("ReadWrite", File::ReadWrite)
      .value("Truncate", File::Truncate)
      .value("Excl", File::Excl)
      .value("Debug", File::Debug)
      .value("Create", File::Create)
      .value("Overwrite", File::Overwrite)
      .value("OpenOrCreate", File::OpenOrCreate)
      .export_values();
}


} // h5gtpy
