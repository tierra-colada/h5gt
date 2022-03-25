"""HDF5 wrapper originally written in C++: https://github.com/tierra-colada/h5gt"""
import h5gtpy._h5gt
import typing
import numpy
_Shape = typing.Tuple[int, ...]

__all__ = [
    "AtomicBool",
    "AtomicCDouble",
    "AtomicCFloat",
    "AtomicChar",
    "AtomicDouble",
    "AtomicFloat",
    "AtomicInt",
    "AtomicLLong",
    "AtomicLong",
    "AtomicSChar",
    "AtomicShort",
    "AtomicStr",
    "AtomicUChar",
    "AtomicUInt",
    "AtomicULLong",
    "AtomicULong",
    "AtomicUShort",
    "Attribute",
    "Create",
    "DataSet",
    "DataSetAccessProps",
    "DataSetCreateProps",
    "DataSpace",
    "DataType",
    "DataTypeAccessProps",
    "DataTypeClass",
    "DataTypeCreateProps",
    "Debug",
    "ElementSet",
    "Exception",
    "Excl",
    "File",
    "FileAccessProps",
    "FileCreateProps",
    "Group",
    "GroupAccessProps",
    "GroupCreateProps",
    "LinkAccessProps",
    "LinkCreateProps",
    "LinkInfo",
    "LinkType",
    "Object",
    "ObjectInfo",
    "ObjectType",
    "OpenFlag",
    "OpenOrCreate",
    "Overwrite",
    "Point",
    "PropertyType",
    "ReadOnly",
    "ReadWrite",
    "Selection",
    "Truncate"
]


class Object():
    def getFileName(self) -> str: ...
    def getId(self, increaseRefCount: bool = false) -> int: ...
    def getIdRefCount(self) -> int: ...
    def getObjectInfo(self) -> ObjectInfo: ...
    def getObjectType(self) -> ObjectType: ...
    def getPath(self) -> str: ...
    def isValid(self) -> bool: ...
    def refresh(self) -> bool: 
        """
        Usually you don't need to use it.
        """
    pass
class DataType(Object):
    @staticmethod
    def FromId(id: int, increaseRefCount: bool = false) -> DataType: 
        """
        Create new object from ID
        """
    def __eq__(self, arg0: DataType) -> bool: ...
    def __init__(self) -> None: ...
    def __ne__(self, arg0: DataType) -> bool: ...
    def empty(self) -> bool: 
        """
        Check the DataType was default constructed. Such value might represent auto-detection of the datatype from a buffer
        """
    def getClass(self) -> DataTypeClass: 
        """
        the fundamental type.
        """
    def getSize(self) -> int: 
        """
        Notice that the size of variable length sequences may have limited applicabilitygiven that it refers to the size of the control structure. For info seehttps://support.hdfgroup.org/HDF5/doc/RM/RM_H5T.html#Datatype-GetSize
        """
    def isFixedLenStr(self) -> bool: 
        """
        Returns whether the type is a fixed-length string
        """
    def isReference(self) -> bool: 
        """
        Returns whether the type is a Reference
        """
    def isTypeEqual(self, arg0: DataType) -> bool: 
        """
        isTypeEqual Unlike `==` operator this only checks ifthe data types are equal and do not check if they belongto the same h5 file
        """
    def isVariableStr(self) -> bool: 
        """
        Returns whether the type is a variable-length string
        """
    def string(self) -> str: 
        """
        Returns a friendly description of the type (e.g. Float32)
        """
    __hash__ = None
    pass
class AtomicCFloat(DataType, Object):
    def __init__(self) -> None: ...
    pass
class AtomicChar(DataType, Object):
    def __init__(self) -> None: ...
    pass
class AtomicDouble(DataType, Object):
    def __init__(self) -> None: ...
    pass
class AtomicFloat(DataType, Object):
    def __init__(self) -> None: ...
    pass
class AtomicInt(DataType, Object):
    def __init__(self) -> None: ...
    pass
class AtomicLLong(DataType, Object):
    def __init__(self) -> None: ...
    pass
class AtomicLong(DataType, Object):
    def __init__(self) -> None: ...
    pass
class AtomicSChar(DataType, Object):
    def __init__(self) -> None: ...
    pass
class AtomicShort(DataType, Object):
    def __init__(self) -> None: ...
    pass
class AtomicStr(DataType, Object):
    def __init__(self) -> None: ...
    pass
class AtomicUChar(DataType, Object):
    def __init__(self) -> None: ...
    pass
class AtomicUInt(DataType, Object):
    def __init__(self) -> None: ...
    pass
class AtomicULLong(DataType, Object):
    def __init__(self) -> None: ...
    pass
class AtomicULong(DataType, Object):
    def __init__(self) -> None: ...
    pass
class AtomicUShort(DataType, Object):
    def __init__(self) -> None: ...
    pass
