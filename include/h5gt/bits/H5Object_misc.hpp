/*
 *  Copyright (c), 2017, Adrien Devresse <adrien.devresse@epfl.ch>
 *
 *  Distributed under the Boost Software License, Version 1.0.
 *    (See accompanying file LICENSE_1_0.txt or copy at
 *          http://www.boost.org/LICENSE_1_0.txt)
 *
 */
#ifndef H5OBJECT_MISC_HPP
#define H5OBJECT_MISC_HPP

#include <iostream>
#include <H5Ppublic.h>

#include "H5Utils.hpp"

namespace h5gt {

static inline ObjectType _convert_object_type(const H5I_type_t& h5type) {
  switch (h5type) {
  case H5I_FILE:
    return ObjectType::File;
  case H5I_GROUP:
    return ObjectType::Group;
  case H5I_DATATYPE:
    return ObjectType::UserDataType;
  case H5I_DATASPACE:
    return ObjectType::DataSpace;
  case H5I_DATASET:
    return ObjectType::Dataset;
  case H5I_ATTR:
    return ObjectType::Attribute;
  default:
    return ObjectType::Other;
  }
}

// convert internal link types to enum class.
// This function is internal, so H5L_TYPE_ERROR shall be handled in the calling context
static inline LinkType _convert_link_type(const H5L_type_t& ltype) noexcept {
  switch (ltype) {
  case H5L_TYPE_HARD:
    return LinkType::Hard;
  case H5L_TYPE_SOFT:
    return LinkType::Soft;
  case H5L_TYPE_EXTERNAL:
    return LinkType::External;
  default:
    // Other link types are possible but are considered strange to h5gt.
    // see https://support.hdfgroup.org/HDF5/doc/RM/H5L/H5Lregister.htm
    return LinkType::Other;
  }
}

inline Object::Object() : _hid(H5I_INVALID_HID) {}

inline Object::Object(const hid_t& hid) : _hid(hid) {}

inline Object::Object(const Object& other) : _hid(other._hid) {
  if (other.isValid() && H5Iinc_ref(_hid) < 0) {
    throw ObjectException("Reference counter increase failure");
  }
}

inline Object::Object(Object&& other) noexcept
  : _hid(other._hid)  {
  other._hid = H5I_INVALID_HID;
}

inline Object::Object(const hid_t& hid,
                      const ObjectType& objType,
                      const bool& increaseRefCount) {
  if (hid < 0) {
    HDF5ErrMapper::ToException<ObjectException>(
          std::string("Invalid id to initialize the object"));
    return;
  }

  ObjectType objType_from_id = _convert_object_type(H5Iget_type(hid));
  if (objType_from_id != objType){
    HDF5ErrMapper::ToException<ObjectException>(
          std::string("Given ID doesn't belong to the requested type (or it is invalid)"));
    return;
  }

  if (increaseRefCount)
    H5Iinc_ref(hid);

  _hid = hid;
}

inline Object& Object::operator=(const Object& other) {
  if (this != &other) {
    if (_hid != H5I_INVALID_HID)
      H5Idec_ref(_hid);

    _hid = other._hid;
    if (other.isValid() && H5Iinc_ref(_hid) < 0) {
      throw ObjectException("Reference counter increase failure");
    }
  }
  return *this;
}

inline bool Object::operator==(const Object& other) const {
  ObjectInfo leftOInfo = getObjectInfo();
  ObjectInfo rightOInfo = other.getObjectInfo();

  if (leftOInfo.getFileNumber() != rightOInfo.getFileNumber() ||
      leftOInfo.getFileNumber() == 0 ||
      rightOInfo.getFileNumber() == 0)
    return false;

#if (H5Oget_info_vers < 3)
  return getObjectInfo().getAddress() == other.getObjectInfo().getAddress();
#else
  int tokenCMP;
  H5O_token_t leftToken = leftOInfo.getHardLinkToken();
  H5O_token_t rightToken = rightOInfo.getHardLinkToken();

  hid_t fileId = H5Iget_file_id(_hid);
  bool closeFileId = true;
  if (!H5Iis_valid(fileId)){
    closeFileId = false;
    HDF5ErrMapper::ToException<ObjectException>(
          std::string("File ID is invalid. Probably the object doesn't belong to any file"));
  }

  if (H5Otoken_cmp(fileId, &leftToken, &rightToken, &tokenCMP) < 0){
    HDF5ErrMapper::ToException<ObjectException>(
          "Unable compare tokens");
  }

  // important: close opened File ID
  if (closeFileId){
    H5Idec_ref(fileId);
  }

  return !tokenCMP;
#endif
}

inline Object::~Object() {
  if (isValid() && H5Idec_ref(_hid) < 0) {
    std::cerr << "h5gt::~Object: reference counter decrease failure"
                  << std::endl;
  }
}

inline void Object::flush(bool globalScope) {
  H5F_scope_t scope;
  if (globalScope)
    scope = H5F_scope_t::H5F_SCOPE_GLOBAL;
  else
    scope = H5F_scope_t::H5F_SCOPE_LOCAL;
  if (H5Fflush(_hid, scope) < 0) {
    HDF5ErrMapper::ToException<ObjectException>(
          std::string("Unable to flush object " + getPath()));
  }
}

inline bool Object::isValid() const noexcept {
  return (_hid != H5I_INVALID_HID) && (H5Iis_valid(_hid) != false);
}

#if (H5_VERS_MAJOR >= 1 && H5_VERS_MINOR >= 10)
inline bool Object::refresh() const noexcept {
  return (H5Orefresh(_hid) < 0) ? false : true;
}
#endif

inline hid_t Object::getId(const bool& increaseRefCount) const noexcept {
  if (increaseRefCount)
    H5Iinc_ref(_hid);

  return _hid;
}

inline std::string Object::getFileName() const {
  return details::get_name([&](char *buffer, hsize_t length) {
    return H5Fget_name(_hid, buffer, length);
  });
}

inline std::string Object::getPath() const {
  return details::get_name([&](char *buffer, hsize_t length) {
    return H5Iget_name(_hid, buffer, length);
  });
}

inline int Object::getIdRefCount() const noexcept {
  return H5Iget_ref(_hid);
}

inline ObjectType Object::getObjectType() const {
  // H5Iget_type is a very lightweight func which extracts the type from the id
  H5I_type_t h5type;
  if ((h5type = H5Iget_type(_hid)) == H5I_BADID) {
    HDF5ErrMapper::ToException<ObjectException>("Invalid hid or object type");
  }
  return _convert_object_type(h5type);
}

inline ObjectInfo Object::getObjectInfo() const {
  ObjectInfo info;
#if (H5Oget_info_vers < 3)
  if (H5Oget_info(_hid, &info.raw_info) < 0) {
#else
  if (H5Oget_info3(_hid, &info.raw_info, H5O_INFO_ALL) < 0) {
#endif
    HDF5ErrMapper::ToException<ObjectException>("Unable to obtain info for object");
  }
  return info;
}

inline LinkInfo Object::_getLinkInfo(const std::string& objPath) const {
  LinkInfo info;
#if (H5Lget_info_vers < 2)
  if (H5Lget_info(_hid, objPath.c_str(), &info.link_info, H5P_DEFAULT) < 0) {
#else
  if (H5Lget_info2(_hid, objPath.c_str(), &info.link_info, H5P_DEFAULT) < 0) {
#endif
    HDF5ErrMapper::ToException<ObjectException>("Unable to obtain info for link");
  }
  return info;
}

inline std::string Object::_unpackSoftLink(
    const std::string& objName) const
{
  h5gt::LinkInfo linkInfo = _getLinkInfo(objName);
  // to avoid segfault check the type of a link
  if (linkInfo.getLinkType() != LinkType::Soft){
    HDF5ErrMapper::ToException<ObjectException>(
          std::string("The link is not Soft"));
    return std::string();
  }

  size_t n = linkInfo.getSoftLinkSize();
  if (n < 1){
    HDF5ErrMapper::ToException<ObjectException>(
          std::string("Link size is too small (n < 1)"));
    return std::string();
  }
  std::vector<char> v(n);
  if (H5Lget_val(getId(false), objName.c_str(),
                 v.data(), n, H5P_DEFAULT) < 0){
    HDF5ErrMapper::ToException<ObjectException>(
          std::string("Unable to get path to which the link points to"));
  }
  return std::string(v.data(), n-1);
}

inline std::string Object::_unpackExternalLink(
    const std::string& objName,
    std::string& fileName_out) const
{
  h5gt::LinkInfo linkInfo = _getLinkInfo(objName);
  // to avoid segfault check the type of a link
  if (linkInfo.getLinkType() != LinkType::External){
    HDF5ErrMapper::ToException<ObjectException>(
          std::string("The link is not External"));
    return std::string();
  }

  size_t n = linkInfo.getSoftLinkSize();
  if (n < 1){
    HDF5ErrMapper::ToException<ObjectException>(
          std::string("Link size is too small (n < 1)"));
    return std::string();
  }
  std::vector<char> v(n);
  if (H5Lget_val(getId(false), objName.c_str(),
                 v.data(), n, H5P_DEFAULT) < 0){
    HDF5ErrMapper::ToException<ObjectException>(
          std::string("Unable to get path to which the link points to"));
  }

  const char* f_out;
  const char* o_out;
  unsigned flags = 0;
  if (H5Lunpack_elink_val(v.data(), n, &flags, &f_out, &o_out) < 0){
    HDF5ErrMapper::ToException<ObjectException>(
          std::string("Unable to get path to which the link points to"));
  }

  fileName_out = std::string(f_out);
  return std::string(o_out);
}

inline void Object::_unlink(const std::string& objName) const
{
  const herr_t val = H5Ldelete(getId(false), objName.c_str(), H5P_DEFAULT);
  if (val < 0) {
    HDF5ErrMapper::ToException<ObjectException>(
          std::string("Invalid name for unlink() "));
  }
}

#if (H5Lget_info_vers < 2)
inline haddr_t ObjectInfo::getAddress() const noexcept {
  return raw_info.addr;
}
#else
inline H5O_token_t ObjectInfo::getHardLinkToken() const noexcept{
  return raw_info.token;
}
#endif

inline unsigned long ObjectInfo::getFileNumber() const noexcept {
  return raw_info.fileno;
}

inline size_t ObjectInfo::getHardLinkRefCount() const noexcept {
  return raw_info.rc;
}
inline time_t ObjectInfo::getAccessTime() const noexcept {
  return raw_info.atime;
}
inline time_t ObjectInfo::getModificationTime() const noexcept {
  return raw_info.mtime;
}
inline time_t ObjectInfo::getChangeTime() const noexcept {
  return raw_info.ctime;
}
inline time_t ObjectInfo::getCreationTime() const noexcept {
  return raw_info.btime;
}
inline hsize_t ObjectInfo::getNumAttr() const noexcept {
  return raw_info.num_attrs;
}


inline LinkType LinkInfo::getLinkType() const noexcept{
  return _convert_link_type(link_info.type);
}
inline hbool_t LinkInfo::creationOrderValid() const noexcept{
  return link_info.corder_valid;
}
inline int64_t LinkInfo::getCreationOrder() const noexcept{
  return link_info.corder;
}
inline H5T_cset_t LinkInfo::getLinkNameCharacterSet() const noexcept{
  return link_info.cset;
}
#if (H5Lget_info_vers < 2)
inline haddr_t LinkInfo::getAddress() const noexcept{
  return link_info.u.address;
}
#else
inline H5O_token_t LinkInfo::getHardLinkToken() const noexcept{
  return link_info.u.token;
}
#endif
inline size_t LinkInfo::getSoftLinkSize() const noexcept{
  return link_info.u.val_size;
}

}  // namespace

#endif // H5OBJECT_MISC_HPP
