#ifndef H5GTPY_H
#define H5GTPY_H

#ifndef H5GT_USE_EIGEN
#define H5GT_USE_EIGEN // should be defined before including h5gt
#endif

#include <h5gt/H5Object.hpp>
#include <h5gt/H5File.hpp>
#include <h5gt/H5DataSet.hpp>
#include <h5gt/H5DataSpace.hpp>

#include <Eigen/Dense>

#include <pybind11/pybind11.h>
#include <pybind11/eigen.h>
#include <pybind11/stl.h>
#include <pybind11/stl_bind.h>
#include <pybind11/complex.h>
#include <pybind11/operators.h>

using namespace h5gt;

namespace py = pybind11;

// opaque types must be included to every translation unit or ODR falls (warnings is given)
PYBIND11_MAKE_OPAQUE(std::vector<CompoundType::member_def>);
PYBIND11_MAKE_OPAQUE(std::vector<EnumType<int>::member_def>);
PYBIND11_MAKE_OPAQUE(std::vector<EnumType<unsigned>::member_def>);
PYBIND11_MAKE_OPAQUE(std::vector<EnumType<long long>::member_def>);
PYBIND11_MAKE_OPAQUE(std::vector<EnumType<unsigned long long>::member_def>);

/* to generate .pyi the returned type should be declared before it
 * is called. For example `createGroup` returns `Group` so I need to
 * place `py::class_<Group, Object>(m, "Group")`
 * before calling `.def("createGroup", ...` ) */

/* py::arithmetic() -> create an enumeration that also supports
 * rudimentary arithmetic and bit-level operations like
 * comparisons, and, or, xor, negation, etc. */

/* export_values() -> export the enum entries into the parent scope,
 * which should be skipped for newer C++11-style strongly typed enums */

/* py::const_ -> is neccessary when binding overoaded functions */

/* All declarations are here. And all functions are invoked in `h5gt.cpp` */


#endif // H5GTPY_H
