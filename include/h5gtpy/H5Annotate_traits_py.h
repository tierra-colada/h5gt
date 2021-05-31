#include "h5gtpy.h"

namespace h5gtpy {

namespace ext {

template <typename Derivate>
Attribute createAttribute_wrap1(
    AnnotateTraits<Derivate>& self,
    const std::string& attr_name,
    const DataSpace& space,
    const DataType& type){
  return self.createAttribute(attr_name, space, type);
}

template <typename Derivate>
Attribute createAttribute_wrap2(
    AnnotateTraits<Derivate>& self,
    const std::string& attr_name,
    const size_t& dim,
    const DataType& type){
  return self.createAttribute(attr_name, DataSpace(dim), type);
}


} // ext


template <typename Derivate>
void AnnotateTraits_py(py::class_<AnnotateTraits<Derivate> > &py_obj){
  py_obj
      .def("createAttribute", &ext::createAttribute_wrap1<Derivate>,
           py::arg("attr_name"),
           py::arg("space"),
           py::arg("type"),
           "create a new attribute with the name attr_name")
      .def("createAttribute", &ext::createAttribute_wrap2<Derivate>,
           py::arg("attr_name"),
           py::arg("dim"),
           py::arg("type"))
      .def("deleteAttribute", &AnnotateTraits<Derivate>::deleteAttribute,
           py::arg("attr_name"),
           "let you delete an attribute by its name.")
      .def("getAttribute", &AnnotateTraits<Derivate>::getAttribute,
           py::arg("attr_name"),
           "open an existing attribute with the name attr_name")
      .def("getNumberAttributes", &AnnotateTraits<Derivate>::getNumberAttributes,
           "return the number of attributes of the object")
      .def("listAttributeNames", &AnnotateTraits<Derivate>::listAttributeNames,
           "list all attribute name of the object")
      .def("hasAttribute", &AnnotateTraits<Derivate>::hasAttribute,
           py::arg("attr_name"),
           "open an existing attribute with the name attr_name");
}


} // h5gtpy
