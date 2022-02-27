/*
 *  Copyright (c), 2017-2018, Adrien Devresse <adrien.devresse@epfl.ch>
 *                            Juan Hernando <juan.hernando@epfl.ch>
 *  Distributed under the Boost Software License, Version 1.0.
 *    (See accompanying file LICENSE_1_0.txt or copy at
 *          http://www.boost.org/LICENSE_1_0.txt)
 *
 */
#ifndef H5PROPERTY_LIST_HPP
#define H5PROPERTY_LIST_HPP

#include <vector>
#include <filesystem>

#include <H5Ppublic.h>

#include "H5Exception.hpp"
#include "H5Object.hpp"

namespace h5gt {

///
/// \brief Types dataset layout
///
enum class LayoutType : int {
  COMPACT,
  CONTIGUOUS,
  CHUNKED,
  VIRTUAL,
};

static inline LayoutType _convert_layout_type(const H5D_layout_t& h5type) {
  switch (h5type) {
  case H5D_COMPACT:
    return LayoutType::COMPACT;
  case H5D_CONTIGUOUS:
    return LayoutType::CONTIGUOUS;
  case H5D_CHUNKED:
    return LayoutType::CHUNKED;
  case H5D_VIRTUAL:
    return LayoutType::VIRTUAL;
  default:
    return static_cast<LayoutType>(-1);
  }
}

///
/// \brief Types of property lists
///
enum class PropertyType : int {
  OBJECT_CREATE,
  FILE_CREATE,
  FILE_ACCESS,
  DATASET_CREATE,
  DATASET_ACCESS,
  DATASET_XFER,
  GROUP_CREATE,
  GROUP_ACCESS,
  DATATYPE_CREATE,
  DATATYPE_ACCESS,
  STRING_CREATE,
  ATTRIBUTE_CREATE,
  OBJECT_COPY,
  LINK_CREATE,
  LINK_ACCESS,
};

///
/// \brief Base HDF5 property List
///
template <PropertyType T>
class PropertyList {

  // copy and move constructors are strongly needed by pybind11

public:
  ~PropertyList() {
    if (_hid != H5P_DEFAULT) {
      H5Pclose(_hid);
    }
  }

  PropertyList(const PropertyList<T>& other) :
    _hid(other.getId(true)){};

  PropertyList& operator=(const PropertyList<T>& other){
    _hid = other.getId(true);
    return *this;
  };

  PropertyList& operator=(PropertyList&& other) noexcept{
    _hid = other._hid;
    other._hid = H5I_INVALID_HID;
    return *this;
  }

  constexpr PropertyType getObjectType() const {
    return T;
  }

  hid_t getId(const bool& increaseRefCount = false) const {
    if (increaseRefCount)
      H5Iinc_ref(_hid);

    return _hid;
  }

  PropertyList() noexcept{
    initializeId();
  }

  PropertyList(PropertyList&& other) noexcept :
    _hid(other.getId(false)){
    other._hid = H5I_INVALID_HID;
  }

protected:
  PropertyList(const hid_t& hid) noexcept : _hid(hid) {}

  void setExternalLinkPrefix(const std::string& prefix);

  std::vector<hsize_t> getChunk(int max_ndims);

  void initializeId();

  hid_t _hid;
};

class LinkCreateProps : public PropertyList<PropertyType::LINK_CREATE> {
public:
  LinkCreateProps(){
    setCreateIntermediateGroup(1);  // create intermediate groups ON by default
  }

  void setCreateIntermediateGroup(unsigned val);
};

class LinkAccessProps : public PropertyList<PropertyType::LINK_ACCESS> {
public:
  LinkAccessProps(){}

  void setExternalLinkPrefix(const std::string& prefix){
    PropertyList::setExternalLinkPrefix(prefix);
  }
};

class FileCreateProps : public PropertyList<PropertyType::FILE_CREATE> {
public:
  FileCreateProps(){}
};

class FileAccessProps : public PropertyList<PropertyType::FILE_ACCESS> {
public:
  FileAccessProps(){}
};

class GroupCreateProps : public PropertyList<PropertyType::GROUP_CREATE> {
public:
  GroupCreateProps(){}
};

class GroupAccessProps : public PropertyList<PropertyType::GROUP_ACCESS> {
public:
  GroupAccessProps(){}