class Attribute(Object):
    def getDataType(self) -> DataType: 
        """
        return the datatype associated with this attribute
        """
    def getMemSpace(self) -> DataSpace: 
        """
        same as getSpace for attribute, compatibility with Selection
        """
    def getName(self) -> str: 
        """
        return the name of the current attribute
        """
    def getSpace(self) -> DataSpace: 
        """
        return the dataspace associated with this attribute
        """
    def getStorageSize(self) -> int: 
        """
        returns the amount of storage allocated for an attribute
        """
    def readBool(self) -> numpy.ndarray[bool, _Shape[m, n]]: ...
    def readCDouble(self) -> numpy.ndarray[numpy.complex128, _Shape[m, n]]: ...
    def readCFloat(self) -> numpy.ndarray[numpy.complex64, _Shape[m, n]]: ...
    def readChar(self) -> numpy.ndarray[numpy.int8, _Shape[m, n]]: ...
    def readDouble(self) -> numpy.ndarray[numpy.float64, _Shape[m, n]]: ...
    def readFloat(self) -> numpy.ndarray[numpy.float32, _Shape[m, n]]: ...
    def readInt(self) -> numpy.ndarray[numpy.int32, _Shape[m, n]]: ...
    def readLLong(self) -> numpy.ndarray[numpy.int64, _Shape[m, n]]: ...
    def readLong(self) -> numpy.ndarray[numpy.int32, _Shape[m, n]]: ...
    def readSChar(self) -> numpy.ndarray[numpy.int8, _Shape[m, n]]: ...
    def readShort(self) -> numpy.ndarray[numpy.int16, _Shape[m, n]]: ...
    def readStr(self) -> typing.List[str]: ...
    def readUChar(self) -> numpy.ndarray[numpy.uint8, _Shape[m, n]]: ...
    def readUInt(self) -> numpy.ndarray[numpy.uint32, _Shape[m, n]]: ...
    def readULLong(self) -> numpy.ndarray[numpy.uint64, _Shape[m, n]]: ...
    def readULong(self) -> numpy.ndarray[numpy.uint32, _Shape[m, n]]: ...
    def readUShort(self) -> numpy.ndarray[numpy.uint16, _Shape[m, n]]: ...
    @typing.overload
    def write(self, arg0: complex) -> None: 
        """
        An exception is raised if the numbers of dimension of the buffer and of the dataset are different
        """
    @typing.overload
    def write(self, arg0: float) -> None: ...
    @typing.overload
    def write(self, arg0: int) -> None: ...
    @typing.overload
    def write(self, arg0: numpy.ndarray[bool, _Shape[m, n]]) -> None: ...
    @typing.overload
    def write(self, arg0: numpy.ndarray[numpy.complex128, _Shape[m, n]]) -> None: ...
    @typing.overload
    def write(self, arg0: numpy.ndarray[numpy.complex64, _Shape[m, n]]) -> None: ...
    @typing.overload
    def write(self, arg0: numpy.ndarray[numpy.float32, _Shape[m, n]]) -> None: ...
    @typing.overload
    def write(self, arg0: numpy.ndarray[numpy.float64, _Shape[m, n]]) -> None: ...
    @typing.overload
    def write(self, arg0: numpy.ndarray[numpy.int16, _Shape[m, n]]) -> None: ...
    @typing.overload
    def write(self, arg0: numpy.ndarray[numpy.int32, _Shape[m, n]]) -> None: ...
    @typing.overload
    def write(self, arg0: numpy.ndarray[numpy.int64, _Shape[m, n]]) -> None: ...
    @typing.overload
    def write(self, arg0: numpy.ndarray[numpy.int8, _Shape[m, n]]) -> None: ...
    @typing.overload
    def write(self, arg0: numpy.ndarray[numpy.uint16, _Shape[m, n]]) -> None: ...
    @typing.overload
    def write(self, arg0: numpy.ndarray[numpy.uint32, _Shape[m, n]]) -> None: ...
    @typing.overload
    def write(self, arg0: numpy.ndarray[numpy.uint64, _Shape[m, n]]) -> None: ...
    @typing.overload
    def write(self, arg0: numpy.ndarray[numpy.uint8, _Shape[m, n]]) -> None: ...
    @typing.overload
    def write(self, arg0: str) -> None: ...
    @typing.overload
    def write(self, arg0: typing.List[complex]) -> None: ...
    @typing.overload
    def write(self, arg0: typing.List[float]) -> None: ...
    @typing.overload
    def write(self, arg0: typing.List[int]) -> None: ...
    @typing.overload
    def write(self, arg0: typing.List[str]) -> None: ...
    @typing.overload
    def write(self, data: bool) -> None: ...
    @typing.overload
    def write_raw(self, data: numpy.ndarray[bool, _Shape[m, n]]) -> None: 
        """
        No dimensionality checks will be performed
        """
    @typing.overload
    def write_raw(self, data: numpy.ndarray[numpy.complex128, _Shape[m, n]]) -> None: ...
    @typing.overload
    def write_raw(self, data: numpy.ndarray[numpy.complex64, _Shape[m, n]]) -> None: ...
    @typing.overload
    def write_raw(self, data: numpy.ndarray[numpy.float32, _Shape[m, n]]) -> None: ...
    @typing.overload
    def write_raw(self, data: numpy.ndarray[numpy.float64, _Shape[m, n]]) -> None: ...
    @typing.overload
    def write_raw(self, data: numpy.ndarray[numpy.int16, _Shape[m, n]]) -> None: ...
    @typing.overload
    def write_raw(self, data: numpy.ndarray[numpy.int32, _Shape[m, n]]) -> None: ...
    @typing.overload
    def write_raw(self, data: numpy.ndarray[numpy.int64, _Shape[m, n]]) -> None: ...
    @typing.overload
    def write_raw(self, data: numpy.ndarray[numpy.int8, _Shape[m, n]]) -> None: ...
    @typing.overload
    def write_raw(self, data: numpy.ndarray[numpy.uint16, _Shape[m, n]]) -> None: ...
    @typing.overload
    def write_raw(self, data: numpy.ndarray[numpy.uint32, _Shape[m, n]]) -> None: ...
    @typing.overload
    def write_raw(self, data: numpy.ndarray[numpy.uint64, _Shape[m, n]]) -> None: ...
    @typing.overload
    def write_raw(self, data: numpy.ndarray[numpy.uint8, _Shape[m, n]]) -> None: ...
    @typing.overload
    def write_raw(self, data: typing.List[complex]) -> None: ...
    @typing.overload
    def write_raw(self, data: typing.List[float]) -> None: ...
    @typing.overload
    def write_raw(self, data: typing.List[int]) -> None: ...
    pass
class _AnnotateDataSet():
    @typing.overload
    def createAttribute(self, attr_name: str, dim: int, type: DataType) -> Attribute: 
        """
        create a new attribute with the name attr_name
        """
    @typing.overload
    def createAttribute(self, attr_name: str, space: DataSpace, type: DataType) -> Attribute: ...
    def deleteAttribute(self, attr_name: str) -> None: 
        """
        let you delete an attribute by its name.
        """
    def getAttribute(self, attr_name: str) -> Attribute: 
        """
        open an existing attribute with the name attr_name
        """
    def getNumberAttributes(self) -> int: 
        """
        return the number of attributes of the object
        """
    def hasAttribute(self, attr_name: str) -> bool: 
        """
        open an existing attribute with the name attr_name
        """
    def listAttributeNames(self) -> typing.List[str]: 
        """
        list all attribute name of the object
        """
    pass
class _DataSetAccessProps():
    def getId(self, increaseRefCount: bool = false) -> int: ...
    def getObjectType(self) -> PropertyType: ...
    pass
class _DataSetCreateProps():
    def getId(self, increaseRefCount: bool = false) -> int: ...
    def getObjectType(self) -> PropertyType: ...
    pass
class DataSpace(Object):
    @staticmethod
    def FromId(id: int, increaseRefCount: bool = false) -> DataSpace: 
        """
        Create new object from ID
        """
    @typing.overload
    def __init__(self, dims: typing.List[int]) -> None: 
        """
        Each element in vector `dims` defines new dimension: size(dim1) = vec[0], size(dim2) = vec[1], etc..

        Create a resizable N-dimensional dataspace
        """
    @typing.overload
    def __init__(self, dims: typing.List[int], maxdims: typing.List[int]) -> None: ...
    def clone(self) -> DataSpace: 
        """
        Create a new DataSpace with a different id available for modifications
        """
    def getDimensions(self) -> typing.List[int]: 
        """
        return a vector of N-element, each element is the size of the associated dataset dimension
        """
    def getElementCount(self) -> int: 
        """
        the total number of elements in the dataspace
        """
    def getMaxDimensions(self) -> typing.List[int]: 
        """
        return a vector of N-element, each element is the size of the associated dataset maximum dimension
        """
    def getNumberDimensions(self) -> int: 
        """
        the number of dimensions in the current dataspace
        """
    pass
class AtomicBool(DataType, Object):
    def __init__(self) -> None: ...
    pass
class _DataTypeAccessProps():
    def getId(self, increaseRefCount: bool = false) -> int: ...
    def getObjectType(self) -> PropertyType: ...
    pass
