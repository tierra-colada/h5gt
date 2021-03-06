/*
 *  Copyright (c), 2017, Adrien Devresse <adrien.devresse@epfl.ch>
 *
 *  Distributed under the Boost Software License, Version 1.0.
 *    (See accompanying file LICENSE_1_0.txt or copy at
 *          http://www.boost.org/LICENSE_1_0.txt)
 *
 */
#ifndef H5OBJECT_HPP
#define H5OBJECT_HPP

#include <ctime>

#include <H5Ipublic.h>
#include <H5Opublic.h>

#include "H5Exception.hpp"
#include "bits/H5_definitions.hpp"

//----------------------------------------------------------------//
// H5Object.hpp H5Object_misc.hpp cant contain variables          //
// of type H5PropertyLists as H5Object.hpp is included            //
// by H5PropertyLists.hpp                                         //
// You will get recursion hell if break this rule                 //
//----------------------------------------------------------------//

namespace h5gt {

class LinkAccessProps;

///
/// \brief Enum of the types of objects (H5O api)
///
enum class ObjectType {
  File,
  Group,
  UserDataType,
  DataSpace,
  Dataset,
  Attribute,
  Other  // Internal/custom object type
};

///
/// \brief The possible types of group entries (link concept)
///
enum class LinkType {
  Hard,
  Soft,
  External,
  Other  // Reserved or User-defined
};


class Object {
public:
  // decrease reference counter
  ~Object();

  ///
  /// \brief flush
  ///
  /// Flushes all buffers associated with an object to disk.
  /// Object is used only to identify the file i.e. there is no
  /// difference whether to flush the dataset or group if they
  /// both belong to the same file.
  /// Scope may be global (flushes the entire virtual file) and
  /// local (flushes only the specified file)
  void flush(bool globalScope = true);

  ///
  /// \brief isValid
  /// \return true if current Object is a valid HDF5Object
  ///
  bool isValid() const noexcept;

#if (H5_VERS_MAJOR >= 1 && H5_VERS_MINOR >= 10)
  ///
  /// \brief refresh usually you don't need to use it.
  /// Close and reopen the object
  /// \return
  ///
  bool refresh() const noexcept;
#endif

  ///
  /// \brief getId
  /// \return internal HDF5 id to the object
  ///  provided for C API compatibility
  ///
  hid_t getId(const bool& increaseRefCount = false) const noexcept;

  std::string getFileName() const;

  ///
  /// \brief return the path to the current group, dataset,
  /// datatype or attribute's holder
  /// \return the path to the dataset
  std::string getPath() const;

  int getIdRefCount() const noexcept;

  ///
  /// \brief Retrieve several infos about the current object (address, dates, etc)
  ///
  ObjectInfo getObjectInfo() const;

  ///
  /// \brief Gets the fundamental type of the object (dataset, group, etc)
  /// \exception ObjectException when the _hid is negative or the type
  ///     is custom and not registered yet
  ///
  ObjectType getObjectType() const;

protected:
  // empty constructor
  Object();

  // copy constructor, increase reference counter
  Object(const Object& other);

  // move constructor, reuse hid
  Object(Object&& other) noexcept;

  // Init with an low-level object id
  explicit Object(const hid_t&);
  explicit Object(const hid_t&, const ObjectType&, const bool&);

  ///
  /// \brief getLinkInfo retrieve link info from an object with 'objPath'.
  /// This object must reside in the same container as current one.
  /// \param objPath relative path to the object
  /// \return
  ///
  LinkInfo _getLinkInfo(const std::string& objPath) const;

  std::string _unpackSoftLink(
      const std::string& objName) const;
  std::string _unpackExternalLink(
      const std::string& objName,
      std::string& fileName_out) const;
  void _unlink(const std::string& obj_name) const;

  Object& operator=(const Object& other);
  /// When coparing objects h5gt::File must be open
  bool operator==(const Object& other) const;

  hid_t _hid;

private:

  template <typename Derivate> friend class NodeTraits;
  template <typename Derivate> friend class AnnotateTraits;
  friend class Reference;
  friend class DataSpace;
  friend class File;
  friend class Group;
  friend class DataSet;
  friend class Attribute;
  friend class DataType;
  friend class Selection;
};


///
/// \brief A class for accessing hdf5 objects info
///
class ObjectInfo {
public:
  /// \brief Retrieve the address of the object (within its file)
#if (H5Oget_info_vers < 3)
  haddr_t getAddress() const noexcept;
#else
  /// \brief Non-zero tokens only for Group, DataSet and DataType
  H5O_token_t getHardLinkToken() const noexcept;
#endif

  unsigned long  getFileNumber() const noexcept;

  /// \brief Retrieve the number of references to this object
  size_t getHardLinkRefCount() const noexcept;

  time_t getAccessTime() const noexcept;
  time_t getModificationTime() const noexcept;
  time_t getChangeTime() const noexcept;
  time_t getCreationTime() const noexcept;
  hsize_t getNumAttr() const noexcept;

protected:

#if (H5Oget_info_vers < 3)
  H5O_info_t raw_info;
#else
  H5O_info2_t raw_info;
#endif

  friend class Object;
};

class LinkInfo  {
public:
  LinkType getLinkType() const noexcept;
  hbool_t creationOrderValid() const noexcept;
  int64_t getCreationOrder() const noexcept;
  H5T_cset_t getLinkNameCharacterSet() const noexcept;
#if (H5Lget_info_vers < 2)
  haddr_t getAddress() const noexcept;
#else
  /// \brief getToken Token of location that hard link points to
  H5O_token_t getHardLinkToken() const noexcept;
#endif
  /// \brief getSoftLinkSize Size of a soft link or UD link value
  size_t getSoftLinkSize() const noexcept;

protected:
#if (H5Lget_info_vers < 2)
  H5L_info_t link_info;
#else
  H5L_info2_t link_info;
#endif

  friend class Object;
};

}  // namespace h5gt

#include "bits/H5Object_misc.hpp"

#endif // H5OBJECT_HPP