  void setExternalLinkPrefix(const std::string& prefix){
    PropertyList::setExternalLinkPrefix(prefix);
  }
};

class DataSetCreateProps : public PropertyList<PropertyType::DATASET_CREATE> {
public:
  DataSetCreateProps(){}

  static DataSetCreateProps FromId(const hid_t& id, const bool& increaseRefCount = false){
    hid_t prop_class_id = H5Pget_class(id);

    if (prop_class_id < 0){
      HDF5ErrMapper::ToException<PropertyException>(
            "Unable to get property class");
    }
    if (H5Pequal(prop_class_id, H5P_DATASET_CREATE) <= 0){
      HDF5ErrMapper::ToException<PropertyException>(
            "Property id is of different class");
    }
    if (increaseRefCount)
      H5Iinc_ref(id);

    return DataSetCreateProps(id);
  };

  /// \brief map external binary file to a dataset
  /// \param file path to the file
  /// \param offset offset in bytes
  /// \param size is the size of the part that is to be mapped (offset+size=fileSize)
  /// leave it as 0 to to map everything starting from the offset
  /// size == H5F_UNLIMITED means the external file can be of unlimited size and
  /// no more files can be added to the external files list
  void addExternalFile(
      const std::string& file,
      off_t offset = 0, hsize_t size = 0);

  /// \brief map source dataset
  /// Spaces must have equal number of selected elements
  /// Spaces may be given from selection: Selection.getSpace()
  /// srcDset may be mapped external file
  void addVirtualDataSet(
      const DataSpace& vSpace,
      const DataSet& srcDset,
      const DataSpace& srcSpace);

  void setShuffle();

  void setDeflate(const unsigned& level);

  void setChunk(const std::initializer_list<hsize_t>& items){
    std::vector<hsize_t> dims{items};
    setChunk(dims);
  }

  template <typename... Args>
  void setChunk(hsize_t item, Args... args){
    std::vector<hsize_t> dims{item, static_cast<hsize_t>(args)...};
    setChunk(dims);
  }

  void setChunk(const std::vector<hsize_t>& dims);

  std::vector<hsize_t> getChunk(int max_ndims);

  LayoutType getLayoutType();

  bool isCompact();
  bool isContiguous();
  bool isChunked();
  bool isVirtual();

  // this allows to inherit all constructors `PropertyList(id)` for example
  // https://stackoverflow.com/questions/347358/inheriting-constructors
  using PropertyList::PropertyList;
};

class DataSetAccessProps : public PropertyList<PropertyType::DATASET_ACCESS> {
public:
  DataSetAccessProps(){}

  static DataSetAccessProps FromId(const hid_t& id, const bool& increaseRefCount = false){
    hid_t prop_class_id = H5Pget_class(id);

    if (prop_class_id < 0){
      HDF5ErrMapper::ToException<PropertyException>(
            "Unable to get property class");
    }
    if (H5Pequal(prop_class_id, H5P_DATASET_ACCESS) <= 0){
      HDF5ErrMapper::ToException<PropertyException>(
            "Property id is of different class");
    }
    if (increaseRefCount)
      H5Iinc_ref(id);

    return DataSetAccessProps(id);
  };

  void setChunkCache(
      const size_t& numSlots, const size_t& cacheSize,
      const double& w0 = static_cast<double>(H5D_CHUNK_CACHE_W0_DEFAULT));

  void setExternalLinkPrefix(const std::string& prefix){
    PropertyList::setExternalLinkPrefix(prefix);
  }

  void getChunkCache(
      size_t& numSlots, size_t& cacheSize, double& w0);

  using PropertyList::PropertyList;
};

class DataTypeCreateProps : public PropertyList<PropertyType::DATATYPE_CREATE> {
public:
  DataTypeCreateProps(){}
};

class DataTypeAccessProps : public PropertyList<PropertyType::DATATYPE_ACCESS> {
public:
  DataTypeAccessProps(){}
};

class DataTransferProps : public PropertyList<PropertyType::DATASET_XFER> {
public:
  DataTransferProps(){}
};

class ObjectCopyProps : public PropertyList<PropertyType::OBJECT_COPY> {
public:
  ObjectCopyProps(){}
};

}  // namespace h5gt


#endif  // H5PROPERTY_LIST_HPP