class DataTypeClass():
    """
    Members:

      Time

      Integer

      Float

      String

      BitField

      Opaque

      Compound

      Reference

      Enum

      VarLen

      Array

      Invalid
    """
    def __eq__(self, other: object) -> bool: ...
    def __ge__(self, other: object) -> bool: ...
    def __getstate__(self) -> int: ...
    def __gt__(self, other: object) -> bool: ...
    def __hash__(self) -> int: ...
    def __init__(self, value: int) -> None: ...
    def __int__(self) -> int: ...
    def __le__(self, other: object) -> bool: ...
    def __lt__(self, other: object) -> bool: ...
    def __ne__(self, other: object) -> bool: ...
    def __repr__(self) -> str: ...
    def __setstate__(self, state: int) -> None: ...
    @property
    def name(self) -> str:
        """
        :type: str
        """
    @property
    def value(self) -> int:
        """
        :type: int
        """
    Array: h5gtpy._h5gt.DataTypeClass # value = <DataTypeClass.Array: 10>
    BitField: h5gtpy._h5gt.DataTypeClass # value = <DataTypeClass.BitField: 4>
    Compound: h5gtpy._h5gt.DataTypeClass # value = <DataTypeClass.Compound: 6>
    Enum: h5gtpy._h5gt.DataTypeClass # value = <DataTypeClass.Enum: 8>
    Float: h5gtpy._h5gt.DataTypeClass # value = <DataTypeClass.Float: 2>
    Integer: h5gtpy._h5gt.DataTypeClass # value = <DataTypeClass.Integer: 1>
    Invalid: h5gtpy._h5gt.DataTypeClass # value = <DataTypeClass.Invalid: 11>
    Opaque: h5gtpy._h5gt.DataTypeClass # value = <DataTypeClass.Opaque: 5>
    Reference: h5gtpy._h5gt.DataTypeClass # value = <DataTypeClass.Reference: 7>
    String: h5gtpy._h5gt.DataTypeClass # value = <DataTypeClass.String: 3>
    Time: h5gtpy._h5gt.DataTypeClass # value = <DataTypeClass.Time: 0>
    VarLen: h5gtpy._h5gt.DataTypeClass # value = <DataTypeClass.VarLen: 9>
    __members__: dict # value = {'Time': <DataTypeClass.Time: 0>, 'Integer': <DataTypeClass.Integer: 1>, 'Float': <DataTypeClass.Float: 2>, 'String': <DataTypeClass.String: 3>, 'BitField': <DataTypeClass.BitField: 4>, 'Opaque': <DataTypeClass.Opaque: 5>, 'Compound': <DataTypeClass.Compound: 6>, 'Reference': <DataTypeClass.Reference: 7>, 'Enum': <DataTypeClass.Enum: 8>, 'VarLen': <DataTypeClass.VarLen: 9>, 'Array': <DataTypeClass.Array: 10>, 'Invalid': <DataTypeClass.Invalid: 11>}
    pass
class _DataTypeCreateProps():
    def getId(self, increaseRefCount: bool = false) -> int: ...
    def getObjectType(self) -> PropertyType: ...
    pass
class ElementSet():
    @typing.overload
    def __init__(self, arg0: typing.List[int]) -> None: ...
    @typing.overload
    def __init__(self, arg0: typing.List[typing.List[int]]) -> None: ...
    pass
class Exception(BaseException):
    pass
class _AnnotateFile():
    @typing.overload
    def createAttribute(self, attr_name: str, dim: int, type: DataType) -> Attribute: 
        """
        create a new attribute with the name attr_name
        """
    @typing.overload
    def createAttribute(self, attr_name: str, space: DataSpace, type: DataType) -> Attribute: ...
    def deleteAttribute(self, attr_name: str) -> None: 
        """
        let you delete an attribute by its name.
        """
    def getAttribute(self, attr_name: str) -> Attribute: 
        """
        open an existing attribute with the name attr_name
        """
    def getNumberAttributes(self) -> int: 
        """
        return the number of attributes of the object
        """
    def hasAttribute(self, attr_name: str) -> bool: 
        """
        open an existing attribute with the name attr_name
        """
    def listAttributeNames(self) -> typing.List[str]: 
        """
        list all attribute name of the object
        """
    pass
class _FileAccessProps():
    def getId(self, increaseRefCount: bool = false) -> int: ...
    def getObjectType(self) -> PropertyType: ...
    pass
class _FileCreateProps():
    def getId(self, increaseRefCount: bool = false) -> int: ...
    def getObjectType(self) -> PropertyType: ...
    pass
class _AnnotateGroup():
    @typing.overload
    def createAttribute(self, attr_name: str, dim: int, type: DataType) -> Attribute: 
        """
        create a new attribute with the name attr_name
        """
    @typing.overload
    def createAttribute(self, attr_name: str, space: DataSpace, type: DataType) -> Attribute: ...
    def deleteAttribute(self, attr_name: str) -> None: 
        """
        let you delete an attribute by its name.
        """
    def getAttribute(self, attr_name: str) -> Attribute: 
        """
        open an existing attribute with the name attr_name
        """
    def getNumberAttributes(self) -> int: 
        """
        return the number of attributes of the object
        """
    def hasAttribute(self, attr_name: str) -> bool: 
        """
        open an existing attribute with the name attr_name
        """
    def listAttributeNames(self) -> typing.List[str]: 
        """
        list all attribute name of the object
        """
    pass
class _GroupAccessProps():
    def getId(self, increaseRefCount: bool = false) -> int: ...
    def getObjectType(self) -> PropertyType: ...
    pass
class _GroupCreateProps():
    def getId(self, increaseRefCount: bool = false) -> int: ...
    def getObjectType(self) -> PropertyType: ...
    pass
class _LinkAccessProps():
    def getId(self, increaseRefCount: bool = false) -> int: ...
    def getObjectType(self) -> PropertyType: ...
    pass
class _LinkCreateProps():
    def getId(self, increaseRefCount: bool = false) -> int: ...
    def getObjectType(self) -> PropertyType: ...
    pass
class LinkInfo():
    def creationOrderValid(self) -> bool: ...
    def getCreationOrder(self) -> int: ...
    def getHardLinkToken(self) -> H5O_token_t: ...
    def getLinkNameCharacterSet(self) -> H5T_cset_t: ...
    def getLinkType(self) -> LinkType: ...
    def getSoftLinkSize(self) -> int: ...
    pass
class LinkType():
    """
    Members:

      Hard

      Soft

      External

      Other
    """
    def __eq__(self, other: object) -> bool: ...
    def __ge__(self, other: object) -> bool: ...
    def __getstate__(self) -> int: ...
    def __gt__(self, other: object) -> bool: ...
    def __hash__(self) -> int: ...
    def __init__(self, value: int) -> None: ...
    def __int__(self) -> int: ...
    def __le__(self, other: object) -> bool: ...
    def __lt__(self, other: object) -> bool: ...
    def __ne__(self, other: object) -> bool: ...
    def __repr__(self) -> str: ...
    def __setstate__(self, state: int) -> None: ...
    @property
    def name(self) -> str:
        """
        :type: str
        """
    @property
    def value(self) -> int:
        """
        :type: int
        """
    External: h5gtpy._h5gt.LinkType # value = <LinkType.External: 2>
    Hard: h5gtpy._h5gt.LinkType # value = <LinkType.Hard: 0>
    Other: h5gtpy._h5gt.LinkType # value = <LinkType.Other: 3>
    Soft: h5gtpy._h5gt.LinkType # value = <LinkType.Soft: 1>
    __members__: dict # value = {'Hard': <LinkType.Hard: 0>, 'Soft': <LinkType.Soft: 1>, 'External': <LinkType.External: 2>, 'Other': <LinkType.Other: 3>}
    pass
class AtomicCDouble(DataType, Object):
    def __init__(self) -> None: ...
    pass
class ObjectInfo():
    def getAccessTime(self) -> int: ...
    def getChangeTime(self) -> int: ...
    def getCreationTime(self) -> int: ...
    def getFileNumber(self) -> int: ...
    def getHardLinkRefCount(self) -> int: ...
    def getHardLinkToken(self) -> H5O_token_t: ...
    def getModificationTime(self) -> int: ...
    def getNumAttr(self) -> int: ...
    pass
