/*
 *  Copyright (c), 2017, Adrien Devresse <adrien.devresse@epfl.ch>
 *
 *  Distributed under the Boost Software License, Version 1.0.
 *    (See accompanying file LICENSE_1_0.txt or copy at
 *          http://www.boost.org/LICENSE_1_0.txt)
 *
 */
#ifndef H5DATASET_HPP
#define H5DATASET_HPP

#include <vector>

#include "H5DataSpace.hpp"
#include "H5DataType.hpp"
#include "bits/H5_definitions.hpp"
#include "bits/H5Annotate_traits.hpp"
#include "bits/H5Slice_traits.hpp"
#include "bits/H5_definitions.hpp"

namespace h5gt {

///
/// \brief Class representing a dataset.
///
class DataSet : public Object,
    public SliceTraits<DataSet>,
    public AnnotateTraits<DataSet> {
public:

  const static ObjectType type = ObjectType::Dataset;

  ///
  /// \brief getStorageSize
  /// \return returns the amount of storage allocated for a dataset.
  ///
  uint64_t getStorageSize() const;

  ///
  /// \brief getOffset
  /// \return returns DataSet address in file
  ///
  uint64_t getOffset() const;

  ///
  /// \brief getDataType
  /// \return return the datatype associated with this dataset
  ///
  DataType getDataType() const;

  LinkInfo getLinkInfo() const;

  ///
  /// \brief getSpace
  /// \return return the dataspace associated with this dataset
  ///
  DataSpace getSpace() const;

  ///
  /// \brief getMemSpace
  /// \return same as getSpace for DataSet, compatibility with Selection
  /// class
  ///
  DataSpace getMemSpace() const;

  File getFile() const;

  ///
  /// \brief unpackSoftLink retrieve target path to this dataset
  /// \return
  ///
  std::string unpackSoftLink() const;

  void unlink() const;

  ///
  /// \brief rename move link within container
  /// \param dest_path
  /// \param linkCreateProps important as they create intermediate groups
  /// \param linkAccessProps
  /// \return
  ///
  bool rename(const std::string& dest_path,
              const LinkCreateProps& linkCreateProps = LinkCreateProps(),
              const LinkAccessProps& linkAccessProps = LinkAccessProps()) const;

  Group getParent(const GroupAccessProps& groupAccessProps = GroupAccessProps()) const;

  DataSetCreateProps getCreateProps() const;
  DataSetAccessProps getAccessProps() const;

  /// \brief Change the size of the dataset
  ///
  /// This requires that the dataset was created with chunking, and you would
  /// generally want to have set a larger maxdims setting
  /// \param dims New size of the dataset
  void resize(const std::vector<size_t>& dims);


  /// \brief Get the dimensions of the whole DataSet.
  ///       This is a shorthand for getSpace().getDimensions()
  /// \return The shape of the current h5gt::DataSet
  ///
  inline std::vector<size_t> getDimensions() const {
    return getSpace().getDimensions();
  }

  /// \brief Get the total number of elements in the current dataset.
  ///       E.g. 2x2x2 matrix has size 8.
  ///       This is a shorthand for getSpace().getTotalCount()
  /// \return The shape of the current h5gt::DataSet
  ///
  inline size_t getElementCount() const {
    return getSpace().getElementCount();
  }

  /// \brief operator == Check if objects reside in the same file and equal to each other
  /// \param other
  /// \return
  bool operator==(const DataSet& other) const;
  bool operator!=(const DataSet& other) const;

  static DataSet FromId(const hid_t& id, const bool& increaseRefCount = false){
      Object obj = Object(id, ObjectType::Dataset, increaseRefCount);
      return DataSet(obj);
  };

protected:
  DataSet(const Object& obj) : Object(obj){};
  using Object::Object;

  inline DataSet(Object&& o) noexcept : Object(std::move(o)) {}

  friend class Reference;
  friend class Selection;
  template <typename Derivate> friend class NodeTraits;

};

}  // namespace h5gt

#endif // H5DATASET_HPP
