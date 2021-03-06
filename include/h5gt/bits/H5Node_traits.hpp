/*
 *  Copyright (c), 2017, Adrien Devresse <adrien.devresse@epfl.ch>
 *
 *  Distributed under the Boost Software License, Version 1.0.
 *    (See accompanying file LICENSE_1_0.txt or copy at
 *          http://www.boost.org/LICENSE_1_0.txt)
 *
 */
#ifndef H5NODE_TRAITS_HPP
#define H5NODE_TRAITS_HPP

#include <string>

#include "../H5PropertyList.hpp"
#include "H5_definitions.hpp"

namespace h5gt {

///
/// \brief NodeTraits: Base class for Group and File
///
template <typename Derivate>
class NodeTraits {
public:

  ///
  /// \brief createDataSet Create a new dataset in the current file of
  /// datatype type and of size space
  /// \param dataset_name identifier of the dataset
  /// \param space Associated DataSpace, see \ref DataSpace for more
  /// informations
  /// \param type Type of Data
  /// \param linkCreateProps A link property list with data set creation properties
  /// \param dsetCreateProps A dataset property list with data set creation properties
  /// \param dsetAccessProps A dataset property list with data set access properties
  /// \return DataSet Object
  DataSet
  createDataSet(const std::string& dataset_name,
                const DataSpace& space,
                const DataType& type,
                const LinkCreateProps& linkCreateProps = LinkCreateProps(),
                const DataSetCreateProps& dsetCreateProps = DataSetCreateProps(),
                const DataSetAccessProps& dsetAccessProps = DataSetAccessProps());

  ///
  /// \brief createDataSet create a new dataset in the current file with a
  /// size specified by space
  /// \param dataset_name identifier of the dataset
  /// \param space Associated DataSpace, see \ref DataSpace for more
  /// information
  /// \param linkCreateProps A link property list with data set creation properties
  /// \param dsetCreateProps A dataset property list with data set creation properties
  /// \param dsetAccessProps A dataset property list with data set access properties
  /// \return DataSet Object
  template<typename Type>
  DataSet
  createDataSet(const std::string& dataset_name,
                const DataSpace& space,
                const LinkCreateProps& linkCreateProps = LinkCreateProps(),
                const DataSetCreateProps& dsetCreateProps = DataSetCreateProps(),
                const DataSetAccessProps& dsetAccessProps = DataSetAccessProps());

  ///
  /// \brief createDataSet create a new dataset in the current file and
  /// write to it, inferring the DataSpace from the data.
  /// \param dataset_name identifier of the dataset
  /// \param data Associated data, must support DataSpace::From, see
  /// \ref DataSpace for more information
  /// \param linkCreateProps A link property list with data set creation properties
  /// \param dsetCreateProps A dataset property list with data set creation properties
  /// \param dsetAccessProps A dataset property list with data set access properties
  /// \return DataSet Object
  template <typename T>
  DataSet
  createDataSet(const std::string& dataset_name,
                const T& data,
                const LinkCreateProps& linkCreateProps = LinkCreateProps(),
                const DataSetCreateProps& dsetCreateProps = DataSetCreateProps(),
                const DataSetAccessProps& dsetAccessProps = DataSetAccessProps());


  template <std::size_t N>
  DataSet
  createDataSet(const std::string& dataset_name,
                const FixedLenStringArray<N>& data,
                const LinkCreateProps& linkCreateProps = LinkCreateProps(),
                const DataSetCreateProps& dsetCreateProps = DataSetCreateProps(),
                const DataSetAccessProps& dsetAccessProps = DataSetAccessProps());

  DataType getDataType(
      const std::string& dtype_name,
      const DataTypeAccessProps& dtypeAccessProps = DataTypeAccessProps()) const ;

  ///
  /// \brief get an existing dataset in the current file
  /// \param dataset_name
  /// \param accessProps property list to configure dataset chunk cache
  /// \return return the named dataset, or throw exception if not found
  DataSet getDataSet(
      const std::string& dataset_name,
      const DataSetAccessProps& accessProps = DataSetAccessProps()) const;

