/*
 *  Copyright (c), 2017, Adrien Devresse <adrien.devresse@epfl.ch>
 *
 *  Distributed under the Boost Software License, Version 1.0.
 *    (See accompanying file LICENSE_1_0.txt or copy at
 *          http://www.boost.org/LICENSE_1_0.txt)
 *
 */
#ifndef H5DATATYPE_HPP
#define H5DATATYPE_HPP

#include <vector>

#include "H5Object.hpp"
#include "bits/H5Utils.hpp"

namespace h5gt {


///
/// \brief Enum of Fundamental data classes
///
enum class DataTypeClass {
  Time,
  Integer,
  Float,
  String,
  BitField,
  Opaque,
  Compound,
  Reference,
  Enum,
  VarLen,
  Array,
  Invalid
};

enum Endian {
  Native,
  Little,
  Big
};


///
/// \brief HDF5 Data Type
///
class DataType : public Object {
public:

  DataType() = default;

  /// \brief isTypeEqual Unlike `==` operator this only checks if
  /// the data types are equal and do not check if they belong
  /// It is virtual as checking compound type is something different
  virtual bool isTypeEqual(const DataType& other) const;

  ///
  /// \brief Return the fundamental type.
  ///
  DataTypeClass getClass() const;

  ///
  /// \brief Returns the length (in bytes) of this type elements
  ///
  /// Notice that the size of variable length sequences may have limited applicability
  ///   given that it refers to the size of the control structure. For info see
  ///   https://support.hdfgroup.org/HDF5/doc/RM/RM_H5T.html#Datatype-GetSize
  size_t getSize() const;

  ///
  /// \brief Returns a friendly description of the type (e.g. Float32)
  ///
  std::string string() const;

  ///
  /// \brief Returns whether the type is a variable-length string
  ///
  bool isVariableStr() const;

  ///
  /// \brief Returns whether the type is a fixed-length string
  ///
  bool isFixedLenStr() const;

  ///
  /// \brief Check the DataType was default constructed.
  /// Such value might represent auto-detection of the datatype from a buffer
  ///
  bool empty() const noexcept;

  /// \brief Returns whether the type is a Reference
  bool isReference() const;

  /// \brief operator == Check if objects reside in the same file and equal to each other
  /// \param other
  /// \return
  bool operator==(const DataType& other) const;
  bool operator!=(const DataType& other) const;

  static DataType FromId(const hid_t& id, const bool& increaseRefCount = false){
    Object obj = Object(id, ObjectType::UserDataType, increaseRefCount);
    return DataType(obj);
  };

protected:
  DataType(const Object& obj) : Object(obj){};
  using Object::Object;

  friend class Attribute;
  friend class File;
  friend class Group;
  friend class DataSet;
  friend class NodeTraits<File>;
  friend class NodeTraits<Group>;
};

///
/// \brief create an HDF5 DataType from a C++ type
///
///  Support only basic data type
///
template <typename T>
class AtomicType : public DataType {
public:
  AtomicType(Endian endian = Endian::Native);

  typedef T basic_type;

  static AtomicType FromId(const hid_t& id, const bool& increaseRefCount = false){
    DataType obj = DataType::FromId(id, increaseRefCount);
    return AtomicType(obj);
  };

protected:
  AtomicType(const DataType& obj) : DataType(obj){};
};


///
/// \brief Create a compound HDF5 datatype
///
class CompoundType : public DataType {
public:
  ///
  /// \brief Use for defining a sub-type of compound type
  /// If your dtype contains 'std::string' then you need to explicitely set
  /// 't_offset' in bytest like 'HOFFSET(STRUCT_NAME, MEMBER_NAME)'
  struct member_def {
    member_def(std::string t_name, DataType t_base_type, size_t t_offset = 0)
      : name(std::move(t_name))
      , base_type(std::move(t_base_type))
      , offset(t_offset) {}
    std::string name;
    DataType base_type;
    size_t offset;
  };

  CompoundType(const CompoundType& other) = default;