class ObjectType():
    """
    Members:

      File

      Group

      UserDataType

      DataSpace

      Dataset

      Attribute

      Other
    """
    def __eq__(self, other: object) -> bool: ...
    def __ge__(self, other: object) -> bool: ...
    def __getstate__(self) -> int: ...
    def __gt__(self, other: object) -> bool: ...
    def __hash__(self) -> int: ...
    def __init__(self, value: int) -> None: ...
    def __int__(self) -> int: ...
    def __le__(self, other: object) -> bool: ...
    def __lt__(self, other: object) -> bool: ...
    def __ne__(self, other: object) -> bool: ...
    def __repr__(self) -> str: ...
    def __setstate__(self, state: int) -> None: ...
    @property
    def name(self) -> str:
        """
        :type: str
        """
    @property
    def value(self) -> int:
        """
        :type: int
        """
    Attribute: h5gtpy._h5gt.ObjectType # value = <ObjectType.Attribute: 5>
    DataSpace: h5gtpy._h5gt.ObjectType # value = <ObjectType.DataSpace: 3>
    Dataset: h5gtpy._h5gt.ObjectType # value = <ObjectType.Dataset: 4>
    File: h5gtpy._h5gt.ObjectType # value = <ObjectType.File: 0>
    Group: h5gtpy._h5gt.ObjectType # value = <ObjectType.Group: 1>
    Other: h5gtpy._h5gt.ObjectType # value = <ObjectType.Other: 6>
    UserDataType: h5gtpy._h5gt.ObjectType # value = <ObjectType.UserDataType: 2>
    __members__: dict # value = {'File': <ObjectType.File: 0>, 'Group': <ObjectType.Group: 1>, 'UserDataType': <ObjectType.UserDataType: 2>, 'DataSpace': <ObjectType.DataSpace: 3>, 'Dataset': <ObjectType.Dataset: 4>, 'Attribute': <ObjectType.Attribute: 5>, 'Other': <ObjectType.Other: 6>}
    pass
class OpenFlag():
    """
    Members:

      ReadOnly

      ReadWrite

      Truncate

      Excl

      Debug

      Create

      Overwrite

      OpenOrCreate
    """
    def __and__(self, other: object) -> object: ...
    def __eq__(self, other: object) -> bool: ...
    def __ge__(self, other: object) -> bool: ...
    def __getstate__(self) -> int: ...
    def __gt__(self, other: object) -> bool: ...
    def __hash__(self) -> int: ...
    def __init__(self, value: int) -> None: ...
    def __int__(self) -> int: ...
    def __invert__(self) -> object: ...
    def __le__(self, other: object) -> bool: ...
    def __lt__(self, other: object) -> bool: ...
    def __ne__(self, other: object) -> bool: ...
    def __or__(self, other: object) -> object: ...
    def __rand__(self, other: object) -> object: ...
    def __repr__(self) -> str: ...
    def __ror__(self, other: object) -> object: ...
    def __rxor__(self, other: object) -> object: ...
    def __setstate__(self, state: int) -> None: ...
    def __xor__(self, other: object) -> object: ...
    @property
    def name(self) -> str:
        """
        :type: str
        """
    @property
    def value(self) -> int:
        """
        :type: int
        """
    Create: h5gtpy._h5gt.OpenFlag # value = <OpenFlag.Create: 16>
    Debug: h5gtpy._h5gt.OpenFlag # value = <OpenFlag.Debug: 8>
    Excl: h5gtpy._h5gt.OpenFlag # value = <OpenFlag.Excl: 4>
    OpenOrCreate: h5gtpy._h5gt.OpenFlag # value = <OpenFlag.OpenOrCreate: 17>
    Overwrite: h5gtpy._h5gt.OpenFlag # value = <OpenFlag.Truncate: 2>
    ReadOnly: h5gtpy._h5gt.OpenFlag # value = <OpenFlag.ReadOnly: 0>
    ReadWrite: h5gtpy._h5gt.OpenFlag # value = <OpenFlag.ReadWrite: 1>
    Truncate: h5gtpy._h5gt.OpenFlag # value = <OpenFlag.Truncate: 2>
    __members__: dict # value = {'ReadOnly': <OpenFlag.ReadOnly: 0>, 'ReadWrite': <OpenFlag.ReadWrite: 1>, 'Truncate': <OpenFlag.Truncate: 2>, 'Excl': <OpenFlag.Excl: 4>, 'Debug': <OpenFlag.Debug: 8>, 'Create': <OpenFlag.Create: 16>, 'Overwrite': <OpenFlag.Truncate: 2>, 'OpenOrCreate': <OpenFlag.OpenOrCreate: 17>}
    pass
class Point():
    def __init__(self, arg0: float, arg1: float, arg2: float) -> None: ...
    def getX(self) -> float: ...
    def getY(self) -> float: ...
    def getZ(self) -> float: ...
    pass
class PropertyType():
    """
    Members:

      OBJECT_CREATE

      FILE_CREATE

      FILE_ACCESS

      DATASET_CREATE

      DATASET_ACCESS

      DATASET_XFER

      GROUP_CREATE

      GROUP_ACCESS

      DATATYPE_CREATE

      DATATYPE_ACCESS

      STRING_CREATE

      ATTRIBUTE_CREATE

      OBJECT_COPY

      LINK_CREATE

      LINK_ACCESS
    """
    def __eq__(self, other: object) -> bool: ...
    def __ge__(self, other: object) -> bool: ...
    def __getstate__(self) -> int: ...
    def __gt__(self, other: object) -> bool: ...
    def __hash__(self) -> int: ...
    def __init__(self, value: int) -> None: ...
    def __int__(self) -> int: ...
    def __le__(self, other: object) -> bool: ...
    def __lt__(self, other: object) -> bool: ...
    def __ne__(self, other: object) -> bool: ...
    def __repr__(self) -> str: ...
    def __setstate__(self, state: int) -> None: ...
    @property
    def name(self) -> str:
        """
        :type: str
        """
    @property
    def value(self) -> int:
        """
        :type: int
        """
    ATTRIBUTE_CREATE: h5gtpy._h5gt.PropertyType # value = <PropertyType.ATTRIBUTE_CREATE: 11>
    DATASET_ACCESS: h5gtpy._h5gt.PropertyType # value = <PropertyType.DATASET_ACCESS: 4>
    DATASET_CREATE: h5gtpy._h5gt.PropertyType # value = <PropertyType.DATASET_CREATE: 3>
    DATASET_XFER: h5gtpy._h5gt.PropertyType # value = <PropertyType.DATASET_XFER: 5>
    DATATYPE_ACCESS: h5gtpy._h5gt.PropertyType # value = <PropertyType.DATATYPE_ACCESS: 9>
    DATATYPE_CREATE: h5gtpy._h5gt.PropertyType # value = <PropertyType.DATATYPE_CREATE: 8>
    FILE_ACCESS: h5gtpy._h5gt.PropertyType # value = <PropertyType.FILE_ACCESS: 2>
    FILE_CREATE: h5gtpy._h5gt.PropertyType # value = <PropertyType.FILE_CREATE: 1>
    GROUP_ACCESS: h5gtpy._h5gt.PropertyType # value = <PropertyType.GROUP_ACCESS: 7>
    GROUP_CREATE: h5gtpy._h5gt.PropertyType # value = <PropertyType.GROUP_CREATE: 6>
    LINK_ACCESS: h5gtpy._h5gt.PropertyType # value = <PropertyType.LINK_ACCESS: 14>
    LINK_CREATE: h5gtpy._h5gt.PropertyType # value = <PropertyType.LINK_CREATE: 13>
    OBJECT_COPY: h5gtpy._h5gt.PropertyType # value = <PropertyType.OBJECT_COPY: 12>
    OBJECT_CREATE: h5gtpy._h5gt.PropertyType # value = <PropertyType.OBJECT_CREATE: 0>
    STRING_CREATE: h5gtpy._h5gt.PropertyType # value = <PropertyType.STRING_CREATE: 10>
    __members__: dict # value = {'OBJECT_CREATE': <PropertyType.OBJECT_CREATE: 0>, 'FILE_CREATE': <PropertyType.FILE_CREATE: 1>, 'FILE_ACCESS': <PropertyType.FILE_ACCESS: 2>, 'DATASET_CREATE': <PropertyType.DATASET_CREATE: 3>, 'DATASET_ACCESS': <PropertyType.DATASET_ACCESS: 4>, 'DATASET_XFER': <PropertyType.DATASET_XFER: 5>, 'GROUP_CREATE': <PropertyType.GROUP_CREATE: 6>, 'GROUP_ACCESS': <PropertyType.GROUP_ACCESS: 7>, 'DATATYPE_CREATE': <PropertyType.DATATYPE_CREATE: 8>, 'DATATYPE_ACCESS': <PropertyType.DATATYPE_ACCESS: 9>, 'STRING_CREATE': <PropertyType.STRING_CREATE: 10>, 'ATTRIBUTE_CREATE': <PropertyType.ATTRIBUTE_CREATE: 11>, 'OBJECT_COPY': <PropertyType.OBJECT_COPY: 12>, 'LINK_CREATE': <PropertyType.LINK_CREATE: 13>, 'LINK_ACCESS': <PropertyType.LINK_ACCESS: 14>}
    pass