  ///
  /// \brief create a new group, and eventually intermediate groups
  /// \param group_name
  /// \param linkCreateProps A link property list with data set creation properties
  /// \param groupCreateProps A group property list with data set creation properties
  /// \param groupAccessProps A group property list with data set access properties
  /// \return the group object
  Group createGroup(const std::string& group_name,
                    const LinkCreateProps& linkCreateProps = LinkCreateProps(),
                    const GroupCreateProps& groupCreateProps = GroupCreateProps(),
                    const GroupAccessProps& groupAccessProps = GroupAccessProps());

  ///
  /// \brief open an existing group with the name group_name
  /// \param group_name
  /// \return the group object
  Group getGroup(const std::string& group_name,
                 const GroupAccessProps& groupAccessProps = GroupAccessProps()) const;

  File getFile() const;

  ///
  /// \brief return the number of leaf objects of the node / group
  /// \return number of leaf objects
  size_t getNumberObjects() const;

  ///
  /// \brief return the name of the object with the given index
  /// \param linkCreateProps A link property list with data set creation properties
  /// \return the name of the object
  std::string getObjectName(size_t index, const LinkAccessProps& linkAccessProps = LinkAccessProps()) const;

  LinkInfo getLinkInfo(const std::string& objName) const;

  ///
  /// \brief unpackSoftLink retrieve target path object path
  /// \param objName
  /// \return
  ///
  std::string unpackSoftLink(const std::string& objName);

  ///
  /// \brief unpackExternalLink retrieve target path object path and file path
  /// \param objName
  /// \param fileName_out
  /// \return target object path
  ///
  std::string unpackExternalLink(const std::string& objName, std::string& fileName_out);

  ///
  /// \brief moves an object and its content within an HDF5 file.
  /// \param src_path relative path of the object to current File/Group
  /// \param dest_path new relative path of the object to current File/Group
  /// \param linkCreateProps A link property list with data set creation properties
  /// \param linkAccessProps A link property list with data set access properties
  /// \return boolean that is true if the move was successful
  bool rename(const std::string& src_path,
              const std::string& dest_path,
              const LinkCreateProps& linkCreateProps = LinkCreateProps(),
              const LinkAccessProps& linkAccessProps = LinkAccessProps()) const;

  ///
  /// \brief list all leaf objects name of the node / group
  /// \return number of leaf objects
  std::vector<std::string> listObjectNames() const;

  ///
  /// \brief check a dataset or group exists in the current node / group
  /// \param obj_name dataset/group name to check
  /// \param linkAccessProps A link property list with data set access properties
  /// \return true if a dataset/group with the associated name exists, or false
  bool exist(const std::string& obj_name,
             const LinkAccessProps& linkAccessProps = LinkAccessProps(),
             bool raise_errors = false) const;

  ///
  /// \brief resolved Hard links always resolved but Soft/External links
  /// may point to unexistant objects
  /// \param obj_name
  /// \param linkAccessProps
  /// \param raise_errors
  /// \return
  bool resolved(const std::string& obj_name,
                const LinkAccessProps& linkAccessProps = LinkAccessProps(),
                bool raise_errors = false) const;

  ///
  /// \brief existAndResolved invokes `exist()` and then `reoslved()`
  bool existAndResolved(const std::string& obj_name,
                        const LinkAccessProps& linkAccessProps = LinkAccessProps(),
                        bool raise_errors = false) const;

  bool hasObject(const std::string& objName, const ObjectType& objType,
                 const LinkAccessProps& linkAccessProps = LinkAccessProps(),
                 bool raise_errors = false) const;

  ///
  /// \brief unlink the given dataset or group
  /// \param obj_name dataset/group name to unlink
  /// \param linkAccessProps A link property list with data set access properties
  void unlink(const std::string& obj_name, const LinkAccessProps& linkAccessProps = LinkAccessProps()) const;

  ///
  /// \brief Returns the kind of link of the given name (soft, hard...)
  /// \param obj_name The entry to check, path relative to the current group
  /// \param linkAccessProps A link property list with data set access properties
  LinkType getLinkType(const std::string& obj_name,
                       const LinkAccessProps& linkAccessProps = LinkAccessProps()) const;