  ///
  /// \brief Initializes a compound type from a vector of member definitions
  /// \param t_members
  /// \param size 'sizeof(STRUCT_NAME)' (must be set if you data contains 'std::string')
  inline CompoundType(const std::vector<member_def>& t_members, size_t size = 0)
    : members(t_members) {
    create(size);
  }
  /// \param size 'sizeof(STRUCT_NAME)' (must be set if you data contains 'std::string')
  inline CompoundType(std::vector<member_def>&& t_members, size_t size = 0)
    : members(std::move(t_members)) {
    create(size);
  }
  /// \param size 'sizeof(STRUCT_NAME)' (must be set if you data contains 'std::string')
  inline CompoundType(const std::initializer_list<member_def>& t_members,
                      size_t size = 0)
    : members(t_members) {
    create(size);
  }

  /// \brief Commit datatype into the given Object
  /// \param object Location to commit object into
  /// \param name Name to give the datatype
  inline void commit(const Object& object, const std::string& name) const;

  /// \brief Get read access to the CompoundType members
  inline const std::vector<member_def>& getMembers() const noexcept {
    return members;
  }

  DataTypeClass getMemberClass(const unsigned& member_no) const;
  int getMemberIndex(const std::string& field_name) const;
  std::string getMemberName(const unsigned& field_idx) const;
  size_t getMemberOffset(const unsigned& memb_no) const;
  /// return predefined type specified by hdf5 macro like: H5T_C_S1, H5T_NATIVE_INT, H5T_IEEE_F32BE etc.
  hid_t getMemberType(const unsigned& field_idx) const;
  int getNMembers() const;


  static CompoundType FromId(const hid_t& id, const bool& increaseRefCount = false){
    DataType obj = DataType::FromId(id, increaseRefCount);
    return CompoundType(obj);
  };

protected:
  CompoundType(const DataType& obj) : DataType(obj){};

private:

  /// A vector of the member_def members of this CompoundType
  std::vector<member_def> members;

  /// \brief Automatically create the type from the set of members
  ///        using standard struct alignment.
  /// \param size Total size of data type
  void create(size_t size = 0);
};

///
/// \brief Create a enum HDF5 datatype
///
/// \code{.cpp}
/// enum class Position {
///     FIRST = 1,
///     SECOND = 2,
/// };
///
/// EnumType<Position> create_enum_position() {
///     return {{"FIRST", Position::FIRST},
///             {"SECOND", Position::SECOND}};
/// }
///
/// // You have to register the type inside h5gt
/// H5GT_REGISTER_TYPE(Position, create_enum_position)
///
/// void write_first(H5::File& file) {
///     auto dataset = file.createDataSet("/foo", Position::FIRST);
/// }
/// \endcode
template<typename T>
class EnumType: public DataType {
public:
  ///
  /// \brief Use for defining a member of enum type
  struct member_def {
    member_def(const std::string& t_name, T t_value)
      : name(t_name)
      , value(std::move(t_value)) {}
    std::string name;
    T value;
  };

  EnumType(const EnumType& other) = default;

  EnumType(const std::vector<member_def>& t_members)
    : members(t_members) {
    create();
  }

  EnumType(std::initializer_list<member_def> t_members)
    : EnumType(std::vector<member_def>({t_members})) {}

  /// \brief Commit datatype into the given Object
  /// \param object Location to commit object into
  /// \param name Name to give the datatype
  void commit(const Object& object, const std::string& name) const;

  int getMemberIndex(const std::string& field_name) const;
  std::string getMemberName(const unsigned& field_idx) const;
  void getMemberValue(const unsigned& memb_no, void *value) const;
  int getNMembers() const;

  static EnumType FromId(const hid_t& id, const bool& increaseRefCount = false){
    DataType obj = DataType::FromId(id, increaseRefCount);
    return EnumType(obj);
  };

protected:
  EnumType(const DataType& obj) : DataType(obj){};

private:
  std::vector<member_def> members;