class _SelectionSlice():
    def readBool(self) -> numpy.ndarray[bool, _Shape[m, n]]: ...
    def readCDouble(self) -> numpy.ndarray[numpy.complex128, _Shape[m, n]]: ...
    def readCFloat(self) -> numpy.ndarray[numpy.complex64, _Shape[m, n]]: ...
    def readChar(self) -> numpy.ndarray[numpy.int8, _Shape[m, n]]: ...
    def readDouble(self) -> numpy.ndarray[numpy.float64, _Shape[m, n]]: ...
    def readFloat(self) -> numpy.ndarray[numpy.float32, _Shape[m, n]]: ...
    def readInt(self) -> numpy.ndarray[numpy.int32, _Shape[m, n]]: ...
    def readLLong(self) -> numpy.ndarray[numpy.int64, _Shape[m, n]]: ...
    def readLong(self) -> numpy.ndarray[numpy.int32, _Shape[m, n]]: ...
    def readSChar(self) -> numpy.ndarray[numpy.int8, _Shape[m, n]]: ...
    def readShort(self) -> numpy.ndarray[numpy.int16, _Shape[m, n]]: ...
    def readStr(self) -> typing.List[str]: ...
    def readUChar(self) -> numpy.ndarray[numpy.uint8, _Shape[m, n]]: ...
    def readUInt(self) -> numpy.ndarray[numpy.uint32, _Shape[m, n]]: ...
    def readULLong(self) -> numpy.ndarray[numpy.uint64, _Shape[m, n]]: ...
    def readULong(self) -> numpy.ndarray[numpy.uint32, _Shape[m, n]]: ...
    def readUShort(self) -> numpy.ndarray[numpy.uint16, _Shape[m, n]]: ...
    @typing.overload
    def select(self, columns: typing.List[int]) -> Selection: ...
    @typing.overload
    def select(self, elements: ElementSet) -> Selection: ...
    @typing.overload
    def select(self, offset: typing.List[int], count: typing.List[int], stride: typing.List[int] = vector<size_t>()) -> Selection: ...
    @typing.overload
    def write(self, arg0: complex) -> None: 
        """
        An exception is raised if the numbers of dimension of the buffer and of the dataset are different
        """
    @typing.overload
    def write(self, arg0: float) -> None: ...
    @typing.overload
    def write(self, arg0: int) -> None: ...
    @typing.overload
    def write(self, arg0: numpy.ndarray[bool, _Shape[m, n]]) -> None: ...
    @typing.overload
    def write(self, arg0: numpy.ndarray[numpy.complex128, _Shape[m, n]]) -> None: ...
    @typing.overload
    def write(self, arg0: numpy.ndarray[numpy.complex64, _Shape[m, n]]) -> None: ...
    @typing.overload
    def write(self, arg0: numpy.ndarray[numpy.float32, _Shape[m, n]]) -> None: ...
    @typing.overload
    def write(self, arg0: numpy.ndarray[numpy.float64, _Shape[m, n]]) -> None: ...
    @typing.overload
    def write(self, arg0: numpy.ndarray[numpy.int16, _Shape[m, n]]) -> None: ...
    @typing.overload
    def write(self, arg0: numpy.ndarray[numpy.int32, _Shape[m, n]]) -> None: ...
    @typing.overload
    def write(self, arg0: numpy.ndarray[numpy.int64, _Shape[m, n]]) -> None: ...
    @typing.overload
    def write(self, arg0: numpy.ndarray[numpy.int8, _Shape[m, n]]) -> None: ...
    @typing.overload
    def write(self, arg0: numpy.ndarray[numpy.uint16, _Shape[m, n]]) -> None: ...
    @typing.overload
    def write(self, arg0: numpy.ndarray[numpy.uint32, _Shape[m, n]]) -> None: ...
    @typing.overload
    def write(self, arg0: numpy.ndarray[numpy.uint64, _Shape[m, n]]) -> None: ...
    @typing.overload
    def write(self, arg0: numpy.ndarray[numpy.uint8, _Shape[m, n]]) -> None: ...
    @typing.overload
    def write(self, arg0: str) -> None: ...
    @typing.overload
    def write(self, arg0: typing.List[complex]) -> None: ...
    @typing.overload
    def write(self, arg0: typing.List[float]) -> None: ...
    @typing.overload
    def write(self, arg0: typing.List[int]) -> None: ...
    @typing.overload
    def write(self, arg0: typing.List[str]) -> None: ...
    @typing.overload
    def write(self, data: bool) -> None: ...
    @typing.overload
    def write_raw(self, data: numpy.ndarray[bool, _Shape[m, n]]) -> None: 
        """
        No dimensionality checks will be performed
        """
    @typing.overload
    def write_raw(self, data: numpy.ndarray[numpy.complex128, _Shape[m, n]]) -> None: ...
    @typing.overload
    def write_raw(self, data: numpy.ndarray[numpy.complex64, _Shape[m, n]]) -> None: ...
    @typing.overload
    def write_raw(self, data: numpy.ndarray[numpy.float32, _Shape[m, n]]) -> None: ...
    @typing.overload
    def write_raw(self, data: numpy.ndarray[numpy.float64, _Shape[m, n]]) -> None: ...
    @typing.overload
    def write_raw(self, data: numpy.ndarray[numpy.int16, _Shape[m, n]]) -> None: ...
    @typing.overload
    def write_raw(self, data: numpy.ndarray[numpy.int32, _Shape[m, n]]) -> None: ...
    @typing.overload
    def write_raw(self, data: numpy.ndarray[numpy.int64, _Shape[m, n]]) -> None: ...
    @typing.overload
    def write_raw(self, data: numpy.ndarray[numpy.int8, _Shape[m, n]]) -> None: ...
    @typing.overload
    def write_raw(self, data: numpy.ndarray[numpy.uint16, _Shape[m, n]]) -> None: ...
    @typing.overload
    def write_raw(self, data: numpy.ndarray[numpy.uint32, _Shape[m, n]]) -> None: ...
    @typing.overload
    def write_raw(self, data: numpy.ndarray[numpy.uint64, _Shape[m, n]]) -> None: ...
    @typing.overload
    def write_raw(self, data: numpy.ndarray[numpy.uint8, _Shape[m, n]]) -> None: ...
    @typing.overload
    def write_raw(self, data: typing.List[complex]) -> None: ...
    @typing.overload
    def write_raw(self, data: typing.List[float]) -> None: ...
    @typing.overload
    def write_raw(self, data: typing.List[int]) -> None: ...
    pass
