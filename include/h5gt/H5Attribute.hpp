/*
 *  Copyright (c), 2017, Ali Can Demiralp <ali.demiralp@rwth-aachen.de>
 *
 *  Distributed under the Boost Software License, Version 1.0.
 *    (See accompanying file LICENSE_1_0.txt or copy at
 *          http://www.boost.org/LICENSE_1_0.txt)
 *
 */
#ifndef H5ATTRIBUTE_HPP
#define H5ATTRIBUTE_HPP

#include <vector>

#include "H5DataSpace.hpp"
#include "H5DataType.hpp"

namespace h5gt {

///
/// \brief Class representing an attribute of a dataset or group
///
class Attribute : public Object {
public:

  const static ObjectType type = ObjectType::Attribute;

  ///
  /// \brief return the name of the current attribute
  /// \return the name of the attribute
  std::string getName() const;

  ///
  /// \brief getStorageSize
  /// \return returns the amount of storage allocated for an attribute.
  ///
  size_t getStorageSize() const;

  ///
  /// \brief getDataType
  /// \return return the datatype associated with this attribute
  ///
  DataType getDataType() const;

  ///
  /// \brief getSpace
  /// \return return the dataspace associated with this attribute
  ///
  DataSpace getSpace() const;

  ///
  /// \brief getMemSpace
  /// \return same as getSpace for attribute, compatibility with Selection
  /// class
  ///
  DataSpace getMemSpace() const;

  File getFile() const;

  ///
  /// Read the attribute into a buffer
  /// An exception is raised if the numbers of dimension of the buffer and of
  /// the attribute are different
  ///
  /// The array type can be a N-pointer or a N-vector ( e.g int** integer two
  /// dimensional array )
  template <typename T>
  void read(T& array) const;

  ///
  /// Read the attribute into a buffer
  ///
  template <typename T>
  void read(T* array, const DataType& dtype = DataType()) const;

  ///
  /// Write the integrality N-dimension buffer to this attribute
  /// An exception is raised if the numbers of dimension of the buffer and of
  /// the attribute are different
  ///
  /// The array type can be a N-pointer or a N-vector ( e.g int** integer two
  /// dimensional array )
  template <typename T>
  void write(const T& buffer);

  ///
  /// Write a buffer to this attribute
  ///
  template <typename T>
  void write_raw(const T* buffer, const DataType& dtype = DataType());

  static Attribute FromId(const hid_t& id, const bool& increaseRefCount = false){
    Object obj = Object(id, ObjectType::Attribute, increaseRefCount);
    return Attribute(obj);
  };

protected:
  Attribute(const Object& obj) : Object(obj){};

private:
  Attribute() = default;

  template <typename Derivate> friend class ::h5gt::AnnotateTraits;
};

}  // namespace h5gt


#endif // H5ATTRIBUTE_HPP
