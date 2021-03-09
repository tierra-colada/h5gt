#ifndef H5_TEST_SIMPLETON_HPP
#define H5_TEST_SIMPLETON_HPP

// Include all headers here to catch any missing `inline` statements, since
// they will be included by two different compilation units.
#include <h5gt/H5Attribute.hpp>
#include <h5gt/H5DataSet.hpp>
#include <h5gt/H5DataSpace.hpp>
#include <h5gt/H5DataType.hpp>
#include <h5gt/H5Easy.hpp>
#include <h5gt/H5Exception.hpp>
#include <h5gt/H5File.hpp>
#include <h5gt/H5FileDriver.hpp>
#include <h5gt/H5Group.hpp>
#include <h5gt/H5Object.hpp>
#include <h5gt/H5PropertyList.hpp>
#include <h5gt/H5Reference.hpp>
#include <h5gt/H5Selection.hpp>
#include <h5gt/H5Utility.hpp>

// Boost should always be found in this setup
#include <boost/numeric/ublas/matrix.hpp>

void function(const h5gt::Object& obj);
void other_function(const boost::numeric::ublas::matrix<double>& m);

#endif