class _DsetSlice():
    def readBool(self) -> numpy.ndarray[bool, _Shape[m, n]]: ...
    def readCDouble(self) -> numpy.ndarray[numpy.complex128, _Shape[m, n]]: ...
    def readCFloat(self) -> numpy.ndarray[numpy.complex64, _Shape[m, n]]: ...
    def readChar(self) -> numpy.ndarray[numpy.int8, _Shape[m, n]]: ...
    def readDouble(self) -> numpy.ndarray[numpy.float64, _Shape[m, n]]: ...
    def readFloat(self) -> numpy.ndarray[numpy.float32, _Shape[m, n]]: ...
    def readInt(self) -> numpy.ndarray[numpy.int32, _Shape[m, n]]: ...
    def readLLong(self) -> numpy.ndarray[numpy.int64, _Shape[m, n]]: ...
    def readLong(self) -> numpy.ndarray[numpy.int32, _Shape[m, n]]: ...
    def readSChar(self) -> numpy.ndarray[numpy.int8, _Shape[m, n]]: ...
    def readShort(self) -> numpy.ndarray[numpy.int16, _Shape[m, n]]: ...
    def readStr(self) -> typing.List[str]: ...
    def readUChar(self) -> numpy.ndarray[numpy.uint8, _Shape[m, n]]: ...
    def readUInt(self) -> numpy.ndarray[numpy.uint32, _Shape[m, n]]: ...
    def readULLong(self) -> numpy.ndarray[numpy.uint64, _Shape[m, n]]: ...
    def readULong(self) -> numpy.ndarray[numpy.uint32, _Shape[m, n]]: ...
    def readUShort(self) -> numpy.ndarray[numpy.uint16, _Shape[m, n]]: ...
    @typing.overload
    def select(self, columns: typing.List[int]) -> Selection: ...
    @typing.overload
    def select(self, elements: ElementSet) -> Selection: ...
    @typing.overload
    def select(self, offset: typing.List[int], count: typing.List[int], stride: typing.List[int] = vector<size_t>()) -> Selection: ...
    @typing.overload
    def write(self, arg0: complex) -> None: 
        """
        An exception is raised if the numbers of dimension of the buffer and of the dataset are different
        """
    @typing.overload
    def write(self, arg0: float) -> None: ...
    @typing.overload
    def write(self, arg0: int) -> None: ...
    @typing.overload
    def write(self, arg0: numpy.ndarray[bool, _Shape[m, n]]) -> None: ...
    @typing.overload
    def write(self, arg0: numpy.ndarray[numpy.complex128, _Shape[m, n]]) -> None: ...
    @typing.overload
    def write(self, arg0: numpy.ndarray[numpy.complex64, _Shape[m, n]]) -> None: ...
    @typing.overload
    def write(self, arg0: numpy.ndarray[numpy.float32, _Shape[m, n]]) -> None: ...
    @typing.overload
    def write(self, arg0: numpy.ndarray[numpy.float64, _Shape[m, n]]) -> None: ...
    @typing.overload
    def write(self, arg0: numpy.ndarray[numpy.int16, _Shape[m, n]]) -> None: ...
    @typing.overload
    def write(self, arg0: numpy.ndarray[numpy.int32, _Shape[m, n]]) -> None: ...
    @typing.overload
    def write(self, arg0: numpy.ndarray[numpy.int64, _Shape[m, n]]) -> None: ...
    @typing.overload
    def write(self, arg0: numpy.ndarray[numpy.int8, _Shape[m, n]]) -> None: ...
    @typing.overload
    def write(self, arg0: numpy.ndarray[numpy.uint16, _Shape[m, n]]) -> None: ...
    @typing.overload
    def write(self, arg0: numpy.ndarray[numpy.uint32, _Shape[m, n]]) -> None: ...
    @typing.overload
    def write(self, arg0: numpy.ndarray[numpy.uint64, _Shape[m, n]]) -> None: ...
    @typing.overload
    def write(self, arg0: numpy.ndarray[numpy.uint8, _Shape[m, n]]) -> None: ...
    @typing.overload
    def write(self, arg0: str) -> None: ...
    @typing.overload
    def write(self, arg0: typing.List[complex]) -> None: ...
    @typing.overload
    def write(self, arg0: typing.List[float]) -> None: ...
    @typing.overload
    def write(self, arg0: typing.List[int]) -> None: ...
    @typing.overload
    def write(self, arg0: typing.List[str]) -> None: ...
    @typing.overload
    def write(self, data: bool) -> None: ...
    @typing.overload
    def write_raw(self, data: numpy.ndarray[bool, _Shape[m, n]]) -> None: 
        """
        No dimensionality checks will be performed
        """
    @typing.overload
    def write_raw(self, data: numpy.ndarray[numpy.complex128, _Shape[m, n]]) -> None: ...
    @typing.overload
    def write_raw(self, data: numpy.ndarray[numpy.complex64, _Shape[m, n]]) -> None: ...
    @typing.overload
    def write_raw(self, data: numpy.ndarray[numpy.float32, _Shape[m, n]]) -> None: ...
    @typing.overload
    def write_raw(self, data: numpy.ndarray[numpy.float64, _Shape[m, n]]) -> None: ...
    @typing.overload
    def write_raw(self, data: numpy.ndarray[numpy.int16, _Shape[m, n]]) -> None: ...
    @typing.overload
    def write_raw(self, data: numpy.ndarray[numpy.int32, _Shape[m, n]]) -> None: ...
    @typing.overload
    def write_raw(self, data: numpy.ndarray[numpy.int64, _Shape[m, n]]) -> None: ...
    @typing.overload
    def write_raw(self, data: numpy.ndarray[numpy.int8, _Shape[m, n]]) -> None: ...
    @typing.overload
    def write_raw(self, data: numpy.ndarray[numpy.uint16, _Shape[m, n]]) -> None: ...
    @typing.overload
    def write_raw(self, data: numpy.ndarray[numpy.uint32, _Shape[m, n]]) -> None: ...
    @typing.overload
    def write_raw(self, data: numpy.ndarray[numpy.uint64, _Shape[m, n]]) -> None: ...
    @typing.overload
    def write_raw(self, data: numpy.ndarray[numpy.uint8, _Shape[m, n]]) -> None: ...
    @typing.overload
    def write_raw(self, data: typing.List[complex]) -> None: ...
    @typing.overload
    def write_raw(self, data: typing.List[float]) -> None: ...
    @typing.overload
    def write_raw(self, data: typing.List[int]) -> None: ...
    pass