  ///
  /// \brief A shorthand to get the kind of object pointed to (group, dataset, type...)
  /// \param obj_name The entry to check, path relative to the current group
  /// \param linkAccessProps A link property list with data set access properties
  inline ObjectType getObjectType(const std::string& obj_name, const LinkAccessProps& accessProps = LinkAccessProps()) const;


  ///
  /// \brief Copies specified object to the new destination (this File/Group) under new name.
  /// The destination must not already have the object with the same name (objNewName).
  /// \return Copied object
  Group copy(const Group& obj, const std::string& objNewName,
             const ObjectCopyProps& copyProps = ObjectCopyProps(),
             const LinkCreateProps& linkCreateProps = LinkCreateProps(),
             const GroupAccessProps& groupAccessProps = GroupAccessProps());

  DataSet copy(const DataSet& obj, const std::string& objNewName,
               const ObjectCopyProps& copyProps = ObjectCopyProps(),
               const LinkCreateProps& linkCreateProps = LinkCreateProps(),
               const DataSetAccessProps& dsetAccessProps = DataSetAccessProps());

  /// Copies commited datatype
  DataType copy(const DataType& obj, const std::string& objNewName,
                const ObjectCopyProps& copyProps = ObjectCopyProps(),
                const LinkCreateProps& linkCreateProps = LinkCreateProps(),
                const DataTypeAccessProps& dtypeAccessProps = DataTypeAccessProps());

  template<typename Node,
           typename std::enable_if<
             std::is_same<Node, File>::value |
             std::is_same<Node, Group>::value>::type* = nullptr>
  Group createLink(
      const Node& target,
      const std::string& linkName,
      const LinkType& linkType,
      const std::string& targetPath = "",
      const LinkCreateProps& linkCreateProps = LinkCreateProps(),
      const LinkAccessProps& linkAccessProps = LinkAccessProps(),
      const GroupAccessProps& groupAccessProps = GroupAccessProps());

  DataSet createLink(
      const DataSet& target,
      const std::string& linkName,
      const LinkType& linkType,
      const std::string& targetPath = "",
      const LinkCreateProps& linkCreateProps = LinkCreateProps(),
      const LinkAccessProps& linkAccessProps = LinkAccessProps(),
      const DataSetAccessProps& dsetAccessProps = DataSetAccessProps());

  DataType createLink(
      const DataType& target,
      const std::string& linkName,
      const LinkType& linkType,
      const std::string& targetPath = "",
      const LinkCreateProps& linkCreateProps = LinkCreateProps(),
      const LinkAccessProps& linkAccessProps = LinkAccessProps(),
      const DataTypeAccessProps& dtypeAccessProps = DataTypeAccessProps());

private:
  typedef Derivate derivate_type;

  // A wrapper over the low-level H5Lexist
  // It makes behavior consistent among versions and by default transforms
  // errors to exceptions
  bool _exist(const std::string& node_name,
              const LinkAccessProps& accessProps = LinkAccessProps(),
              bool raise_errors = false) const;

  bool _resolved(const std::string& node_name,
                 const LinkAccessProps& accessProps = LinkAccessProps(),
                 bool raise_errors = false) const;

  // Opens an arbitrary object to obtain info
  Object _open(const std::string& node_name,
               const LinkAccessProps& accessProps = LinkAccessProps()) const;

  template<typename T>
  void _copy(const T& obj, const std::string& newName,
             const ObjectCopyProps& copyProps = ObjectCopyProps(),
             const LinkCreateProps& linkCreateProps = LinkCreateProps());

  /// targetPath is useful to create links using relative path
  template<typename T>
  void _createLink(
      T& target,
      const std::string& linkName,
      const LinkType& linkType,
      std::string targetPath = "",
      const LinkCreateProps& linkCreateProps = LinkCreateProps(),
      const LinkAccessProps& linkAccessProps = LinkAccessProps());
};


}  // namespace h5gt


#endif  // H5NODE_TRAITS_HPP
