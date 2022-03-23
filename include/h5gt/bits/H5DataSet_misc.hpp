/*
 *  Copyright (c), 2017, Adrien Devresse <adrien.devresse@epfl.ch>
 *
 *  Distributed under the Boost Software License, Version 1.0.
 *    (See accompanying file LICENSE_1_0.txt or copy at
 *          http://www.boost.org/LICENSE_1_0.txt)
 *
 */
#ifndef H5DATASET_MISC_HPP
#define H5DATASET_MISC_HPP

#include <algorithm>
#include <functional>
#include <numeric>
#include <sstream>
#include <string>

#ifdef H5GT_USE_BOOST
#include <boost/multi_array.hpp>
#endif

#include <H5Dpublic.h>
#include <H5Ppublic.h>

#include "H5Utils.hpp"

namespace h5gt {

inline uint64_t DataSet::getStorageSize() const {
  return H5Dget_storage_size(_hid);
}

inline DataType DataSet::getDataType() const {
  return DataType(H5Dget_type(_hid));
}

inline LinkInfo DataSet::getLinkInfo() const {
  return Object::_getLinkInfo(getPath());
}

inline DataSpace DataSet::getSpace() const {
  DataSpace space;
  if ((space._hid = H5Dget_space(_hid)) < 0) {
    HDF5ErrMapper::ToException<DataSetException>(
          "Unable to get DataSpace out of DataSet");
  }
  return space;
}

inline DataSpace DataSet::getMemSpace() const {
  return getSpace();
}

inline std::string DataSet::unpackSoftLink() const{
  return Object::_unpackSoftLink(getPath());
}

inline void DataSet::unlink() const{
  return Object::_unlink(getPath());
}

inline bool DataSet::rename(const std::string& dst_path,
                            const LinkCreateProps& linkCreateProps,
                            const LinkAccessProps& linkAccessProps) const {
  herr_t status = H5Lmove(getId(false), getPath().c_str(),
                          getId(false), dst_path.c_str(),
                          linkCreateProps.getId(false), linkAccessProps.getId(false));
  if (status < 0) {
    HDF5ErrMapper::ToException<DataSetException>(
          std::string("Unable to move link to \"") + dst_path + "\":");
    return false;
  }
  return true;
}

inline Group DataSet::getParent(const GroupAccessProps& groupAccessProps) const {
  std::string path = getPath();
  if (path == "/")
    HDF5ErrMapper::ToException<DataSetException>(
      std::string(path + " has no parent"));

  std::string objName;
  std::string parentPath = details::splitPathToParentAndObj(path, objName);
  if (parentPath.empty())
    HDF5ErrMapper::ToException<DataSetException>(
      std::string(objName + " has no parent"));

  File file = File::FromId(getFileId(true));
  return file.getGroup(parentPath, groupAccessProps);
}

inline DataSetCreateProps DataSet::getCreateProps() const{
  hid_t prop_id = H5Dget_create_plist(_hid);
  if (prop_id < 0) {
    HDF5ErrMapper::ToException<DataSetException>(
          "Cannot get creation property list for a DataSet");
  }
  return DataSetCreateProps::FromId(prop_id, false);
}

inline DataSetAccessProps DataSet::getAccessProps() const{
  hid_t prop_id = H5Dget_access_plist(_hid);
  if (prop_id < 0) {
    HDF5ErrMapper::ToException<DataSetException>(
          "Cannot get access property list for a DataSet");
  }
  return DataSetAccessProps::FromId(prop_id, false);
}

inline uint64_t DataSet::getOffset() const {
  uint64_t addr = H5Dget_offset(_hid);
  if (addr == HADDR_UNDEF) {
    HDF5ErrMapper::ToException<DataSetException>(
          "Cannot get offset of DataSet.");
  }
  return addr;
}

inline void DataSet::resize(const std::vector<size_t>& dims) {

  const size_t numDimensions = getSpace().getDimensions().size();
  if (dims.size() != numDimensions) {
    HDF5ErrMapper::ToException<DataSetException>(
          "Invalid dataspace dimensions, got " + std::to_string(dims.size()) +
          " expected " + std::to_string(numDimensions));
  }

  std::vector<hsize_t> real_dims(dims.begin(), dims.end());

  if (H5Dset_extent(getId(false), real_dims.data()) < 0) {
    HDF5ErrMapper::ToException<DataSetException>(
          "Could not resize dataset.");
  }
}

inline bool DataSet::operator==(const DataSet& other) const {
  return Object::operator==(other);
}

inline bool DataSet::operator!=(const DataSet& other) const {
  return !(*this == other);
}

} // namespace h5gt

#endif // H5DATASET_MISC_HPP