class _FileNode():
    @typing.overload
    def createDataSet(self, dset_name: str, dim: int, type: DataType, linkCreateProps: LinkCreateProps = LinkCreateProps(), dsetCreateProps: DataSetCreateProps = DataSetCreateProps(), dsetAccessProps: DataSetAccessProps = DataSetAccessProps()) -> DataSet: ...
    @typing.overload
    def createDataSet(self, dset_name: str, dims: typing.List[int], type: DataType, linkCreateProps: LinkCreateProps = LinkCreateProps(), dsetCreateProps: DataSetCreateProps = DataSetCreateProps(), dsetAccessProps: DataSetAccessProps = DataSetAccessProps()) -> DataSet: ...
    @typing.overload
    def createDataSet(self, dset_name: str, space: DataSpace, type: DataType, linkCreateProps: LinkCreateProps = LinkCreateProps(), dsetCreateProps: DataSetCreateProps = DataSetCreateProps(), dsetAccessProps: DataSetAccessProps = DataSetAccessProps()) -> DataSet: ...
    def createGroup(self, group_name: str, linkCreateProps: LinkCreateProps = LinkCreateProps(), groupCreateProps: GroupCreateProps = GroupCreateProps(), groupAccessProps: GroupAccessProps = GroupAccessProps()) -> Group: 
        """
        Create a new group, and eventually intermediate groups
        """
    @typing.overload
    def createLink(self, dset: DataSet, linkName: str, linkType: LinkType, linkCreateProps: LinkCreateProps = LinkCreateProps(), linkAccesProps: LinkAccessProps = LinkAccessProps(), dsetAccessProps: DataSetAccessProps = DataSetAccessProps()) -> DataSet: 
        """
        Creates link to a `File`

        Creates link to a `Group`

        Creates link to a `DataSet`
        """
    @typing.overload
    def createLink(self, file: File, linkName: str, linkType: LinkType, linkCreateProps: LinkCreateProps = LinkCreateProps(), linkAccesProps: LinkAccessProps = LinkAccessProps(), groupAccessProps: GroupAccessProps = GroupAccessProps()) -> Group: ...
    @typing.overload
    def createLink(self, group: Group, linkName: str, linkType: LinkType, linkCreateProps: LinkCreateProps = LinkCreateProps(), linkAccesProps: LinkAccessProps = LinkAccessProps(), groupAccessProps: GroupAccessProps = GroupAccessProps()) -> Group: ...
    def exist(self, obj_name: str, linkAccessProps: LinkAccessProps = LinkAccessProps()) -> bool: 
        """
        Check a dataset or group exists in the current node / group
        """
    def getDataSet(self, dset_name: str, dsetAccessProps: DataSetAccessProps = DataSetAccessProps()) -> DataSet: 
        """
        Get an existing dataset in the current file
        """
    def getDataType(self, dtype_name: str, dtypeAccessProps: DataTypeAccessProps = DataTypeAccessProps()) -> DataType: ...
    def getGroup(self, group_name: str, groupAccessProps: GroupAccessProps = GroupAccessProps()) -> Group: 
        """
        return the number of leaf objects of the node / group
        """
    def getLinkType(self, obj_name: str, linkAccessProps: LinkAccessProps = LinkAccessProps()) -> LinkType: 
        """
        Unlink the given dataset or group. NOTE: unlink doesn't frees memory. Use `h5repack` to free unused space
        """
    def getNumberObjects(self) -> int: ...
    def getObjectName(self, index: int, linkAccessProps: LinkAccessProps = LinkAccessProps()) -> str: ...
    def getObjectType(self, obj_name: str, linkAccessProps: LinkAccessProps = LinkAccessProps()) -> ObjectType: 
        """
        A shorthand to get the kind of object pointed to (group, dataset, type...)
        """
    def hasObject(self, objName: str, objectType: ObjectType, linkAccessProps: LinkAccessProps = LinkAccessProps()) -> bool: 
        """
        Advanced version of `exist` that also checks the type of object
        """
    def listObjectNames(self) -> typing.List[str]: ...
    def rename(self, src_path: str, dest_path: str, linkCreateProps: LinkCreateProps = LinkCreateProps(), linkAccessProps: LinkAccessProps = LinkAccessProps()) -> bool: 
        """
        Moves an object and its content within an HDF5 file.
        """
    def unlink(self, obj_name: str, linkAccessProps: LinkAccessProps = LinkAccessProps()) -> None: ...
    pass
class _GroupNode():
    @typing.overload
    def createDataSet(self, dset_name: str, dim: int, type: DataType, linkCreateProps: LinkCreateProps = LinkCreateProps(), dsetCreateProps: DataSetCreateProps = DataSetCreateProps(), dsetAccessProps: DataSetAccessProps = DataSetAccessProps()) -> DataSet: ...
    @typing.overload
    def createDataSet(self, dset_name: str, dims: typing.List[int], type: DataType, linkCreateProps: LinkCreateProps = LinkCreateProps(), dsetCreateProps: DataSetCreateProps = DataSetCreateProps(), dsetAccessProps: DataSetAccessProps = DataSetAccessProps()) -> DataSet: ...
    @typing.overload
    def createDataSet(self, dset_name: str, space: DataSpace, type: DataType, linkCreateProps: LinkCreateProps = LinkCreateProps(), dsetCreateProps: DataSetCreateProps = DataSetCreateProps(), dsetAccessProps: DataSetAccessProps = DataSetAccessProps()) -> DataSet: ...
    def createGroup(self, group_name: str, linkCreateProps: LinkCreateProps = LinkCreateProps(), groupCreateProps: GroupCreateProps = GroupCreateProps(), groupAccessProps: GroupAccessProps = GroupAccessProps()) -> Group: 
        """
        Create a new group, and eventually intermediate groups
        """
    @typing.overload
    def createLink(self, dset: DataSet, linkName: str, linkType: LinkType, linkCreateProps: LinkCreateProps = LinkCreateProps(), linkAccesProps: LinkAccessProps = LinkAccessProps(), dsetAccessProps: DataSetAccessProps = DataSetAccessProps()) -> DataSet: 
        """
        Creates link to a `File`

        Creates link to a `Group`

        Creates link to a `DataSet`
        """
    @typing.overload
    def createLink(self, file: File, linkName: str, linkType: LinkType, linkCreateProps: LinkCreateProps = LinkCreateProps(), linkAccesProps: LinkAccessProps = LinkAccessProps(), groupAccessProps: GroupAccessProps = GroupAccessProps()) -> Group: ...
    @typing.overload
    def createLink(self, group: Group, linkName: str, linkType: LinkType, linkCreateProps: LinkCreateProps = LinkCreateProps(), linkAccesProps: LinkAccessProps = LinkAccessProps(), groupAccessProps: GroupAccessProps = GroupAccessProps()) -> Group: ...
    def exist(self, obj_name: str, linkAccessProps: LinkAccessProps = LinkAccessProps()) -> bool: 
        """
        Check a dataset or group exists in the current node / group
        """
    def getDataSet(self, dset_name: str, dsetAccessProps: DataSetAccessProps = DataSetAccessProps()) -> DataSet: 
        """
        Get an existing dataset in the current file
        """
    def getDataType(self, dtype_name: str, dtypeAccessProps: DataTypeAccessProps = DataTypeAccessProps()) -> DataType: ...
    def getGroup(self, group_name: str, groupAccessProps: GroupAccessProps = GroupAccessProps()) -> Group: 
        """
        return the number of leaf objects of the node / group
        """
    def getLinkType(self, obj_name: str, linkAccessProps: LinkAccessProps = LinkAccessProps()) -> LinkType: 
        """
        Unlink the given dataset or group. NOTE: unlink doesn't frees memory. Use `h5repack` to free unused space
        """
    def getNumberObjects(self) -> int: ...
    def getObjectName(self, index: int, linkAccessProps: LinkAccessProps = LinkAccessProps()) -> str: ...
    def getObjectType(self, obj_name: str, linkAccessProps: LinkAccessProps = LinkAccessProps()) -> ObjectType: 
        """
        A shorthand to get the kind of object pointed to (group, dataset, type...)
        """
    def hasObject(self, objName: str, objectType: ObjectType, linkAccessProps: LinkAccessProps = LinkAccessProps()) -> bool: 
        """
        Advanced version of `exist` that also checks the type of object
        """
    def listObjectNames(self) -> typing.List[str]: ...
    def rename(self, src_path: str, dest_path: str, linkCreateProps: LinkCreateProps = LinkCreateProps(), linkAccessProps: LinkAccessProps = LinkAccessProps()) -> bool: 
        """
        Moves an object and its content within an HDF5 file.
        """
    def unlink(self, obj_name: str, linkAccessProps: LinkAccessProps = LinkAccessProps()) -> None: ...
    pass
