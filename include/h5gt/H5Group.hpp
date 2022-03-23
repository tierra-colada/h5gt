/*
 *  Copyright (c), 2017, Adrien Devresse <adrien.devresse@epfl.ch>
 *
 *  Distributed under the Boost Software License, Version 1.0.
 *    (See accompanying file LICENSE_1_0.txt or copy at
 *          http://www.boost.org/LICENSE_1_0.txt)
 *
 */
#ifndef H5GT_H5GROUP_HPP
#define H5GT_H5GROUP_HPP

#include "H5Object.hpp"
#include "bits/H5_definitions.hpp"
#include "bits/H5Annotate_traits.hpp"
#include "bits/H5Node_traits.hpp"

namespace h5gt {

///
/// \brief Represents an hdf5 group
class Group : public Object,
    public NodeTraits<Group>,
    public AnnotateTraits<Group> {
public:

  // this makes available to use both
  // Object::getObjectType and NodeTraits<T>::getObjectType
  using Object::getObjectType;
  using NodeTraits<Group>::getObjectType;
  using NodeTraits<Group>::unpackSoftLink;
  using NodeTraits<Group>::unlink;
  using NodeTraits<Group>::rename;

  const static ObjectType type = ObjectType::Group;

  LinkInfo getLinkInfo() const {
    return Object::_getLinkInfo(getPath());
  }

  ///
  /// \brief unpackSoftLink retrieve the path where Soft link points to
  /// \param objName
  /// \param accessProp
  /// \return path to the object
  ///
  std::string unpackSoftLink() const{
    return Object::_unpackSoftLink(getPath());
  }

  void unlink() const{
    return Object::_unlink(getPath());
  }

  ///
  /// \brief rename move link within container
  /// \param dest_path
  /// \param linkCreateProps important as they create intermediate groups
  /// \param linkAccessProps
  /// \return
  ///
  bool rename(const std::string& dest_path,
              const LinkCreateProps& linkCreateProps = LinkCreateProps(),
              const LinkAccessProps& linkAccessProps = LinkAccessProps()) const{
    return NodeTraits<Group>::rename(getPath(), dest_path, linkCreateProps, linkAccessProps);
  }

  Group getParent(const GroupAccessProps& groupAccessProps = GroupAccessProps()) const {
    std::string path = getPath();
    if (path == "/")
      HDF5ErrMapper::ToException<GroupException>(
        std::string(path + " has no parent"));

    std::string objName;
    std::string parentPath = details::splitPathToParentAndObj(path, objName);
    if (parentPath.empty())
      HDF5ErrMapper::ToException<GroupException>(
        std::string(objName + " has no parent"));

    return getGroup(parentPath, groupAccessProps);
  }

  /// \brief operator == Check if objects reside in the same file and equal to each other
  /// \param other
  /// \return
  bool operator==(const Group& other) const {
    return Object::operator==(other);
  }

  bool operator!=(const Group& other) const {
    return !(*this == other);
  }

  static Group FromId(const hid_t& id, const bool& increaseRefCount = false){
    Object obj = Object(id, ObjectType::Group, increaseRefCount);
    return Group(obj);
  };

protected:
  Group(const Object& obj) : Object(obj){};
  using Object::Object;

  inline Group(Object&& o) noexcept : Object(std::move(o)) {};

  friend class File;
  friend class Reference;
  template <typename Derivate> friend class ::h5gt::NodeTraits;
};

}  // namespace h5gt

#endif // H5GT_H5GROUP_HPP
