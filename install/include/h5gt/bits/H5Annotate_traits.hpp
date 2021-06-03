/*
 *  Copyright (c), 2017, Adrien Devresse <adrien.devresse@epfl.ch>
 *
 *  Distributed under the Boost Software License, Version 1.0.
 *    (See accompanying file LICENSE_1_0.txt or copy at
 *          http://www.boost.org/LICENSE_1_0.txt)
 *
 */
#ifndef H5ANNOTATE_TRAITS_HPP
#define H5ANNOTATE_TRAITS_HPP

#include <string>

#include "../H5Attribute.hpp"

namespace h5gt {

template <typename Derivate>
class AnnotateTraits {
public:
  ///
  /// \brief create a new attribute with the name attr_name
  /// \param attr_name identifier of the attribute
  /// \param space Associated \ref DataSpace
  /// \param type
  /// \return the attribute object
  ///
  Attribute createAttribute(const std::string& attr_name,
                            const DataSpace& space,
                            const DataType& type);

  ///
  /// \brief createAttribute create a new attribute on the current dataset with
  /// size specified by space
  /// \param attr_name identifier of the attribute
  /// \param space Associated DataSpace
  /// informations
  /// \return Attribute Object
  template <typename Type>
  Attribute createAttribute(const std::string& attr_name,
                            const DataSpace& space);

  ///
  /// \brief createAttribute create a new attribute on the current dataset and
  /// write to it, inferring the DataSpace from data.
  /// \param attr_name identifier of the attribute
  /// \param data Associated data to write, must support DataSpace::From, see
  /// \ref DataSpace for more information
  /// \return Attribute Object
  ///
  template <typename T>
  Attribute createAttribute(const std::string& attr_name,
                            const T& data);

  ///
  /// \brief deleteAttribute let you delete an attribute by its name.
  /// \param attr_name identifier of the attribute
  void deleteAttribute(const std::string& attr_name);

  ///
  /// \brief open an existing attribute with the name attr_name
  /// \param attr_name identifier of the attribute
  /// \return the attribute object
  Attribute getAttribute(const std::string& attr_name) const;

  ///
  /// \brief return the number of attributes of the object
  /// \return number of attributes
  size_t getNumberAttributes() const;

  ///
  /// \brief list all attribute name of the object
  /// \return number of attributes
  std::vector<std::string> listAttributeNames() const;

  ///
  /// \brief checks an attribute exists
  /// \return number of attributes
  bool hasAttribute(const std::string& attr_name) const;

private:
  typedef Derivate derivate_type;
};
}


#endif // H5ANNOTATE_TRAITS_HPP