class DataSetAccessProps(_DataSetAccessProps):
    def __init__(self) -> None: ...
    def setChunkCache(self, numSlots: int, cacheSize: int, w0: float = static_cast<double>(H5D_CHUNK_CACHE_W0_DEFAULT)) -> None: ...
    def setExternalLinkPrefix(self, prefix: str) -> None: ...
    pass
class DataSetCreateProps(_DataSetCreateProps):
    def __init__(self) -> None: ...
    @typing.overload
    def setChunk(self, dim: int) -> None: ...
    @typing.overload
    def setChunk(self, dims: typing.List[int]) -> None: ...
    def setShuffle(self) -> None: ...
    pass
class DataTypeAccessProps(_DataTypeAccessProps):
    def __init__(self) -> None: ...
    pass
class DataTypeCreateProps(_DataTypeCreateProps):
    def __init__(self) -> None: ...
    pass
class DataSet(Object, _DsetSlice, _AnnotateDataSet):
    @staticmethod
    def FromId(id: int, increaseRefCount: bool = false) -> DataSet: 
        """
        Create new object from ID
        """
    def __eq__(self, arg0: DataSet) -> bool: ...
    def __ne__(self, arg0: DataSet) -> bool: ...
    def getDimensions(self) -> typing.List[int]: 
        """
        Get the dimensions of the whole DataSet.This is a shorthand for getSpace().getDimensions()
        """
    def getElementCount(self) -> int: 
        """
        Get the total number of elements in the current dataset.E.g. 2x2x2 matrix has size 8. This is a shorthand for getSpace().getTotalCount()
        """
    def getLinkInfo(self) -> LinkInfo: 
        """
        return the link info object
        """
    def getMemSpace(self) -> DataSpace: 
        """
        same as getSpace for DataSet, compatibility with Selection
        """
    @typing.overload
    def getOffset(self) -> DataType: 
        """
        returns DataSet address in file

        return the datatype associated with this dataset
        """
    @typing.overload
    def getOffset(self) -> int: ...
    def getParent(self, groupAccessProps: GroupAccessProps = GroupAccessProps()) -> Group: ...
    def getSpace(self) -> DataSpace: 
        """
        return the dataspace associated with this dataset
        """
    def getStorageSize(self) -> int: 
        """
        returns the amount of storage allocated for a dataset
        """
    def getTargetPath(self, accessProp: LinkAccessProps = LinkAccessProps()) -> str: 
        """
        getTargetPath For soft link that returns path to target thatlink points to. Otherwise it works the same way as `getPath()`
        """
    def resize(self, dims: typing.List[int]) -> None: 
        """
        This requires that the dataset was created with chunking, and you wouldgenerally want to have set a larger maxdims setting
        """
    __hash__ = None
    pass
class FileAccessProps(_FileAccessProps):
    def __init__(self) -> None: ...
    pass
class FileCreateProps(_FileCreateProps):
    def __init__(self) -> None: ...
    pass
class File(Object, _FileNode, _AnnotateFile):
    @staticmethod
    def FromId(id: int, increaseRefCount: bool = false) -> File: 
        """
        Create new object from ID
        """
    def __eq__(self, arg0: File) -> bool: ...
    def __init__(self, file_name: str, open_flag: OpenFlag) -> None: 
        """
        Open or create new h5 file
        """
    def __ne__(self, arg0: File) -> bool: ...
    def flush(self) -> None: 
        """
        Flushes all buffers associated with a file to disk
        """
    __hash__ = None
    pass
class GroupAccessProps(_GroupAccessProps):
    def __init__(self) -> None: ...
    def setExternalLinkPrefix(self, prefix: str) -> None: ...
    pass
class GroupCreateProps(_GroupCreateProps):
    def __init__(self) -> None: ...
    pass
class Group(Object, _GroupNode, _AnnotateGroup):
    @staticmethod
    def FromId(id: int, increaseRefCount: bool = false) -> Group: 
        """
        Create new object from ID
        """
    def __eq__(self, arg0: Group) -> bool: 
        """
        Check if objects reside in the same file and equal to each other
        """
    def __ne__(self, arg0: Group) -> bool: ...
    def getLinkInfo(self) -> LinkInfo: 
        """
        returns link info object
        """
    def getParent(self, groupAccessProps: GroupAccessProps = GroupAccessProps()) -> Group: 
        """
        returns parent `Group`
        """
    def getTargetPath(self, arg0: LinkAccessProps) -> str: 
        """
        getTargetPath For soft link that returns path to target thatlink points to. Otherwise it works the same way as `getPath()`
        """
    __hash__ = None
    pass
class LinkAccessProps(_LinkAccessProps):
    def __init__(self) -> None: ...
    def setExternalLinkPrefix(self, prefix: str) -> None: ...
    pass
class LinkCreateProps(_LinkCreateProps):
    def __init__(self) -> None: ...
    def setCreateIntermediateGroup(self, arg0: int) -> None: ...
    pass
class Selection(_SelectionSlice):
    def getDataType(self) -> DataType: 
        """
        return the datatype of the selection
        """
    def getDataset(self) -> DataSet: 
        """
        parent dataset of this selection
        """
    def getMemSpace(self) -> DataSpace: 
        """
        Dataspace associated with the memory representation of this selection
        """
    def getSpace(self) -> DataSpace: 
        """
        Dataspace associated with this selection
        """
    pass
Create: h5gtpy._h5gt.OpenFlag # value = <OpenFlag.Create: 16>
Debug: h5gtpy._h5gt.OpenFlag # value = <OpenFlag.Debug: 8>
Excl: h5gtpy._h5gt.OpenFlag # value = <OpenFlag.Excl: 4>
OpenOrCreate: h5gtpy._h5gt.OpenFlag # value = <OpenFlag.OpenOrCreate: 17>
Overwrite: h5gtpy._h5gt.OpenFlag # value = <OpenFlag.Truncate: 2>
ReadOnly: h5gtpy._h5gt.OpenFlag # value = <OpenFlag.ReadOnly: 0>
ReadWrite: h5gtpy._h5gt.OpenFlag # value = <OpenFlag.ReadWrite: 1>
Truncate: h5gtpy._h5gt.OpenFlag # value = <OpenFlag.Truncate: 2>
