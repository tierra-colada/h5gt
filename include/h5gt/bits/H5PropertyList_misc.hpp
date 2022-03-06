/*
 *  Copyright (c), 2017-2018, Adrien Devresse <adrien.devresse@epfl.ch>
 *                            Juan Hernando <juan.hernando@epfl.ch>
 *  Distributed under the Boost Software License, Version 1.0.
 *    (See accompanying file LICENSE_1_0.txt or copy at
 *          http://www.boost.org/LICENSE_1_0.txt)
 *
 */
#ifndef H5PROPERTY_LIST_MISC_HPP
#define H5PROPERTY_LIST_MISC_HPP

#include <H5Ppublic.h>

namespace h5gt {

namespace {
inline hid_t convert_plist_type(PropertyType propertyType) {
  // The HP5_XXX are macros with function calls so we can't assign
  // them as the enum values
  switch (propertyType) {
  case PropertyType::OBJECT_CREATE:
    return H5P_OBJECT_CREATE;
  case PropertyType::FILE_CREATE:
    return H5P_FILE_CREATE;
  case PropertyType::FILE_ACCESS:
    return H5P_FILE_ACCESS;
  case PropertyType::DATASET_CREATE:
    return H5P_DATASET_CREATE;
  case PropertyType::DATASET_ACCESS:
    return H5P_DATASET_ACCESS;
  case PropertyType::DATASET_XFER:
    return H5P_DATASET_XFER;
  case PropertyType::GROUP_CREATE:
    return H5P_GROUP_CREATE;
  case PropertyType::GROUP_ACCESS:
    return H5P_GROUP_ACCESS;
  case PropertyType::DATATYPE_CREATE:
    return H5P_DATATYPE_CREATE;
  case PropertyType::DATATYPE_ACCESS:
    return H5P_DATATYPE_ACCESS;
  case PropertyType::STRING_CREATE:
    return H5P_STRING_CREATE;
  case PropertyType::ATTRIBUTE_CREATE:
    return H5P_ATTRIBUTE_CREATE;
  case PropertyType::OBJECT_COPY:
    return H5P_OBJECT_COPY;
  case PropertyType::LINK_CREATE:
    return H5P_LINK_CREATE;
  case PropertyType::LINK_ACCESS:
    return H5P_LINK_ACCESS;
  default:
    HDF5ErrMapper::ToException<PropertyException>(
          "Unsupported property list type");
  }
}

}  // namespace

template <PropertyType T>
inline void PropertyList<T>::initializeId() {
  if ((_hid = H5Pcreate(convert_plist_type(T))) < 0) {
    HDF5ErrMapper::ToException<PropertyException>(
          "Unable to create property list");
  }
}

template <PropertyType T>
///
/// \brief setExternalLinkPrefix Let’s say you’ve created an external link
/// in foo.h5 and the external link refers to a path name in file bar.h5.
/// If (by intention or accident) the relative location of foo.h5 and bar.h5
/// changes, the external link becomes invalid, because the HDF5 library
/// can’t locate the file bar.h5. To deal with a situation like that,
/// you can create a prefix that you’d like to apply to the file names
/// of external links before the library attempts to traverse them.
/// \param prefix
/// \return
///
inline void PropertyList<T>::setExternalLinkPrefix(const std::string& prefix) {
  if (H5Pset_elink_prefix(_hid, prefix.c_str()) < 0){
    HDF5ErrMapper::ToException<PropertyException>(
          "Unable to set external link prefix property");
  }
}

inline void LinkCreateProps::setCreateIntermediateGroup(unsigned val) {
  if (H5Pset_create_intermediate_group(_hid, val) < 0){
    HDF5ErrMapper::ToException<PropertyException>(
          "Unable to set create intermediate group property");
  }
}

inline void DataSetAccessProps::setChunkCache(
    const size_t& numSlots, const size_t& cacheSize, const double& w0)
{
  if (H5Pset_chunk_cache(_hid, numSlots, cacheSize, w0) < 0){
    HDF5ErrMapper::ToException<PropertyException>(
          "Unable to set chunk cache property");
  }
}

inline void DataSetAccessProps::getChunkCache(
    size_t& numSlots, size_t& cacheSize, double& w0)
{
  if (H5Pget_chunk_cache(_hid, &numSlots, &cacheSize, &w0) < 0){
    HDF5ErrMapper::ToException<PropertyException>(
          "Unable to get chunk cache property");
  }
}

inline void DataSetCreateProps::addExternalFile(const std::string& file, off_t offset, hsize_t size)
{
  if (size == 0){
    try {
      size = std::filesystem::file_size(file);
      size = size - offset;
    } catch(std::filesystem::filesystem_error& e) {
      std::cout << e.what() << std::endl;
    }
  }

  if (H5Pset_external(_hid, file.c_str(), offset, size) < 0){
    HDF5ErrMapper::ToException<PropertyException>(
          "Unable to add external file");
  }
}

inline void DataSetCreateProps::addVirtualDataSet(
    const DataSpace& vSpace,
    const DataSet& srcDset,
    const DataSpace& srcSpace)
{
  if (H5Pset_virtual( _hid, vSpace.getId(), srcDset.getFileName().c_str(),
                  srcDset.getPath().c_str(), srcSpace.getId()) < 0){
    HDF5ErrMapper::ToException<PropertyException>(
          "Unable to add set dataset virtual");
  }
}

inline void DataSetCreateProps::setShuffle() {
  if (!H5Zfilter_avail(H5Z_FILTER_SHUFFLE)){
    HDF5ErrMapper::ToException<PropertyException>(
          "Z-FILTER is unavailable");
  }

  if (H5Pset_shuffle(_hid) < 0){
    HDF5ErrMapper::ToException<PropertyException>(
          "Unable to set shuffle property");
  }
}

inline void DataSetCreateProps::setDeflate(const unsigned& level) {
  if (!H5Zfilter_avail(H5Z_FILTER_SHUFFLE)){
    HDF5ErrMapper::ToException<PropertyException>(
          "Z-FILTER is unavailable");
  }

  if (H5Pset_deflate(_hid, level) < 0){
    HDF5ErrMapper::ToException<PropertyException>(
          "Unable to set deflate property");
  }
}

inline void DataSetCreateProps::setChunk(const std::vector<hsize_t>& dims) {
  if (H5Pset_chunk(_hid, static_cast<int>(dims.size()), dims.data()) < 0){
    HDF5ErrMapper::ToException<PropertyException>(
          "Unable to set chunk property");
  }
}

inline size_t DataSetCreateProps::getExternalCount() {
  int num = H5Pget_external_count(_hid);
  if (num < 0){
    HDF5ErrMapper::ToException<PropertyException>(
          "Unable to get number of external files");
  }
  return num;
}

inline std::string DataSetCreateProps::getExternal(
    unsigned idx, off_t& offset, hsize_t& fileSize) {
  char buffer[H5GT_MAX_PATH_LEN + 1];
  herr_t status = H5Pget_external(
        _hid, idx, static_cast<hsize_t>(H5GT_MAX_PATH_LEN) + 1,
        buffer, &offset, &fileSize);
  if (status < 0){
    HDF5ErrMapper::ToException<PropertyException>(
          "Unable to get info about external file");
  }
  return std::string(buffer);
}

inline size_t DataSetCreateProps::getVirtualCount(){
  size_t num;
  herr_t status = H5Pget_virtual_count(_hid, &num);
  if (status < 0){
    HDF5ErrMapper::ToException<PropertyException>(
          "Unable to get number of virtual datasets");
  }
  return num;
}

inline std::string DataSetCreateProps::getVirtualDataSetName(size_t idx){
  char buffer[H5GT_MAX_PATH_LEN + 1];
  ssize_t retcode = H5Pget_virtual_dsetname(
        _hid, idx, buffer,
        static_cast<size_t>(H5GT_MAX_PATH_LEN) + 1);
  if (retcode < 0) {
    HDF5ErrMapper::ToException<PropertyException>("Error accessing object name");
  }
  const size_t length = static_cast<std::size_t>(retcode);
  if (length <= H5GT_MAX_PATH_LEN) {
    return std::string(buffer, length);
  }
  std::vector<char> bigBuffer(length + 1, 0);
  H5Pget_virtual_dsetname(
        _hid, idx, bigBuffer.data(),
        static_cast<hsize_t>(length) + 1);
  return std::string(bigBuffer.data(), length);
}

inline std::string DataSetCreateProps::getVirtualFileName(size_t idx) {
  char buffer[H5GT_MAX_PATH_LEN + 1];
  ssize_t retcode = H5Pget_virtual_filename(
        _hid, idx, buffer,
        static_cast<size_t>(H5GT_MAX_PATH_LEN) + 1);
  if (retcode < 0) {
    HDF5ErrMapper::ToException<PropertyException>("Error accessing object name");
  }
  const size_t length = static_cast<std::size_t>(retcode);
  if (length <= H5GT_MAX_PATH_LEN) {
    return std::string(buffer, length);
  }
  std::vector<char> bigBuffer(length + 1, 0);
  H5Pget_virtual_filename(
        _hid, idx, bigBuffer.data(),
        static_cast<hsize_t>(length) + 1);
  return std::string(bigBuffer.data(), length);
}

inline DataSpace DataSetCreateProps::getVirtualSrcSpace(size_t idx){
  return DataSpace::FromId(H5Pget_virtual_srcspace(_hid, idx));
}

inline DataSpace DataSetCreateProps::getVirtualVSpace(size_t idx){
  return DataSpace::FromId(H5Pget_virtual_vspace(_hid, idx));
}

inline std::vector<hsize_t> DataSetCreateProps::getChunk(int max_ndims)
{
  // initialize with zeros
  hsize_t* dims = (hsize_t *) calloc(max_ndims, sizeof(hsize_t*));
  if (H5Pget_chunk(_hid, max_ndims, dims) < 0 ){
    HDF5ErrMapper::ToException<PropertyException>(
          "Unable to get chunk property");
  }
  std::vector<hsize_t> v;
  v.reserve(max_ndims);
  for (int i = 0; i < max_ndims; i++){
    if (dims[i] == 0)
      break;
    v.push_back(dims[i]);
  }

  v.shrink_to_fit();
  free(dims);
  return v;
}

inline LayoutType DataSetCreateProps::getLayoutType()
{
  return _convert_layout_type(H5Pget_layout(_hid));
}

inline bool DataSetCreateProps::isCompact(){
  if (getLayoutType() == LayoutType::COMPACT)
    return true;
  return false;
}

inline bool DataSetCreateProps::isContiguous(){
  if (getLayoutType() == LayoutType::CONTIGUOUS)
    return true;
  return false;
}

inline bool DataSetCreateProps::isChunked(){
  if (getLayoutType() == LayoutType::CHUNKED)
    return true;
  return false;
}

inline bool DataSetCreateProps::isVirtual(){
  if (getLayoutType() == LayoutType::VIRTUAL)
    return true;
  return false;
}


}  // namespace h5gt

#endif  // H5PROPERTY_LIST_HPP