  void create();
};


/// \brief Create a DataType instance representing type T
template <typename T>
DataType create_datatype();


/// \brief Create a DataType instance representing type T and perform a sanity check on its size
template <typename T>
DataType create_and_check_datatype();


///
/// \brief A structure representing a set of fixed-length strings
///
/// Although fixed-len arrays can be created 'raw' without the need for
/// this structure, to retrieve results efficiently it must be used.
///
template <std::size_t N>
class FixedLenStringArray {
public:
  FixedLenStringArray() = default;

  ///
  /// \brief Create a FixedStringArray from a raw contiguous buffer
  ///
  FixedLenStringArray(const char array[][N], std::size_t length);

  ///
  /// \brief Create a FixedStringArray from a sequence of strings.
  ///
  /// Such conversion involves a copy, original vector is not modified
  ///
  explicit FixedLenStringArray(const std::vector<std::string> & vec);

  FixedLenStringArray(const std::string* iter_begin, const std::string* iter_end);

  FixedLenStringArray(const std::initializer_list<std::string> &);

  ///
  /// \brief Append an std::string to the buffer structure
  ///
  void push_back(const std::string&);

  void push_back(const std::array<char, N>&);

  ///
  /// \brief Retrieve a string from the structure as std::string
  ///
  std::string getString(std::size_t index) const;

  // Container interface
  inline const char* operator[](std::size_t i) const noexcept {
    return datavec[i].data();
  }
  inline const char* at(std::size_t i) const {
    return datavec.at(i).data();
  }
  inline bool empty() const noexcept {
    return datavec.empty();
  }
  inline std::size_t size() const noexcept {
    return datavec.size();
  }
  inline void resize(std::size_t n) {
    datavec.resize(n);
  }
  inline const char* front() const {
    return datavec.front().data();
  }
  inline const char* back() const {
    return datavec.back().data();
  }
  inline char* data() noexcept {
    return datavec[0].data();
  }
  inline const char* data() const noexcept {
    return datavec[0].data();
  }

private:
  using vector_t = typename std::vector<std::array<char, N>>;

public:
  // Use the underlying iterator
  using iterator = typename vector_t::iterator;
  using const_iterator = typename vector_t::const_iterator;
  using reverse_iterator = typename vector_t::reverse_iterator;
  using const_reverse_iterator = typename vector_t::const_reverse_iterator;
  using value_type = typename vector_t::value_type;

  inline iterator begin() noexcept {
    return datavec.begin();
  }
  inline iterator end() noexcept {
    return datavec.end();
  }
  inline const_iterator begin() const noexcept {
    return datavec.begin();
  }
  inline const_iterator cbegin() const noexcept {
    return datavec.cbegin();
  }
  inline const_iterator end() const noexcept {
    return datavec.end();
  }
  inline const_iterator cend() const noexcept {
    return datavec.cend();
  }
  inline reverse_iterator rbegin() noexcept {
    return datavec.rbegin();
  }
  inline reverse_iterator rend() noexcept {
    return datavec.rend();
  }
  inline const_reverse_iterator rbegin() const noexcept {
    return datavec.rbegin();
  }
  inline const_reverse_iterator rend() const noexcept {
    return datavec.rend();
  }

private:
  vector_t datavec;
};

}  // namespace h5gt


/// \brief Macro to extend datatype of h5gt
///
/// The idea is simple:
/// instantiate `inline DataType create_datatype<type>()` at compile 
/// time so it return a `DataType` for custom `type`.
/// If it is not instantiated then calling `create_datatype` will work
/// with `atomic_type<type>` (not defined for custom type).
///
/// This macro has to be called outside of any namespace.
///
/// \code{.cpp}
/// enum FooBar { FOO = 1, BAR = 2 };
/// EnumType create_enum_foobar() {
///    return EnumType<FooBar>({{"FOO", FooBar::FOO},
///                             {"BAR", FooBar::BAR}});
/// }
/// H5GT_REGISTER_TYPE(FooBar, create_enum_foobar)
/// \endcode
#define H5GT_REGISTER_TYPE(type, function)                \
  template<>                                              \
  inline h5gt::DataType h5gt::create_datatype<type>() {   \
  return function();                                      \
  }                                                       \

#include "bits/H5DataType_misc.hpp"

#endif // H5DATATYPE_HPP
