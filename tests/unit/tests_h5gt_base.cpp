/*
 *  Copyright (c), 2017-2019, Blue Brain Project - EPFL
 *
 *  Distributed under the Boost Software License, Version 1.0.
 *    (See accompanying file LICENSE_1_0.txt or copy at
 *          http://www.boost.org/LICENSE_1_0.txt)
 *
 */

#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <memory>
#include <random>
#include <string>
#include <typeinfo>
#include <vector>
#include <fstream>

#include <h5gt/H5DataSet.hpp>
#include <h5gt/H5DataSpace.hpp>
#include <h5gt/H5File.hpp>
#include <h5gt/H5Group.hpp>
#include <h5gt/H5Reference.hpp>
#include <h5gt/H5Utility.hpp>

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "tests_h5gt.hpp"

using namespace h5gt;

TEST(H5GTBase, Basic) {
  const std::string FILE_NAME("h5tutr_dset.h5");
  const std::string DATASET_NAME("dset");

  // Create a new file using the default property lists.
  File file(FILE_NAME, File::ReadWrite | File::Create | File::Truncate);

  EXPECT_EQ(file.getFileName(), FILE_NAME);

  // Create the data space for the dataset.
  std::vector<size_t> dims{4, 6};

  DataSpace dataspace(dims);

  // check if the dataset exist
  bool dataset_exist = file.exist(DATASET_NAME + "_double");
  ASSERT_TRUE(!dataset_exist);

  // Create a dataset with double precision floating points
  DataSet dataset_double = file.createDataSet(DATASET_NAME + "_double", dataspace,
                                              AtomicType<double>());

  EXPECT_EQ(file.getObjectName(0), DATASET_NAME + "_double");

  DataSet dataset_vec = file.createDataSet(DATASET_NAME + "_vec", {2, 3},
                                              AtomicType<double>());

  EXPECT_TRUE(file.exist(DATASET_NAME + "_vec"));

//  auto atomic = create_atomic(AtomicClass::AtomicChar);

//  auto atomic = AtomicType<AtomicClass::AtomicChar>(AtomicClass::AtomicChar);

//  AtomicType<AtomicClass::AtomicDouble> atomic = AtomicType<AtomicClass::AtomicDouble>::FromClass(AtomicClass::AtomicDouble);



  {
    // check if it exist again
    dataset_exist = file.exist(DATASET_NAME + "_double");
    EXPECT_EQ(dataset_exist, true);

    // and also try to recreate it to the sake of exception testing
    SilenceHDF5 silencer;
    EXPECT_THROW(
          {
            DataSet fail_duplicated = file.createDataSet(
            DATASET_NAME + "_double", dataspace, AtomicType<double>());
          },
          DataSetException);
  }

  DataSet dataset_size_t = file.createDataSet<size_t>(DATASET_NAME + "_size_t",
                                                      dataspace);
}

TEST(H5GTBase, Silence) {
  // Setting up a buffer for stderr so we can detect if the stack trace
  // was disabled
  fflush(stderr);
  char buffer[1024];
  memset(buffer, 0, sizeof(char) * 1024);
  setvbuf(stderr, buffer, _IOLBF, 1023);

  try {
    SilenceHDF5 silence;
    File file("nonexistent", File::ReadOnly);
  } catch (const FileException&) {
  }
  EXPECT_EQ(buffer[0], '\0');

  // restore the dyn allocated buffer
  // or using stderr will segfault when buffer get out of scope
  fflush(stderr);
  setvbuf(stderr, NULL, _IONBF, 0);
}

TEST(H5GTBase, OpenMode) {
  const std::string FILE_NAME("openmodes.h5");
  const std::string DATASET_NAME("dset");

  std::remove(FILE_NAME.c_str());

  SilenceHDF5 silencer;

  // Attempt open file only ReadWrite should fail (wont create)
  EXPECT_THROW({ File file(FILE_NAME, File::ReadWrite); }, FileException);

  // But with Create flag should be fine
  { File file(FILE_NAME, File::ReadWrite | File::Create); }

  // But if its there and exclusive is given, should fail
  EXPECT_THROW({ File file(FILE_NAME, File::ReadWrite | File::Excl); },
               FileException);
  // ReadWrite and Excl flags are fine together (posix)
  std::remove(FILE_NAME.c_str());
  { File file(FILE_NAME, File::ReadWrite | File::Excl); }
  // All three are fine as well (as long as the file does not exist)
  std::remove(FILE_NAME.c_str());
  { File file(FILE_NAME, File::ReadWrite | File::Create | File::Excl); }

  // Just a few combinations are incompatible, detected by hdf5lib
  EXPECT_THROW({ File file(FILE_NAME, File::Truncate | File::Excl); },
               FileException);

  std::remove(FILE_NAME.c_str());
  EXPECT_THROW({ File file(FILE_NAME, File::Truncate | File::Excl); },
               FileException);

  // But in most cases we will truncate and that should always work
  { File file(FILE_NAME, File::Truncate); }
  std::remove(FILE_NAME.c_str());
  { File file(FILE_NAME, File::Truncate); }

  // Last but not least, defaults should be ok
  { File file(FILE_NAME); }     // ReadOnly
  { File file(FILE_NAME, 0); }  // force empty-flags, does open without flags
}

TEST(H5GTBase, GroupAndDataSet) {
  const std::string FILE_NAME("h5_group_test.h5");
  const std::string DATASET_NAME("dset");
  const std::string CHUNKED_DATASET_NAME("chunked_dset");
  const std::string CHUNKED_DATASET_SMALL_NAME("chunked_dset_small");
  const std::string GROUP_NAME1("/group1");
  const std::string GROUP_NAME2("group2");
  const std::string GROUP_NESTED_NAME("group_nested");

  {
    // Create a new file using the default property lists.
    File file(FILE_NAME, File::ReadWrite | File::Create | File::Truncate);

    // absolute group
    file.createGroup(GROUP_NAME1);
    // nested group absolute
    file.createGroup(GROUP_NAME1 + "/" + GROUP_NESTED_NAME);
    // relative group
    Group g1 = file.createGroup(GROUP_NAME2);
    // relative group
    Group nested = g1.createGroup(GROUP_NESTED_NAME);

    // Create the data space for the dataset.
    std::vector<size_t> dims{4, 6};

    DataSpace dataspace(dims);

    DataSet dataset_absolute = file.createDataSet(
          GROUP_NAME1 + "/" + GROUP_NESTED_NAME + "/" + DATASET_NAME, dataspace,
          AtomicType<double>());

    DataSet dataset_relative = nested.createDataSet(DATASET_NAME, dataspace,
                                                    AtomicType<double>());

    DataSetCreateProps goodChunking;
    goodChunking.setChunk(std::vector<hsize_t>{2, 2});
    DataSetAccessProps cacheConfig;
    cacheConfig.setChunkCache(13, 1024, 0.5);

    // will fail because exceeds dimensions
    DataSetCreateProps badChunking0;
    badChunking0.setChunk(std::vector<hsize_t>{10, 10});

    DataSetCreateProps badChunking1;
    badChunking1.setChunk(std::vector<hsize_t>{1, 1, 1});

    {
      SilenceHDF5 silencer;
      EXPECT_THROW(file.createDataSet(CHUNKED_DATASET_NAME, dataspace, AtomicType<double>(),
                                      LinkCreateProps(), badChunking0),
                   DataSetException);

      EXPECT_THROW(file.createDataSet(CHUNKED_DATASET_NAME, dataspace,
                                      AtomicType<double>(), LinkCreateProps(), badChunking1),
                   DataSetException);
    }

    // here we use the other signature
    DataSet dataset_chunked = file.createDataSet<float>(
          CHUNKED_DATASET_NAME, dataspace, LinkCreateProps(), goodChunking, cacheConfig);

    // Here we resize to smaller than the chunking size
    DataSet dataset_chunked_small = file.createDataSet<float>(
          CHUNKED_DATASET_SMALL_NAME, dataspace, LinkCreateProps(), goodChunking);

    dataset_chunked_small.resize({1, 1});
  }
  // read it back
  {
    File file(FILE_NAME, File::ReadOnly);
    Group g1 = file.getGroup(GROUP_NAME1);
    Group g2 = file.getGroup(GROUP_NAME2);
    Group nested_group2 = g2.getGroup(GROUP_NESTED_NAME);

    DataSet dataset_absolute = file.getDataSet(GROUP_NAME1 + "/" + GROUP_NESTED_NAME +
                                               "/" + DATASET_NAME);
    EXPECT_EQ(4, dataset_absolute.getSpace().getDimensions()[0]);

    DataSet dataset_relative = nested_group2.getDataSet(DATASET_NAME);
    EXPECT_EQ(4, dataset_relative.getSpace().getDimensions()[0]);

    DataSetAccessProps accessProps;
    accessProps.setChunkCache(13, 1024, 0.5);
    DataSet dataset_chunked = file.getDataSet(CHUNKED_DATASET_NAME, accessProps);
    EXPECT_EQ(4, dataset_chunked.getSpace().getDimensions()[0]);

    DataSet dataset_chunked_small = file.getDataSet(CHUNKED_DATASET_SMALL_NAME);
    EXPECT_EQ(1, dataset_chunked_small.getSpace().getDimensions()[0]);
  }
}

TEST(H5GTBase, ExtensibleDataSet) {
  const std::string FILE_NAME("create_extensible_dataset_example.h5");
  const std::string DATASET_NAME("dset");
  constexpr double t1[3][1] = {{2.0}, {2.0}, {4.0}};
  constexpr double t2[1][3] = {{4.0, 8.0, 6.0}};

  {
    // Create a new file using the default property lists.
    File file(FILE_NAME, File::ReadWrite | File::Create | File::Truncate);

    // Create a dataspace with initial shape and max shape
    DataSpace dataspace = DataSpace({4, 5}, {17, DataSpace::UNLIMITED});

    // Use chunking
    DataSetCreateProps props;
    props.setChunk(std::vector<hsize_t>{2, 2});

    // Create the dataset
    DataSet dataset = file.createDataSet(DATASET_NAME, dataspace,
                                         AtomicType<double>(), LinkCreateProps(), props);

    // Write into the initial part of the dataset
    dataset.select({0, 0}, {3, 1}).write(t1);

    // Resize the dataset to a larger size
    dataset.resize({4, 6});

    EXPECT_EQ(4, dataset.getSpace().getDimensions()[0]);
    EXPECT_EQ(6, dataset.getSpace().getDimensions()[1]);

    // Write into the new part of the dataset
    dataset.select({3, 3}, {1, 3}).write(t2);

    SilenceHDF5 silencer;
    // Try resize out of bounds
    EXPECT_THROW(dataset.resize({18, 1}), DataSetException);
    // Try resize invalid dimensions
    EXPECT_THROW(dataset.resize({1, 2, 3}), DataSetException);
  }

  // read it back
  {
    File file(FILE_NAME, File::ReadOnly);

    DataSet dataset_absolute = file.getDataSet("/" + DATASET_NAME);
    const auto dims = dataset_absolute.getSpace().getDimensions();
    double values[4][6];
    dataset_absolute.read(values);
    EXPECT_EQ(4, dims[0]);
    EXPECT_EQ(6, dims[1]);

    EXPECT_EQ(t1[0][0], values[0][0]);
    EXPECT_EQ(t1[1][0], values[1][0]);
    EXPECT_EQ(t1[2][0], values[2][0]);

    EXPECT_EQ(t2[0][0], values[3][3]);
    EXPECT_EQ(t2[0][1], values[3][4]);
    EXPECT_EQ(t2[0][2], values[3][5]);
  }
}

TEST(H5GTBase, RefCountMove) {
  const std::string FILE_NAME("h5_ref_count_test.h5");
  const std::string DATASET_NAME("dset");
  const std::string GROUP_NAME1("/group1");
  const std::string GROUP_NAME2("/group2");

  // Create a new file using the default property lists.
  File file(FILE_NAME, File::ReadWrite | File::Create | File::Truncate);

  std::unique_ptr<DataSet> d1_ptr;
  std::unique_ptr<Group> g_ptr;

  {
    // create group
    Group g1 = file.createGroup(GROUP_NAME1);

    // override object
    g1 = file.createGroup(GROUP_NAME2);

    // Create the data space for the dataset.
    std::vector<size_t> dims = {10, 10};

    DataSpace dataspace(dims);

    DataSet d1 = file.createDataSet(GROUP_NAME1 + DATASET_NAME, dataspace,
                                    AtomicType<double>());

    double values[10][10] = {{0}};
    values[5][0] = 1;
    d1.write(values);

    // force move
    d1_ptr.reset(new DataSet(std::move(d1)));

    // force copy
    g_ptr.reset(new Group(g1));
  }
  // read it back
  {
    DataSet d2(std::move(*d1_ptr));
    d1_ptr.reset();

    double values[10][10];
    d2.read(values);

    for (std::size_t i = 0; i < 10; ++i) {
      for (std::size_t j = 0; j < 10; ++j) {
        double v = values[i][j];

        if (i == 5 && j == 0) {
          EXPECT_EQ(v, 1);
        } else {
          EXPECT_EQ(v, 0);
        }
      }
    }

    // force copy
    Group g2 = *g_ptr;

    // add a subgroup
    g2.createGroup("blabla");
  }
}

TEST(H5GTBase, SimpleListing) {
  const std::string FILE_NAME("h5_list_test.h5");
  const std::string GROUP_NAME_CORE("group_name");
  const std::string GROUP_NESTED_NAME("/group_nested");

  // Create a new file using the default property lists.
  File file(FILE_NAME, File::ReadWrite | File::Create | File::Truncate);

  {
    // absolute group
    for (int i = 0; i < 2; ++i) {
      std::ostringstream ss;
      ss << "/" << GROUP_NAME_CORE << "_" << i;
      file.createGroup(ss.str());
    }

    size_t n_elem = file.getNumberObjects();
    EXPECT_EQ(2, n_elem);

    std::vector<std::string> elems = file.listObjectNames();
    EXPECT_EQ(2, elems.size());
    std::vector<std::string> reference_elems;
    for (int i = 0; i < 2; ++i) {
      std::ostringstream ss;
      ss << GROUP_NAME_CORE << "_" << i;
      reference_elems.push_back(ss.str());
    }

    ASSERT_THAT(elems, ::testing::ElementsAreArray(reference_elems));
  }

  {
    file.createGroup(GROUP_NESTED_NAME);
    Group g_nest = file.getGroup(GROUP_NESTED_NAME);

    for (int i = 0; i < 50; ++i) {
      std::ostringstream ss;
      ss << GROUP_NAME_CORE << "_" << i;
      g_nest.createGroup(ss.str());
    }

    size_t n_elem = g_nest.getNumberObjects();
    EXPECT_EQ(50, n_elem);

    std::vector<std::string> elems = g_nest.listObjectNames();
    EXPECT_EQ(50, elems.size());
    std::vector<std::string> reference_elems;

    for (int i = 0; i < 50; ++i) {
      std::ostringstream ss;
      ss << GROUP_NAME_CORE << "_" << i;
      reference_elems.push_back(ss.str());
    }
    // there is no guarantee on the order of the hdf5 index, let's sort it
    // to put them in order
    std::sort(elems.begin(), elems.end());
    std::sort(reference_elems.begin(), reference_elems.end());

    ASSERT_THAT(elems, ::testing::ElementsAreArray(reference_elems));
  }
}

TEST(H5GTBase, DataTypeEqualSimple) {
  AtomicType<double> d_var;
  AtomicType<size_t> size_var;
  AtomicType<double> d_var_test;
  AtomicType<size_t> size_var_cpy(size_var);
  AtomicType<int> int_var;
  AtomicType<unsigned> uint_var;

  // check different type matching
  ASSERT_TRUE(d_var.isTypeEqual(d_var_test));
  ASSERT_FALSE(d_var.isTypeEqual(size_var));

  // check type copy matching
  ASSERT_TRUE(size_var_cpy.isTypeEqual(size_var));

  // check sign change not matching
  ASSERT_FALSE(int_var.isTypeEqual(uint_var));
}

TEST(H5GTBase, DataTypeEqualTakeBack) {
  const std::string FILE_NAME("h5tutr_dset.h5");
  const std::string DATASET_NAME("dset");

  // Create a new file using the default property lists.
  File file(FILE_NAME, File::ReadWrite | File::Create | File::Truncate);

  // Create the data space for the dataset.
  std::vector<size_t> dims{10, 1};

  DataSpace dataspace(dims);

  // Create a dataset with double precision floating points
  DataSet dataset = file.createDataSet<size_t>(DATASET_NAME + "_double", dataspace);

  AtomicType<size_t> s;
  AtomicType<double> d;

  ASSERT_TRUE(s.isTypeEqual(dataset.getDataType()));
  ASSERT_FALSE(d.isTypeEqual(dataset.getDataType()));
}

TEST(H5GTBase, DataSpaceTest) {
  const std::string FILE_NAME("h5tutr_space.h5");
  const std::string DATASET_NAME("dset");

  // Create a new file using the default property lists.
  File file(FILE_NAME, File::ReadWrite | File::Create | File::Truncate);

  // Create the data space for the dataset.
  DataSpace dataspace{std::vector<size_t>{10, 1}};

  // Create a dataset with size_t type
  DataSet dataset = file.createDataSet<size_t>(DATASET_NAME, dataspace);

  DataSpace space = dataset.getSpace();
  DataSpace space2 = dataset.getSpace();

  // verify space id are different
  EXPECT_NE(space.getId(false), space2.getId(false));

  // verify space id are consistent
  EXPECT_EQ(space.getDimensions().size(), 2);
  EXPECT_EQ(space.getDimensions()[0], 10);
  EXPECT_EQ(space.getDimensions()[1], 1);
}

TEST(H5GTBase, ExternalAndVirtualData) {
  // Create binary data
  std::string fileName = "virtual_data.bin";
  std::vector<int> vin({1,2,3,4,5,6,7}), vout;
  {
    std::ofstream ostrm(fileName, std::ios::binary);
    ostrm.write(reinterpret_cast<const char*>(vin.data()), vin.size()*sizeof(int));
  }

  // read back
  vout.resize(vin.size());
  {
    std::ifstream istrm(fileName, std::ios::binary);
    istrm.read(reinterpret_cast<char*>(vout.data()), vout.size()*sizeof(int));
  }
  for (int i = 0; i < vin.size(); i++){
    EXPECT_EQ(vin[i], vout[i]);
  }

  // Create external dataset from binary file
  const std::string FILE_NAME("virtual_data.h5");
  const std::string SRCDSET_NAME("/srcdset"); // with slash '/' as it is going to be compared
  const std::string VDSET_NAME("/vdset");

  File file(FILE_NAME, File::ReadWrite | File::Create | File::Truncate);
  DataSetCreateProps srcDsetCreateProps;
  off_t offset = 4;
  size_t nbytes = 24;
  srcDsetCreateProps.addExternalFile(fileName, offset);
  auto srcDset = file.createDataSet<int>(
        SRCDSET_NAME, DataSpace(2,3), LinkCreateProps(), srcDsetCreateProps);
  std::vector<int> srcVout_row1, srcVout_row2;
  std::vector<double> vVout; // intensionally created as DOUBLE while src dset is INT
  srcDset.select({0,0},{1,3}).read(srcVout_row1);
  srcDset.select({1,0},{1,3}).read(srcVout_row2);

  // Create virtual dataset from selections
  DataSetCreateProps vDsetCreateProps;
  Selection srcSel1 = srcDset.select({0,0},{1,3});
  DataSpace vSpace(1,6);
  Selection vSel1(vSpace);
  vSel1 = vSel1.select({0,3},{1,3}); // need to update mem space
  vDsetCreateProps.addVirtualDataSet(vSel1.getSpace(), srcDset, srcSel1.getSpace());

  Selection srcSel2 = srcDset.select({1,0},{1,3});
  DataSpace vSpace2(1,6);
  Selection vSel2(vSpace);
  vSel2 = vSel2.select({0,0},{1,3}); // need to update mem space
  vDsetCreateProps.addVirtualDataSet(vSel2.getSpace(), srcDset, srcSel2.getSpace());

  auto vDset = file.createDataSet<double>(
        VDSET_NAME, vSpace, LinkCreateProps(), vDsetCreateProps);

  // we can also read to pointer and it still works as expected
  vDset.read(vVout);
  EXPECT_EQ(srcVout_row1[0], vVout[3]);
  EXPECT_EQ(srcVout_row1[1], vVout[4]);
  EXPECT_EQ(srcVout_row1[2], vVout[5]);
  EXPECT_EQ(srcVout_row2[0], vVout[0]);
  EXPECT_EQ(srcVout_row2[1], vVout[1]);
  EXPECT_EQ(srcVout_row2[2], vVout[2]);

  // modify data (we can write the data from pointer as well)
  vVout[1] = -6.8;
  vDset.write(vVout);
  file.flush();

  vDset.read(vVout.data());
  EXPECT_EQ(vVout[1], -6);

  // read modified data from binary file
  std::vector<int> vout2(6);
  {
    std::ifstream istrm(fileName, std::ios::binary);
    istrm.read(reinterpret_cast<char*>(vout2.data()), vout2.size()*sizeof(int));
  }
  EXPECT_EQ(vout2[5], -6);

  // check properties
  // External
  auto srcDsetCreatePropsOut = srcDset.getCreateProps();
  EXPECT_EQ(srcDsetCreatePropsOut.getExternalCount(), 1);
  off_t srcOffset = 10;
  hsize_t fileSize;
  std::string externalName = srcDsetCreatePropsOut.getExternal(0, srcOffset, fileSize);
  EXPECT_STREQ(fileName.c_str(), externalName.c_str());

  // virtual
  auto vDsetCreatePropsOut = vDset.getCreateProps();
  EXPECT_EQ(vDsetCreatePropsOut.getVirtualCount(), 2);
  std::string srcDsetName = vDsetCreatePropsOut.getVirtualDataSetName(0);
  EXPECT_STREQ(SRCDSET_NAME.c_str(), srcDsetName.c_str());
  std::string vFileName = vDsetCreatePropsOut.getVirtualFileName(0);
  EXPECT_STREQ(FILE_NAME.c_str(), vFileName.c_str());
}

TEST(H5GTBase, DataSpaceVectorTest) {
  // Create 1D shortcut dataspace
  DataSpace space(7);

  EXPECT_EQ(space.getDimensions().size(), 1);
  EXPECT_EQ(space.getDimensions()[0], 7);

  // Initializer list (explicit)
  DataSpace space3({8, 9, 10});
  auto space3_res = space3.getDimensions();
  std::vector<size_t> space3_ans{8, 9, 10};

  ASSERT_THAT(space3_res, ::testing::ElementsAreArray(space3_ans));

  // Verify 2D works (note that without the {}, this matches the iterator
  // constructor)
  DataSpace space2(std::vector<size_t>{3, 4});

  auto space2_res = space2.getDimensions();
  std::vector<size_t> space2_ans{3, 4};

  ASSERT_THAT(space2_res, ::testing::ElementsAreArray(space2_ans));
}

TEST(H5GTBase, DataSpaceVariadicTest) {
  // Create 1D shortcut dataspace
  DataSpace space1{7};

  auto space1_res = space1.getDimensions();
  std::vector<size_t> space1_ans{7};

  ASSERT_THAT(space1_res, ::testing::ElementsAreArray(space1_ans));

  // Initializer list (explicit)
  DataSpace space3{8, 9, 10};

  auto space3_res = space3.getDimensions();
  std::vector<size_t> space3_ans{8, 9, 10};

  ASSERT_THAT(space3_res, ::testing::ElementsAreArray(space3_ans));

  // Verify 2D works using explicit syntax
  DataSpace space2{3, 4};

  auto space2_res = space2.getDimensions();
  std::vector<size_t> space2_ans{3, 4};

  ASSERT_THAT(space2_res, ::testing::ElementsAreArray(space2_ans));

  // Verify 2D works using old syntax (this used to match the iterator!)
  DataSpace space2b(3, 4);

  auto space2b_res = space2b.getDimensions();
  std::vector<size_t> space2b_ans{3, 4};

  ASSERT_THAT(space2b_res, ::testing::ElementsAreArray(space2b_ans));
}

TEST(H5GTBase, ReadWriteShortcut) {
  std::ostringstream filename;
  filename << "h5_rw_vec_shortcut_test.h5";

  const unsigned x_size = 800;
  const std::string DATASET_NAME("dset");
  std::vector<unsigned> vec;
  vec.resize(x_size);
  for (unsigned i = 0; i < x_size; i++)
    vec[i] = i * 2;
  std::string at_contents("Contents of string");
  int my_int = 3;
  std::vector<std::vector<int>> my_nested = {{1, 2}, {3, 4}};

  // Create a new file using the default property lists.
  File file(filename.str(), File::ReadWrite | File::Create | File::Truncate);

  // Create a dataset with int points
  DataSet dataset = file.createDataSet(DATASET_NAME, vec);
  dataset.createAttribute("str", at_contents);

  DataSet ds_int = file.createDataSet("/TmpInt", my_int);
  DataSet ds_nested = file.createDataSet("/TmpNest", my_nested);

  std::vector<int> result;
  dataset.read(result);
  ASSERT_THAT(vec, ::testing::ElementsAreArray(result));

  std::string read_in;
  dataset.getAttribute("str").read(read_in);
  EXPECT_EQ(read_in, at_contents);

  int out_int = 0;
  ds_int.read(out_int);
  EXPECT_EQ(my_int, out_int);

  decltype(my_nested) out_nested;
  ds_nested.read(out_nested);

  for (size_t i = 0; i < 2; ++i) {
    for (size_t j = 0; j < 2; ++j) {
      EXPECT_EQ(my_nested[i][j], out_nested[i][j]);
    }
  }

  // Plain c arrays. 1D
  {
    int int_c_array[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    DataSet ds_int2 = file.createDataSet("/TmpCArrayInt", int_c_array);

    decltype(int_c_array) int_c_array_out;
    ds_int2.read(int_c_array_out);
    for (size_t i = 0; i < 10; ++i) {
      EXPECT_EQ(int_c_array[i], int_c_array_out[i]);
    }
  }

  // Plain c arrays. 2D
  {
    char char_c_2darray[][3] = {"aa", "bb", "cc", "12"};
    DataSet ds_char2 = file.createDataSet("/TmpCArray2dchar", char_c_2darray);

    decltype(char_c_2darray) char_c_2darray_out;
    ds_char2.read(char_c_2darray_out);
    for (size_t i = 0; i < 4; ++i) {
      for (size_t j = 0; j < 3; ++j) {
        EXPECT_EQ(char_c_2darray[i][j], char_c_2darray_out[i][j]);
      }
    }
  }
}

template <typename T>
void readWriteAttributeVectorTest() {
  std::ostringstream filename;
  filename << "h5_rw_attribute_vec_" << typeNameHelper<T>() << "_test.h5";

  std::srand((unsigned)std::time(0));
  const size_t x_size = 25;
  const std::string DATASET_NAME("dset");
  typename std::vector<T> vec;

  // Create a new file using the default property lists.
  File file(filename.str(), File::ReadWrite | File::Create | File::Truncate);

  vec.resize(x_size);
  ContentGenerate<T> generator;
  std::generate(vec.begin(), vec.end(), generator);

  {
    // Create a dummy group to annotate with an attribute
    Group g = file.createGroup("dummy_group");

    // check that no attributes are there
    std::size_t n = g.getNumberAttributes();
    EXPECT_EQ(n, 0);

    std::vector<std::string> all_attribute_names = g.listAttributeNames();
    EXPECT_EQ(all_attribute_names.size(), 0);

    bool has_attribute = g.hasAttribute("my_attribute");
    EXPECT_EQ(has_attribute, false);

    Attribute a1 = g.createAttribute<T>("my_attribute", DataSpace::From(vec));
    a1.write(vec);

    // check now that we effectively have an attribute listable
    std::size_t n2 = g.getNumberAttributes();
    EXPECT_EQ(n2, 1);

    has_attribute = g.hasAttribute("my_attribute");
    ASSERT_TRUE(has_attribute);

    all_attribute_names = g.listAttributeNames();
    EXPECT_EQ(all_attribute_names.size(), 1);
    EXPECT_EQ(all_attribute_names[0], std::string("my_attribute"));

    // Create the same attribute on a newly created dataset
    DataSet s = g.createDataSet("dummy_dataset", DataSpace(1), AtomicType<int>());

    Attribute a2 = s.createAttribute<T>("my_attribute_copy", DataSpace::From(vec));
    a2.write(vec);

    // const data, short-circuit syntax
    const std::vector<int> v{1, 2, 3};
    s.createAttribute("version_test", v);
  }

  {
    typename std::vector<T> result1, result2;

    Attribute a1_read = file.getGroup("dummy_group").getAttribute("my_attribute");
    a1_read.read(result1);

    EXPECT_EQ(vec.size(), x_size);
    EXPECT_EQ(result1.size(), x_size);

    for (size_t i = 0; i < x_size; ++i)
      EXPECT_EQ(result1[i], vec[i]);

    Attribute a2_read = file.getDataSet("/dummy_group/dummy_dataset")
        .getAttribute("my_attribute_copy");
    a2_read.read(result2);

    EXPECT_EQ(vec.size(), x_size);
    EXPECT_EQ(result2.size(), x_size);

    for (size_t i = 0; i < x_size; ++i)
      EXPECT_EQ(result2[i], vec[i]);

    std::vector<int> v;  // with const would print a nice err msg
    file.getDataSet("/dummy_group/dummy_dataset")
        .getAttribute("version_test")
        .read(v);
  }

  // Delete some attributes
  {
    // From group
    auto g = file.getGroup("dummy_group");
    g.deleteAttribute("my_attribute");
    auto n = g.getNumberAttributes();
    EXPECT_EQ(n, 0);

    // From dataset
    auto d = file.getDataSet("/dummy_group/dummy_dataset");
    d.deleteAttribute("my_attribute_copy");
    n = g.getNumberAttributes();
    EXPECT_EQ(n, 0);
  }
}

TEST(H5GTBase, ReadWriteAttributeVector) {
  readWriteAttributeVectorTest<std::string>();
  readWriteAttributeVectorTest<int>();
  readWriteAttributeVectorTest<unsigned>();
  readWriteAttributeVectorTest<size_t>();
  readWriteAttributeVectorTest<ptrdiff_t>();
  readWriteAttributeVectorTest<float>();
  readWriteAttributeVectorTest<double>();
  readWriteAttributeVectorTest<std::complex<float> >();
  readWriteAttributeVectorTest<std::complex<double> >();
}

TEST(H5GTBase, datasetOffset) {
  std::string filename = "datasetOffset.h5";
  std::string dsetname = "dset";
  const size_t size_dataset = 20;

  File file(filename, File::ReadWrite | File::Create | File::Truncate);
  std::vector<int> data(size_dataset);
  DataSet ds = file.createDataSet<int>(dsetname, DataSpace::From(data));
  ds.write(data);
  DataSet ds_read = file.getDataSet(dsetname);
  ASSERT_TRUE(ds_read.getOffset() > 0);
}

template <typename T>
void selectionArraySimpleTest() {
  typedef typename std::vector<T> Vector;

  std::ostringstream filename;
  filename << "h5_rw_select_test_" << typeNameHelper<T>() << "_test.h5";

  const size_t size_x = 10;
  const size_t offset_x = 2, count_x = 5;

  const std::string DATASET_NAME("dset");

  Vector values(size_x);

  ContentGenerate<T> generator;
  std::generate(values.begin(), values.end(), generator);

  // Create a new file using the default property lists.
  File file(filename.str(), File::ReadWrite | File::Create | File::Truncate);

  DataSet dataset = file.createDataSet<T>(DATASET_NAME, DataSpace::From(values));

  dataset.write(values);

  file.flush();

  // select slice
  {
    // read it back
    Vector result;
    std::vector<size_t> offset{offset_x};
    std::vector<size_t> size{count_x};

    Selection slice = dataset.select(offset, size);

    EXPECT_EQ(slice.getSpace().getDimensions()[0], size_x);
    EXPECT_EQ(slice.getMemSpace().getDimensions()[0], count_x);

    slice.read(result);

    EXPECT_EQ(result.size(), 5);

    for (size_t i = 0; i < count_x; ++i) {
      EXPECT_EQ(values[i + offset_x], result[i]);
    }
  }

  // select cherry pick
  {
    // read it back
    Vector result;
    std::vector<size_t> ids{1, 3, 4, 7};

    Selection slice = dataset.select(ElementSet(ids));

    EXPECT_EQ(slice.getSpace().getDimensions()[0], size_x);
    EXPECT_EQ(slice.getMemSpace().getDimensions()[0], ids.size());

    slice.read(result);

    EXPECT_EQ(result.size(), ids.size());

    for (size_t i = 0; i < ids.size(); ++i) {
      const std::size_t id = ids[i];
      EXPECT_EQ(values[id], result[i]);
    }
  }
}

TEST(H5GTBase, selectionArraySimple) {
  selectionArraySimpleTest<std::string>();
  selectionArraySimpleTest<int>();
  selectionArraySimpleTest<unsigned>();
  selectionArraySimpleTest<size_t>();
  selectionArraySimpleTest<ptrdiff_t>();
  selectionArraySimpleTest<float>();
  selectionArraySimpleTest<double>();
  selectionArraySimpleTest<std::complex<float> >();
  selectionArraySimpleTest<std::complex<double> >();
}

TEST(H5GTBase, selectionByElementMultiDim) {
  const std::string FILE_NAME("h5_test_selection_multi_dim.h5");
  // Create a 2-dim dataset
  File file(FILE_NAME, File::ReadWrite | File::Create | File::Truncate);
  std::vector<size_t> dims{3, 3};

  auto set = file.createDataSet("test", DataSpace(dims), AtomicType<int>());
  int values[3][3] = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
  set.write(values);

  {
    int value;
    set.select(ElementSet{{1, 1}}).read(value);
    EXPECT_EQ(value, 5);
  }

  {
    int value[2];
    set.select(ElementSet{0, 0, 2, 2}).read(value);
    EXPECT_EQ(value[0], 1);
    EXPECT_EQ(value[1], 9);
  }

  {
    int value[2];
    set.select(ElementSet{{0, 1}, {1, 2}}).read(value);
    EXPECT_EQ(value[0], 2);
    EXPECT_EQ(value[1], 6);
  }

  {
    SilenceHDF5 silencer;
    EXPECT_THROW(set.select(ElementSet{0, 1, 2}), DataSpaceException);
  }
}

template <typename T>
void columnSelectionTest() {
  std::ostringstream filename;
  filename << "h5_rw_select_column_test_" << typeNameHelper<T>() << "_test.h5";

  const size_t x_size = 10;
  const size_t y_size = 7;

  const std::string DATASET_NAME("dset");

  T values[x_size][y_size];

  ContentGenerate<T> generator;
  generate2D(values, x_size, y_size, generator);

  // Create a new file using the default property lists.
  File file(filename.str(), File::ReadWrite | File::Create | File::Truncate);

  // Create the data space for the dataset.
  std::vector<size_t> dims{x_size, y_size};

  DataSpace dataspace(dims);
  // Create a dataset with arbitrary type
  DataSet dataset = file.createDataSet<T>(DATASET_NAME, dataspace);

  dataset.write(values);

  file.flush();

  std::vector<size_t> columns{1, 3, 5};

  Selection slice = dataset.select(columns);
  T result[x_size][3];
  slice.read(result);

  EXPECT_EQ(slice.getSpace().getDimensions()[0], x_size);
  EXPECT_EQ(slice.getMemSpace().getDimensions()[0], x_size);

  for (size_t i = 0; i < 3; ++i)
    for (size_t j = 0; j < x_size; ++j)
      EXPECT_EQ(result[j][i], values[j][columns[i]]);
}

TEST(H5GTBase, columnSelectionTest) {
  columnSelectionTest<int>();
  columnSelectionTest<unsigned>();
  columnSelectionTest<size_t>();
  columnSelectionTest<ptrdiff_t>();
  columnSelectionTest<float>();
  columnSelectionTest<double>();
  columnSelectionTest<std::complex<float> >();
  columnSelectionTest<std::complex<double> >();
}

template <typename T>
void attribute_scalar_rw() {
  std::ostringstream filename;
  filename << "h5_rw_attribute_scalar_rw" << typeNameHelper<T>() << "_test.h5";

  File h5file(filename.str(), File::ReadWrite | File::Create | File::Truncate);

  ContentGenerate<T> generator;

  const T attribute_value(generator());

  Group g = h5file.createGroup("metadata");

  bool family_exist = g.hasAttribute("family");
  ASSERT_TRUE(!family_exist);

  // write a scalar attribute
  {
    T out(attribute_value);
    Attribute att = g.createAttribute<T>("family", DataSpace::From(out));
    att.write(out);
  }

  h5file.flush();

  // test if attribute exist
  family_exist = g.hasAttribute("family");
  ASSERT_TRUE(family_exist);

  // read back a scalar attribute
  {
    T res;
    Attribute att = g.getAttribute("family");
    att.read(res);
    EXPECT_EQ(res, attribute_value);
  }
}

TEST(H5GTBase, attribute_scalar_rw) {
  // complex doesn't work
  attribute_scalar_rw<std::string>();
  attribute_scalar_rw<int>();
  attribute_scalar_rw<unsigned>();
  attribute_scalar_rw<size_t>();
  attribute_scalar_rw<ptrdiff_t>();
  attribute_scalar_rw<float>();
  attribute_scalar_rw<double>();
}

TEST(H5GTBase, attribute_scalar_rw_string) {
  attribute_scalar_rw<std::string>();
}

// regression test https://github.com/BlueBrain/h5gt/issues/98
TEST(H5GTBase, OutofDimension) {
  std::string filename("h5_rw_reg_zero_dim_test.h5");

  const std::string DATASET_NAME("dset");

  {
    // Create a new file using the default property lists.
    File file(filename, File::ReadWrite | File::Create | File::Truncate);

    DataSpace d_null(DataSpace::DataspaceType::datascape_null);

    DataSet d1 = file.createDataSet<double>(DATASET_NAME, d_null);

    file.flush();

    DataSpace recovered_d1 = d1.getSpace();

    auto ndim = recovered_d1.getNumberDimensions();
    EXPECT_EQ(ndim, 0);

    auto dims = recovered_d1.getDimensions();
    EXPECT_EQ(dims.size(), 0);
  }
}

template <typename T>
void readWriteShuffleDeflateTest() {
  std::ostringstream filename;
  filename << "h5_rw_deflate_" << typeNameHelper<T>() << "_test.h5";
  const std::string DATASET_NAME("dset");
  const size_t x_size = 128;
  const size_t y_size = 32;
  const size_t x_chunk = 16;
  const size_t y_chunk = 16;

  const int deflate_level = 9;

  T array[x_size][y_size];

  // write a compressed file
  {
    File file(filename.str(), File::ReadWrite | File::Create | File::Truncate);

    // Create the data space for the dataset.
    std::vector<size_t> dims{x_size, y_size};

    DataSpace dataspace(dims);

    // Use chunking
    DataSetCreateProps props;
    props.setChunk(std::vector<hsize_t>{x_chunk, y_chunk});

    // Enable shuffle
    props.setShuffle();

    // Enable deflate
    props.setDeflate(deflate_level);

    // Create a dataset with arbitrary type
    DataSet dataset = file.createDataSet<T>(
          DATASET_NAME, dataspace, LinkCreateProps(), props);

    ContentGenerate<T> generator;
    generate2D(array, x_size, y_size, generator);

    dataset.write(array);

    file.flush();
  }

  // read it back
  {
    File file_read(filename.str(), File::ReadOnly);
    DataSet dataset_read = file_read.getDataSet("/" + DATASET_NAME);

    T result[x_size][y_size];

    dataset_read.read(result);

    for (size_t i = 0; i < x_size; ++i) {
      for (size_t j = 0; i < y_size; ++i) {
        EXPECT_EQ(result[i][j], array[i][j]);
      }
    }
  }
}

TEST(H5GTBase, readWriteShuffleDeflateTest) {
  readWriteShuffleDeflateTest<int>();
  readWriteShuffleDeflateTest<unsigned>();
  readWriteShuffleDeflateTest<size_t>();
  readWriteShuffleDeflateTest<ptrdiff_t>();
  readWriteShuffleDeflateTest<float>();
  readWriteShuffleDeflateTest<double>();
  readWriteShuffleDeflateTest<std::complex<float> >();
  readWriteShuffleDeflateTest<std::complex<double> >();
}

// Broadcasting is supported
TEST(H5GTBase, ReadInBroadcastDims) {

  const std::string FILE_NAME("h5_missmatch1_dset.h5");
  const std::string DATASET_NAME("dset");

  // Create a new file using the default property lists.
  File file(FILE_NAME, File::ReadWrite | File::Create | File::Truncate);

  // Create the data space for the dataset.
  std::vector<size_t> dims_a{1, 3};
  std::vector<size_t> dims_b{3, 1};

  // 1D input / output vectors
  std::vector<double> some_data{5.0, 6.0, 7.0};
  std::vector<double> data_a;
  std::vector<double> data_b;

  DataSpace dataspace_a(dims_a);
  DataSpace dataspace_b(dims_b);

  // Create a dataset with double precision floating points
  DataSet dataset_a = file.createDataSet(DATASET_NAME + "_a", dataspace_a,
                                         AtomicType<double>());
  DataSet dataset_b = file.createDataSet(DATASET_NAME + "_b", dataspace_b,
                                         AtomicType<double>());

  dataset_a.write(some_data);
  dataset_b.write(some_data);

  DataSet out_a = file.getDataSet(DATASET_NAME + "_a");
  DataSet out_b = file.getDataSet(DATASET_NAME + "_b");

  out_a.read(data_a);
  out_b.read(data_b);

  ASSERT_THAT(data_a, ::testing::ElementsAreArray(some_data));

  ASSERT_THAT(data_b, ::testing::ElementsAreArray(some_data));
}

TEST(H5GTBase, RecursiveGroups) {
  const std::string FILE_NAME("h5_ds_exist.h5");
  const std::string GROUP_1("group1"), GROUP_2("group2");
  const std::string DS_PATH = GROUP_1 + "/" + GROUP_2;
  const std::string DS_NAME = "ds";

  // Create a new file using the default property lists.
  File file(FILE_NAME, File::ReadWrite | File::Create | File::Truncate);

  EXPECT_EQ(file.getFileName(), FILE_NAME);

  Group g2 = file.createGroup(DS_PATH);

  std::vector<double> some_data{5.0, 6.0, 7.0};
  g2.createDataSet(DS_NAME, some_data);

  ASSERT_TRUE(file.exist(GROUP_1));

  Group g1 = file.getGroup(GROUP_1);
  ASSERT_TRUE(g1.exist(GROUP_2));

  // checks with full path
  ASSERT_TRUE(file.exist(DS_PATH));
  ASSERT_TRUE(file.exist(DS_PATH + "/" + DS_NAME));

  // Check with wrong middle path (before would raise Exception)
  EXPECT_EQ(file.exist(std::string("blabla/group2")), false);

  // Using root slash
  ASSERT_TRUE(file.exist(std::string("/") + DS_PATH));

  // Check unlink with existing group
  ASSERT_TRUE(g1.exist(GROUP_2));
  g1.unlink(GROUP_2);
  ASSERT_TRUE(!g1.exist(GROUP_2));

  // Check unlink with non-existing group
  {
    SilenceHDF5 silencer;
    EXPECT_THROW(g1.unlink("x"), h5gt::GroupException);
  }
}

TEST(H5GTBase, Inspect) {
  const std::string FILE_NAME("group_info.h5");
  const std::string GROUP_1("group1");
  const std::string DS_NAME = "ds";

  // Create a new file using the default property lists.
  File file(FILE_NAME, File::ReadWrite | File::Create | File::Truncate);
  Group g = file.createGroup(GROUP_1);

  std::vector<double> some_data{5.0, 6.0, 7.0};
  g.createDataSet(DS_NAME, some_data);

  ASSERT_TRUE(file.getLinkType(GROUP_1) == LinkType::Hard);

  {
    SilenceHDF5 silencer;
    EXPECT_THROW(file.getLinkType("x"), h5gt::GroupException);
  }

  ASSERT_TRUE(file.getObjectType(GROUP_1) == ObjectType::Group);
  ASSERT_TRUE(file.getObjectType(GROUP_1 + "/" + DS_NAME) == ObjectType::Dataset);
  ASSERT_TRUE(g.getObjectType(DS_NAME) == ObjectType::Dataset);

  {
    SilenceHDF5 silencer;
    EXPECT_THROW(file.getObjectType(DS_NAME), h5gt::GroupException);
  }

  // Data type
  auto ds = g.getDataSet(DS_NAME);
  auto dt = ds.getDataType();
  ASSERT_TRUE(dt.getClass() == DataTypeClass::Float);
  ASSERT_TRUE(dt.getSize() == 8);
  ASSERT_TRUE(dt.string() == "Float64");

  // meta
  ASSERT_TRUE(ds.getObjectType() == ObjectType::Dataset);  // internal
  ASSERT_TRUE(ds.getObjectInfo().getHardLinkRefCount() == 1);
}

TEST(H5GTBase, GetPath) {
  File file("getpath.h5", File::ReadWrite | File::Create | File::Truncate);

  int number = 100;
  Group group = file.createGroup("group");
  DataSet dataset = group.createDataSet("data", DataSpace(1), AtomicType<int>());
  dataset.write(number);
  std::string string_list("Very important DataSet!");
  Attribute attribute = dataset.createAttribute<std::string>("attribute", DataSpace::From(string_list));
  attribute.write(string_list);

  EXPECT_EQ("/", file.getPath());
  EXPECT_EQ("/group", group.getPath());
  EXPECT_EQ("/group/data", dataset.getPath());
  EXPECT_EQ("attribute", attribute.getName());
}

TEST(H5GTBase, Filename) {
  File file("getFileName.h5", File::ReadWrite | File::Create | File::Truncate);

  EXPECT_EQ("getFileName.h5", file.getFileName());
}

TEST(H5GTBase, Refresh) {
  File file("getFileName.h5", File::ReadWrite | File::Create | File::Truncate);
  DataSet dset = file.createDataSet<double>("data", DataSpace(1));
  ASSERT_TRUE(dset.refresh());
}

TEST(H5GTBase, EqualityOperator) {
  File file1("equality_operator_1.h5", File::ReadWrite | File::Create | File::Truncate);
  File file2("equality_operator_2.h5", File::ReadWrite | File::Create | File::Truncate);
  File file3("equality_operator_1.h5", File::ReadWrite | File::ReadOnly);
  File file4("equality_operator_2.h5", File::ReadWrite | File::ReadOnly);

  ASSERT_TRUE(file1 == file1);
  ASSERT_TRUE(file1 == file3);
  ASSERT_TRUE(file1 != file2);
  ASSERT_TRUE(file2 == file4);

  Group group11 = file1.createGroup("group_1");
  Group group12 = file1.createGroup("group_2");
  Group group13 = file1.getGroup("group_1");
  Group group21 = file2.createGroup("group_1");
  Group group22 = file2.createGroup("group_2");
  Group group23 = file2.getGroup("group_1");

  ASSERT_TRUE(group11 == group11);
  ASSERT_TRUE(group11 == group13);
  ASSERT_TRUE(group21 == group23);
  ASSERT_TRUE(group11 != group12);
  ASSERT_TRUE(group11 != group21);

  DataSet dset11 = file1.createDataSet<int>("data_1", DataSpace(1));
  DataSet dset12 = file1.createDataSet<int>("data_2", DataSpace(1));
  DataSet dset21 = file2.createDataSet<int>("data_1", DataSpace(1));
  DataSet dset22 = file2.createDataSet<int>("data_2", DataSpace(1));

  ASSERT_TRUE(dset11 == dset11);
  ASSERT_TRUE(dset11 != dset12);
  ASSERT_TRUE(dset11 != dset21);
}

TEST(H5GTBase, CheckIdFunctionality) {
  // Retrieve file ID from objects and compare it with original
  File file("getFileName.h5", File::ReadWrite | File::Create | File::Truncate);
  Group group = file.createGroup("group");
  DataSpace dataSpace = DataSpace(1);
  DataSet dataset = group.createDataSet<int>("data", dataSpace);
  Attribute attr = dataset.createAttribute<int>("attr", dataSpace);
  std::vector<CompoundType::member_def> t_members(
        {{"real", AtomicType<int>{}},
         {"imag", AtomicType<int>{}}});

  CompoundType t(t_members);

  ASSERT_EQ(file.getId(false), file.getFileId(false)); // getFileId() uses HDF5 native functionality
  ASSERT_EQ(file.getId(false), group.getFileId(false));
  ASSERT_EQ(file.getId(false), dataset.getFileId(false));
  ASSERT_EQ(file.getId(false), attr.getFileId(false));

  t.commit(file, "new_type1");

  ASSERT_EQ(file.getId(false), t.getFileId(false));

  // Create objects from ID
  File file2 = File::FromId(file.getId(false), true);
  Group group2 = Group::FromId(group.getId(false), true);
  DataSpace dataSpace2 = DataSpace::FromId(dataSpace.getId(false), true);
  DataSet dataset2 = DataSet::FromId(dataset.getId(false), true);
  Attribute attr2 = Attribute::FromId(attr.getId(false), true);
  CompoundType t2 = CompoundType::FromId(t.getId(false), true);

  ASSERT_TRUE(file2.isValid());
  ASSERT_TRUE(group2.isValid());
  ASSERT_TRUE(dataSpace2.isValid());
  ASSERT_TRUE(dataset2.isValid());
  ASSERT_TRUE(attr2.isValid());
  ASSERT_TRUE(t2.isValid());

  // Check reference count ID increase
  ASSERT_EQ(file2.getIdRefCount(), 2);
  File file3 = File::FromId(file.getId(false), true);
  ASSERT_EQ(file3.getIdRefCount(), 3);
}

TEST(H5GTBase, CopyObject) {
  File file1("copyFrom.h5", File::ReadWrite | File::Create | File::Truncate);
  File file2("copyTo.h5", File::ReadWrite | File::Create | File::Truncate);

  Group group1 = file1.createGroup("group1");
  double val_in = 5.5;
  DataSet dset1 = group1.createDataSet<double>("data", DataSpace::From(val_in));
  dset1.write(val_in);

  // copy group with inner dataset in it
  Group group2 = file2.copy(group1, group1.getPath());
  DataSet dset2 = group2.getDataSet(dset1.getPath());
  double val_out;
  dset2.read(val_out);

  ASSERT_TRUE(group1.getPath() == group2.getPath());
  ASSERT_TRUE(dset1.getPath() == dset2.getPath());
  ASSERT_EQ(val_in, val_out);
}

TEST(H5GTBase, GetDatasetProps) {
  std::vector<hsize_t> c_dims({2,3});
  File file("getProps.h5", File::ReadWrite | File::Create | File::Truncate);
  DataSetCreateProps dsetCreateProps;
  dsetCreateProps.setChunk(c_dims);
  DataSet dset = file.createDataSet<double>(
        "data", DataSpace({10,10}), LinkCreateProps(), dsetCreateProps);
  std::vector<hsize_t> v = dset.getCreateProps().getChunk(4);
  ASSERT_TRUE(dset.getCreateProps().isChunked());
  ASSERT_THAT(v, ::testing::ElementsAreArray(c_dims));

  dset = file.createDataSet<double>(
        "data1", DataSpace({10,10}));
  ASSERT_TRUE(dset.getCreateProps().isContiguous());
}

TEST(H5GTBase, SoftLink) {
  int val_in = 371;
  int val_out = 0;

  File file("link_soft.h5", File::ReadWrite | File::Create | File::Truncate);
  Group group = file.createGroup("path/to/group");

  // Dataset
  DataSet dset = group.createDataSet<int>("data", DataSpace(1));
  dset.write(val_in);

  DataSet dset_out = file.createLink(dset, "myDsetLink", LinkType::Soft);
  ASSERT_EQ(dset_out.getLinkInfo().getLinkType(), LinkType::Soft);
  ASSERT_EQ(dset_out.getTargetPath(), "/path/to/group/data");
  ASSERT_EQ(dset_out.getPath(), "/myDsetLink");
  ASSERT_EQ(dset_out.getFileName(), "link_soft.h5");

  dset_out.read(val_out);
  ASSERT_EQ(val_out, val_in);

  // Group
  Group group_out = file.createLink(group, "myGroupLink", LinkType::Soft);
  ASSERT_EQ(group_out.getLinkInfo().getLinkType(), LinkType::Soft);

  DataSet group_out_dset = group_out.getDataSet("data");
  ASSERT_TRUE(group_out_dset.isValid());

  val_out = 0;
  group_out_dset.read(val_out);
  ASSERT_EQ(val_out, val_in);

  // Change the value of an existing original dset
  val_in = 717;
  dset.write(val_in);

  val_out = 0;
  dset_out.read(val_out);
  ASSERT_EQ(val_out, val_in);

  val_out = 0;
  group_out_dset.read(val_out);
  ASSERT_EQ(val_out, val_in);
}

TEST(H5GTBase, HardLink) {
  int val_in = 371;
  int val_out = 0;

  File file("link_hard.h5", File::ReadWrite | File::Create | File::Truncate);
  Group group = file.createGroup("path/to/group");

  // Dataset
  DataSet dset = group.createDataSet<int>("data", DataSpace(1));
  dset.write(val_in);

  DataSet dset_out = file.createLink(dset, "myDsetLink", LinkType::Hard);
  ASSERT_EQ(dset_out.getLinkInfo().getLinkType(), LinkType::Hard);
  ASSERT_EQ(dset_out.getTargetPath(), "/myDsetLink");
  ASSERT_EQ(dset_out.getPath(), "/myDsetLink");
  ASSERT_EQ(dset_out.getFileName(), "link_hard.h5");
  ASSERT_EQ(dset.getObjectInfo().getHardLinkRefCount(), 2);
  ASSERT_EQ(dset_out.getObjectInfo().getHardLinkRefCount(), 2);

  dset_out.read(val_out);
  ASSERT_EQ(val_out, val_in);

  // Group
  Group group_out = file.createLink(group, "myGroupLink", LinkType::Hard);
  ASSERT_EQ(group_out.getLinkInfo().getLinkType(), LinkType::Hard);

  DataSet group_out_dset = group_out.getDataSet("data");
  ASSERT_TRUE(group_out_dset.isValid());

  val_out = 0;
  group_out_dset.read(val_out);
  ASSERT_EQ(val_out, val_in);

  // Change the value of an existing original dset
  val_in = 717;
  dset.write(val_in);

  val_out = 0;
  dset_out.read(val_out);
  ASSERT_EQ(val_out, val_in);

  val_out = 0;
  group_out_dset.read(val_out);
  ASSERT_EQ(val_out, val_in);
}

TEST(H5GTBase, ExternalLink) {
  int val_in = 371;
  int val_out = 0;

  File file1("link_external_from.h5", File::ReadWrite | File::Create | File::Truncate);
  Group group = file1.createGroup("path/to");
  DataSet dset1 = group.createDataSet<int>("data", DataSpace(1));

  File file2("link_external_to.h5", File::ReadWrite | File::Create | File::Truncate);
  Group group2 = file2.createLink(group, "myExternalLink", LinkType::External);

  // When you get object via External link then you get original object (Hard linked object)
  // and there is nothing left from that External link
  ASSERT_EQ(group2.getLinkInfo().getLinkType(), LinkType::Hard);
  ASSERT_EQ(group2.getTargetPath(), "/path/to");
  ASSERT_EQ(group2.getPath(), "/path/to");
  ASSERT_FALSE(group2.getFileName().empty());

  DataSet dset2 = group2.getDataSet("data");

  dset1.write(val_in);
  dset2.read(val_out);
  ASSERT_EQ(val_out, val_in);
}

TEST(H5GTBase, getParent) {
  File file("getParent_test.h5", File::ReadWrite | File::Create | File::Truncate);
  
  Group group = file.createGroup("/path/to/group");
  EXPECT_EQ(group.getParent().getPath(), "/path/to");
  EXPECT_EQ(group.getParent().getParent().getPath(), "/path");
  EXPECT_EQ(group.getParent().getParent().getParent().getPath(), "/");
  EXPECT_THROW(group.getParent().getParent().getParent().getParent().getPath(), h5gt::GroupException);

  DataSet dataset = file.createDataSet("/path/to/data", DataSpace(1), AtomicType<int>());
  EXPECT_EQ(dataset.getParent().getPath(), "/path/to");
  EXPECT_EQ(dataset.getParent().getParent().getPath(), "/path");
  EXPECT_EQ(dataset.getParent().getParent().getParent().getPath(), "/");
  EXPECT_THROW(dataset.getParent().getParent().getParent().getParent().getPath(), h5gt::GroupException);
}

TEST(H5GTBase, Rename) {

  File file("move_test.h5", File::ReadWrite | File::Create | File::Truncate);

  int number = 100;

  {
    Group group = file.createGroup("group");
    DataSet dataset = group.createDataSet("data", DataSpace(1), AtomicType<int>());
    dataset.write(number);
    std::string path = dataset.getPath();
    EXPECT_EQ("/group/data", path);
  }

  file.rename("/group/data", "/new/group/new/data");

  {
    DataSet dataset = file.getDataSet("/new/group/new/data");
    std::string path = dataset.getPath();
    EXPECT_EQ("/new/group/new/data", path);
    int read;
    dataset.read(read);
    EXPECT_EQ(number, read);
  }
}

TEST(H5GTBase, RenameRelative) {

  File file("move_test.h5", File::ReadWrite | File::Create | File::Truncate);
  Group group = file.createGroup("group");

  int number = 100;

  {
    DataSet dataset = group.createDataSet("data", DataSpace(1), AtomicType<int>());
    dataset.write(number);
    EXPECT_EQ("/group/data", dataset.getPath());
  }

  group.rename("data", "new_data");

  {
    DataSet dataset = group.getDataSet("new_data");
    EXPECT_EQ("/group/new_data", dataset.getPath());
    int read;
    dataset.read(read);
    EXPECT_EQ(number, read);
  }
}

#define H5GEO_CHAR_ARRAY_SIZE 50

typedef struct {
  int m1;
  int m2;
  int m3;
} CSL1;

typedef struct {
  CSL1 csl1;
} CSL2;

typedef struct {
    double x;
    std::string name;
    std::string family;
    std::string middlename;
} CT1;

typedef struct {
    int y;
    std::string film;
    CT1 ct1;
} CT2;

typedef struct CSL4 {
  double x;
  double y;
  double z;

  void setName(const std::string& s){
    size_t nChar2copy = std::min(s.size(), size_t(H5GEO_CHAR_ARRAY_SIZE - 1));
    if (nChar2copy < 1){
      this->name[0] = '\0';
    } else {
       s.copy(this->name, nChar2copy);
      this->name[nChar2copy] = '\0';
    }
  }

  std::string getName(){
    return std::string(this->name);
  }

  // needs to be public to calculate offset
  char name[H5GEO_CHAR_ARRAY_SIZE];
} CSL4;


CompoundType create_compound_csl1() {
  auto t2 = AtomicType<int>();
  CompoundType t1({{"m1", AtomicType<int>{}}, {"m2", AtomicType<int>{}}, {"m3", t2}});

  return t1;
}

CompoundType create_compound_csl2() {
  CompoundType t1 = create_compound_csl1();

  CompoundType t2({{"csl1", t1}});

  return t2;
}

/// NOT SUPPORTED (NOT TRIVIAL TYPE, 'offsetof' is undefined behaviour)
CompoundType create_compound_CT1() {
  CompoundType t(
        {
          {"x", AtomicType<double>{}, HOFFSET(CT1, x)}, // HOFFSET(CT1, x) expands to offsetof(CT1, x)
          {"name", AtomicType<std::string>{}, HOFFSET(CT1, name)},
          {"family", AtomicType<std::string>{}, HOFFSET(CT1, family)},
          {"middlename", AtomicType<std::string>{}, HOFFSET(CT1, middlename)}
        }, sizeof(CT1));

  return t;
}

/// NOT SUPPORTED (NOT TRIVIAL TYPE, 'offsetof' is undefined behaviour)
inline CompoundType create_compound_CT2() {
  CompoundType t(
        {
          {"y", AtomicType<int>{}, HOFFSET(CT2, y)},
          {"film", AtomicType<std::string>{}, HOFFSET(CT2, film)},
          {"ct1", create_compound_CT1(), HOFFSET(CT2, ct1)}
        }, sizeof(CT2));

  return t;
}

CompoundType create_compound_csl4() {
  CompoundType t4(
        {
          {"x", AtomicType<double>{}, HOFFSET(CSL4, x)},
          {"y", AtomicType<double>{}, HOFFSET(CSL4, y)},
          {"z", AtomicType<double>{}, HOFFSET(CSL4, z)},
          {"name", AtomicType<FixedLenStringArray<H5GEO_CHAR_ARRAY_SIZE>>{}, HOFFSET(CSL4, name)}
        }, sizeof(CSL4));

  return t4;
}

H5GT_REGISTER_TYPE(CSL1, create_compound_csl1)
H5GT_REGISTER_TYPE(CSL2, create_compound_csl2)
H5GT_REGISTER_TYPE(CT1, create_compound_CT1)
H5GT_REGISTER_TYPE(CT2, create_compound_CT2)
H5GT_REGISTER_TYPE(CSL4, create_compound_csl4)

TEST(H5GTBase, Compounds) {
  const std::string FILE_NAME("compounds_test.h5");
  const std::string DATASET_NAME1("/a");
  const std::string DATASET_NAME2("/b");
  const std::string DATASET_NAME3("/c");
  const std::string DATASET_NAME4("/d");

  File file(FILE_NAME, File::ReadWrite | File::Create | File::Truncate);

  CompoundType t1 = create_compound_csl1();
  t1.commit(file, "my_type");

  CompoundType t2 = create_compound_csl2();
  t2.commit(file, "my_type2");

  CompoundType t_ct1 = create_compound_CT1();
  t_ct1.commit(file, "CT1");

  CompoundType t_ct2 = create_compound_CT2();
  t_ct2.commit(file, "CT2");

  CompoundType t4 = create_compound_csl4();
  t4.commit(file, "H5Points4");

  {  // Not nested
    auto dataset = file.createDataSet(DATASET_NAME1, DataSpace(2), t1);

    std::vector<CSL1> csl = {{1, 1, 1}, {2, 3, 4}};
    dataset.write(csl);

    file.flush();

    std::vector<CSL1> result;
    dataset.select({0}, {2}).read(result);

    EXPECT_EQ(result.size(), 2);
    EXPECT_EQ(result[0].m1, 1);
    EXPECT_EQ(result[0].m2, 1);
    EXPECT_EQ(result[0].m3, 1);
    EXPECT_EQ(result[1].m1, 2);
    EXPECT_EQ(result[1].m2, 3);
    EXPECT_EQ(result[1].m3, 4);

    // ! VARIANT LENGTH COMPOUND TYPE UNSUPPORTED AS 'offsetof' WORKS ONLY WITH TRIVIAL (POD) TYPES
    // type containing 'std::string' as a member var is non-POD
    // you can check if the type is trivial using 'std::cout << std::is_trivial<CSL4>::value << std::endl;'
//    std::vector<CT1> ct1_vec_in{
//        {1.1, "John", "Travolta", "Olegovich"},
//        {2.2, "Clint", "Eastwood", "Vasilievich"}
//        };

//    auto dataset_ct1 = file.createDataSet("dset_vec_CT1", DataSpace::From(ct1_vec_in), t_ct1);
//    dataset_ct1.write(ct1_vec_in);

//    file.flush();

//    std::vector<CT1> ct1_vec_out;
//    dataset_ct1.read(ct1_vec_out);

//    std::vector<CT2> ct2_vec_in{
//        {3, "Gone with the wind", {1.1, "John", "Travolta", "Olegovich"}},
//        {4, "Fast and dead", {2.2, "Clint", "Eastwood", "Vasilievich"}}
//        };

//    auto dataset_ct2 = file.createDataSet("dset_vec_CT2", DataSpace::From(ct2_vec_in), t_ct2);
//    dataset_ct2.write(ct2_vec_in);

//    file.flush();

//    std::vector<CT2> ct2_vec_out;
//    dataset_ct2.read(ct2_vec_out);

//    EXPECT_EQ(ct2_vec_out.size(), 2);
//    EXPECT_EQ(ct2_vec_out[0].y, ct2_vec_in[0].y);
//    EXPECT_EQ(ct2_vec_out[0].film, ct2_vec_in[0].film);
//    EXPECT_EQ(ct2_vec_out[0].ct1.x, ct2_vec_in[0].ct1.x);
//    EXPECT_EQ(ct2_vec_out[0].ct1.name, ct2_vec_in[0].ct1.name);
//    EXPECT_EQ(ct2_vec_out[0].ct1.family, ct2_vec_in[0].ct1.family);
//    EXPECT_EQ(ct2_vec_out[1].y, ct2_vec_in[1].y);
//    EXPECT_EQ(ct2_vec_out[1].film, ct2_vec_in[1].film);
//    EXPECT_EQ(ct2_vec_out[1].ct1.x, ct2_vec_in[1].ct1.x);
//    EXPECT_EQ(ct2_vec_out[1].ct1.name, ct2_vec_in[1].ct1.name);
//    EXPECT_EQ(ct2_vec_out[1].ct1.family, ct2_vec_in[1].ct1.family);

    CSL4 cls41, cls42;
    cls41.x = 1;
    cls41.z = 1;
    cls41.y = 1;
    strncpy(cls41.name, "one", sizeof(cls41.name));
    cls41.name[sizeof(cls41.name) - 1] = 0;
    cls42.x = 2;
    cls42.z = 2;
    cls42.y = 2;
    strncpy(cls42.name, "two", sizeof(cls42.name));
    cls42.name[sizeof(cls42.name) - 1] = 0;

    std::vector<CSL4> csl4 = {cls41, cls42};

    auto dataset4 = file.createDataSet(DATASET_NAME4, DataSpace::From(csl4), t4);
    dataset4.write(csl4);

    auto dtype4 = dataset4.getDataType();
    EXPECT_TRUE(dtype4.isTypeEqual(create_compound_csl4()));

    file.flush();

    std::vector<CSL4> result4(2);
    dataset4.read(result4.data(), dtype4);

    EXPECT_EQ(result4.size(), 2);
    EXPECT_EQ(result4[0].x, csl4[0].x);
    EXPECT_EQ(result4[0].y, csl4[0].y);
    EXPECT_EQ(result4[0].z, csl4[0].z);
    EXPECT_EQ(result4[0].getName(), csl4[0].getName());
    EXPECT_EQ(result4[1].x, csl4[1].x);
    EXPECT_EQ(result4[1].y, csl4[1].y);
    EXPECT_EQ(result4[1].z, csl4[1].z);
    EXPECT_EQ(result4[1].getName(), csl4[1].getName());
  }

  {  // Nested
    auto dataset = file.createDataSet(DATASET_NAME2, DataSpace(2), t2);

    std::vector<CSL2> csl = {{{1, 1, 1}, {2, 3, 4}}};
    dataset.write(csl);

    file.flush();
    std::vector<CSL2> result = {{{1, 1, 1}, {2, 3, 4}}};
    dataset.select({0}, {2}).read(result);

    EXPECT_EQ(result.size(), 2);
    EXPECT_EQ(result[0].csl1.m1, 1);
    EXPECT_EQ(result[0].csl1.m2, 1);
    EXPECT_EQ(result[0].csl1.m3, 1);
    EXPECT_EQ(result[1].csl1.m1, 2);
    EXPECT_EQ(result[1].csl1.m2, 3);
    EXPECT_EQ(result[1].csl1.m3, 4);
  }
}

enum Position {
  FIRST = 1,
  SECOND = 2,
  THIRD = 3,
  LAST = -1,
};

enum class Direction : signed char {
  FORWARD = 1,
  BACKWARD = -1,
  LEFT = -2,
  RIGHT = 2,
};

// This is only for boost test
std::ostream& operator<<(std::ostream& ost, const Direction& dir) {
  ost << static_cast<int>(dir);
  return ost;
}

#include <type_traits>
EnumType<std::underlying_type<Position>::type> create_enum_position() {
  return {{"FIRST", Position::FIRST},
    {"SECOND", Position::SECOND},
    {"THIRD", Position::THIRD},
    {"LAST", Position::LAST}};
}
H5GT_REGISTER_TYPE(Position, create_enum_position)

typedef std::underlying_type<Direction>::type DirectionUType;
EnumType<DirectionUType> create_enum_direction() {
  return {{"FORWARD", static_cast<DirectionUType>(Direction::FORWARD)},
    {"BACKWARD", static_cast<DirectionUType>(Direction::BACKWARD)},
    {"LEFT", static_cast<DirectionUType>(Direction::LEFT)},
    {"RIGHT", static_cast<DirectionUType>(Direction::RIGHT)}};
}
H5GT_REGISTER_TYPE(Direction, create_enum_direction)

TEST(H5GTBase, Enum) {
  const std::string FILE_NAME("enum_test.h5");
  const std::string DATASET_NAME1("/a");
  const std::string DATASET_NAME2("/b");

  File file(FILE_NAME, File::ReadWrite | File::Create | File::Truncate);

  {  // Unscoped enum
    auto e1 = create_enum_position();
    e1.commit(file, "Position");

    auto dataset = file.createDataSet(DATASET_NAME1, DataSpace(1), e1);
    dataset.write(Position::FIRST);

    file.flush();

    Position result;
    dataset.select(ElementSet({0})).read(result);

    EXPECT_EQ(result, Position::FIRST);
  }

  {  // Scoped enum
    auto e1 = create_enum_direction();
    e1.commit(file, "Direction");

    auto dataset = file.createDataSet(DATASET_NAME2, DataSpace(5), e1);
    std::vector<Direction> robot_moves({Direction::BACKWARD, Direction::FORWARD,
                                        Direction::FORWARD, Direction::LEFT,
                                        Direction::LEFT});
    dataset.write(robot_moves);

    file.flush();

    std::vector<Direction> result;
    dataset.read(result);

    EXPECT_EQ(result[0], Direction::BACKWARD);
    EXPECT_EQ(result[1], Direction::FORWARD);
    EXPECT_EQ(result[2], Direction::FORWARD);
    EXPECT_EQ(result[3], Direction::LEFT);
    EXPECT_EQ(result[4], Direction::LEFT);
  }
}

TEST(H5GTBase, FixedString) {
  const std::string FILE_NAME("array_atomic_types.h5");
  const std::string GROUP_1("group1");

  // Create a new file using the default property lists.
  File file(FILE_NAME, File::ReadWrite | File::Create | File::Truncate);
  char raw_strings[][10] = {"abcd", "1234"};

  /// This will not compile - only char arrays - hits static_assert with a nice error
  // file.createDataSet<int[10]>(DS_NAME, DataSpace(2)));

  {  // But char should be fine
    auto ds = file.createDataSet<char[10]>("ds1", DataSpace(2));
    ASSERT_TRUE(ds.getDataType().getClass() == DataTypeClass::String);
    ds.write(raw_strings);
  }

  {  // char[] is, by default, int8
    auto ds2 = file.createDataSet("ds2", raw_strings);
    ASSERT_TRUE(ds2.getDataType().getClass() == DataTypeClass::Integer);
  }

  {  // String Truncate happens low-level if well setup
    auto ds3 = file.createDataSet<char[6]>(
          "ds3", DataSpace::FromCharArrayStrings(raw_strings));
    ds3.write(raw_strings);
  }

  {  // Write as raw elements from pointer (with const)
    const char(*strings_fixed)[10] = raw_strings;
    // With a pointer we dont know how many strings -> manual DataSpace
    file.createDataSet<char[10]>("ds4", DataSpace(2)).write(strings_fixed);
  }

  {  // Cant convert flex-length to fixed-length
    const char* buffer[] = {"abcd", "1234"};
    SilenceHDF5 silencer;
    EXPECT_THROW(file.createDataSet<char[10]>("ds5", DataSpace(2)).write(buffer),
        h5gt::DataSetException);
  }

  {  // scalar char strings
    const char buffer[] = "abcd";
    file.createDataSet<char[10]>("ds6", DataSpace(1)).write(buffer);
  }

  {  // Dedicated FixedLenStringArray
    FixedLenStringArray<10> arr{"0000000", "1111111"};
    // For completeness, test also the other constructor
    FixedLenStringArray<10> arrx(std::vector<std::string>{"0000", "1111"});

    // More API: test inserting something
    arr.push_back("2222");
    auto ds = file.createDataSet("ds7", arr);  // Short syntax ok

    // Recover truncating
    FixedLenStringArray<4> array_back;
    ds.read(array_back);
    ASSERT_TRUE(array_back.size() == 3);
    ASSERT_TRUE(array_back[0] == std::string("000"));
    ASSERT_TRUE(array_back[1] == std::string("111"));
    ASSERT_TRUE(array_back[2] == std::string("222"));
    ASSERT_TRUE(array_back.getString(1) == "111");
    ASSERT_TRUE(array_back.front() == std::string("000"));
    ASSERT_TRUE(array_back.back() == std::string("222"));
    ASSERT_TRUE(array_back.data() == std::string("000"));
    array_back.data()[0] = 'x';
    ASSERT_TRUE(array_back.data() == std::string("x00"));

    for (auto& raw_elem : array_back) {
      raw_elem[1] = 'y';
    }
    ASSERT_TRUE(array_back.getString(1) == "1y1");
    for (auto iter = array_back.cbegin(); iter != array_back.cend(); ++iter) {
      ASSERT_TRUE((*iter)[1] == 'y');
    }
  }
}

TEST(H5GTBase, FixedLenStringArrayStructure) {

  using fixed_array_t = FixedLenStringArray<10>;
  // increment the characters of a string written in a std::array
  auto increment_string = [](const fixed_array_t::value_type arr) {
    fixed_array_t::value_type output(arr);
    for (auto& c : output) {
      if (c == 0) {
        break;
      }
      ++c;
    }
    return output;
  };

  // manipulate FixedLenStringArray with std::copy
  {
    const fixed_array_t arr1{"0000000", "1111111"};
    fixed_array_t arr2{"0000000", "1111111"};
    std::copy(arr1.begin(), arr1.end(), std::back_inserter(arr2));
    EXPECT_EQ(arr2.size(), 4);
  }

  // manipulate FixedLenStringArray with std::transform
  {
    fixed_array_t arr;
    {
      const fixed_array_t arr1{"0000000", "1111111"};
      std::transform(arr1.begin(), arr1.end(), std::back_inserter(arr),
                     increment_string);
    }
    EXPECT_EQ(arr.size(), 2);
    EXPECT_EQ(arr[0], std::string("1111111"));
    EXPECT_EQ(arr[1], std::string("2222222"));
  }

  // manipulate FixedLenStringArray with std::transform and reverse iterator
  {
    fixed_array_t arr;
    {
      const fixed_array_t arr1{"0000000", "1111111"};
      std::copy(arr1.rbegin(), arr1.rend(), std::back_inserter(arr));
    }
    EXPECT_EQ(arr.size(), 2);
    EXPECT_EQ(arr[0], std::string("1111111"));
    EXPECT_EQ(arr[1], std::string("0000000"));
  }

  // manipulate FixedLenStringArray with std::remove_copy_if
  {
    fixed_array_t arr2;
    {
      const fixed_array_t arr1{"0000000", "1111111"};
      std::remove_copy_if(arr1.begin(), arr1.end(), std::back_inserter(arr2),
                          [](const fixed_array_t::value_type& s) {
        return std::strncmp(s.data(), "1111111", 7) == 0;
      });
    }
    EXPECT_EQ(arr2.size(), 1);
    EXPECT_EQ(arr2[0], std::string("0000000"));
  }
}

TEST(H5GTBase, Reference) {
  const std::string FILE_NAME("h5_ref_test.h5");
  const std::string DATASET1_NAME("dset1");
  const std::string DATASET2_NAME("dset2");
  const std::string GROUP_NAME("/group1");
  const std::string REFGROUP_NAME("/group2");
  const std::string REFDATASET_NAME("dset2");

  ContentGenerate<double> generator;
  std::vector<double> vec1(4);
  std::vector<double> vec2(4);
  std::generate(vec1.begin(), vec1.end(), generator);
  std::generate(vec2.begin(), vec2.end(), generator);
  {
    // Create a new file using the default property lists.
    File file(FILE_NAME, File::ReadWrite | File::Create | File::Truncate);

    // create group
    Group g1 = file.createGroup(GROUP_NAME);

    // create datasets and write some data
    DataSet dataset1 = g1.createDataSet(DATASET1_NAME, vec1);
    DataSet dataset2 = g1.createDataSet(DATASET2_NAME, vec2);

    // create group to hold reference
    Group refgroup = file.createGroup(REFGROUP_NAME);

    // create the references and write them into a new dataset inside refgroup
    auto references = std::vector<Reference>({{g1, dataset1}, {file, g1}});
    DataSet ref_ds = refgroup.createDataSet(REFDATASET_NAME, references);
  }
  // read it back
  {
    File file(FILE_NAME, File::ReadOnly);
    Group refgroup = file.getGroup(REFGROUP_NAME);

    DataSet refdataset = refgroup.getDataSet(REFDATASET_NAME);
    EXPECT_EQ(2, refdataset.getSpace().getDimensions()[0]);
    auto refs = std::vector<Reference>();
    refdataset.read(refs);
    EXPECT_THROW(refs[0].dereference<Group>(file), h5gt::ReferenceException);
    auto data_ds = refs[0].dereference<DataSet>(file);
    std::vector<double> rdata;
    data_ds.read(rdata);
    for (size_t i = 0; i < rdata.size(); ++i) {
      EXPECT_EQ(rdata[i], vec1[i]);
    }

    auto group = refs[1].dereference<Group>(file);
    DataSet data_ds2 = group.getDataSet(DATASET2_NAME);
    std::vector<double> rdata2;
    data_ds2.read(rdata2);
    for (size_t i = 0; i < rdata2.size(); ++i) {
      EXPECT_EQ(rdata2[i], vec2[i]);
    }
  }
}

#ifdef H5GT_USE_EIGEN

template <typename T>
void test_eigen_vec(File& file,
                    const std::string& test_flavor,
                    const T& vec_input,
                    T& vec_output) {
  const std::string DS_NAME = "ds";
  file.createDataSet(DS_NAME + test_flavor, vec_input).write(vec_input);
  file.getDataSet(DS_NAME + test_flavor).read(vec_output);
  ASSERT_TRUE(vec_input == vec_output);
}

TEST(H5GTBase, Eigen) {
  const std::string FILE_NAME("test_eigen.h5");

  // Create a new file using the default property lists.
  File file(FILE_NAME, File::ReadWrite | File::Create | File::Truncate);
  std::string DS_NAME_FLAVOR;

  // std::vector<of vector <of POD>>
  {
    DS_NAME_FLAVOR = "VectorOfVectorOfPOD";
    std::vector<std::vector<float>> vec_in{
      {5.0f, 6.0f, 7.0f}, {5.1f, 6.1f, 7.1f}, {5.2f, 6.2f, 7.2f}};
    std::vector<std::vector<float>> vec_out;
    test_eigen_vec(file, DS_NAME_FLAVOR, vec_in, vec_out);
  }

  // std::vector<Eigen::Vector3d>
  {
    DS_NAME_FLAVOR = "VectorOfEigenVector3d";
    std::vector<Eigen::Vector3d> vec_in{{5.0, 6.0, 7.0}, {7.0, 8.0, 9.0}};
    std::vector<Eigen::Vector3d> vec_out;
    test_eigen_vec(file, DS_NAME_FLAVOR, vec_in, vec_out);
  }

  // Eigen Vector2d
  {
    DS_NAME_FLAVOR = "EigenVector2d";
    Eigen::Vector2d vec_in{5.0, 6.0};
    Eigen::Vector2d vec_out;

    test_eigen_vec(file, DS_NAME_FLAVOR, vec_in, vec_out);
  }

  // Eigen Matrix
  {
    DS_NAME_FLAVOR = "EigenMatrix";
    Eigen::Matrix<double, 3, 3> vec_in;
    vec_in << 1, 2, 3, 4, 5, 6, 7, 8, 9;
    Eigen::Matrix<double, 3, 3> vec_out;

    test_eigen_vec(file, DS_NAME_FLAVOR, vec_in, vec_out);
  }

  // Eigen MatrixXd
  {
    DS_NAME_FLAVOR = "EigenMatrixXd";
    Eigen::MatrixXd vec_in = 100. * Eigen::MatrixXd::Random(20, 5);
    Eigen::MatrixXd vec_out(20, 5);

    test_eigen_vec(file, DS_NAME_FLAVOR, vec_in, vec_out);
  }

  // std::vector<of EigenMatrixXd>
  {
    DS_NAME_FLAVOR = "VectorEigenMatrixXd";

    Eigen::MatrixXd m1 = 100. * Eigen::MatrixXd::Random(20, 5);
    Eigen::MatrixXd m2 = 100. * Eigen::MatrixXd::Random(20, 5);
    std::vector<Eigen::MatrixXd> vec_in;
    vec_in.push_back(m1);
    vec_in.push_back(m2);
    std::vector<Eigen::MatrixXd> vec_out(2, Eigen::MatrixXd::Zero(20, 5));

    test_eigen_vec(file, DS_NAME_FLAVOR, vec_in, vec_out);
  }

  // std::vector<of EigenMatrixXd> - exception
  {
    const std::string DS_NAME = "ds";
    DS_NAME_FLAVOR = "VectorEigenMatrixXdExc";

    Eigen::MatrixXd m1 = 100. * Eigen::MatrixXd::Random(20, 5);
    Eigen::MatrixXd m2 = 100. * Eigen::MatrixXd::Random(20, 5);
    std::vector<Eigen::MatrixXd> vec_in;
    vec_in.push_back(m1);
    vec_in.push_back(m2);
    file.createDataSet(DS_NAME + DS_NAME_FLAVOR, vec_in).write(vec_in);

    std::vector<Eigen::MatrixXd> vec_out_exception;
    SilenceHDF5 silencer;
    EXPECT_THROW(
          file.getDataSet(DS_NAME + DS_NAME_FLAVOR).read(vec_out_exception),
          h5gt::DataSetException);
  }

#ifdef H5GT_USE_BOOST
  // boost::multi_array<of EigenVector3f>
  {
    DS_NAME_FLAVOR = "BMultiEigenVector3f";

    boost::multi_array<Eigen::Vector3f, 3> vec_in(boost::extents[3][2][2]);
    for (int i = 0; i < 3; ++i) {
      for (int j = 0; j < 2; ++j) {
        for (int k = 0; k < 2; ++k) {
          vec_in[i][j][k] = Eigen::Vector3f::Random(3);
        }
      }
    }
    boost::multi_array<Eigen::Vector3f, 3> vec_out(boost::extents[3][2][2]);

    test_eigen_vec(file, DS_NAME_FLAVOR, vec_in, vec_out);
  }

  // boost::multi_array<of EigenMatrixXd>
  {
    DS_NAME_FLAVOR = "BMultiEigenMatrixXd";

    boost::multi_array<Eigen::MatrixXd, 3> vec_in(boost::extents[3][2][2]);
    for (int i = 0; i < 3; ++i) {
      for (int j = 0; j < 2; ++j) {
        for (int k = 0; k < 2; ++k) {
          vec_in[i][j][k] = Eigen::MatrixXd::Random(3, 3);
        }
      }
    }
    boost::multi_array<Eigen::MatrixXd, 3> vec_out(boost::extents[3][2][2]);
    for (int i = 0; i < 3; ++i)
      for (int j = 0; j < 2; ++j) {
        for (int k = 0; k < 2; ++k) {
          vec_out[i][j][k] = Eigen::MatrixXd::Zero(3, 3);
        }
      }
    test_eigen_vec(file, DS_NAME_FLAVOR, vec_in, vec_out);
  }

  // boost::mulit_array<of EigenMatrixXd> - exception
  {
    const std::string DS_NAME = "ds";
    DS_NAME_FLAVOR = "BMultiEigenMatrixXdExc";

    boost::multi_array<Eigen::MatrixXd, 3> vec_in(boost::extents[3][2][2]);
    for (int i = 0; i < 3; ++i) {
      for (int j = 0; j < 2; ++j) {
        for (int k = 0; k < 2; ++k) {
          vec_in[i][j][k] = Eigen::MatrixXd::Random(3, 3);
        }
      }
    }

    file.createDataSet(DS_NAME + DS_NAME_FLAVOR, vec_in).write(vec_in);
    boost::multi_array<Eigen::MatrixXd, 3> vec_out_exception(boost::extents[3][2][2]);

    SilenceHDF5 silencer;
    EXPECT_THROW(
          file.getDataSet(DS_NAME + DS_NAME_FLAVOR).read(vec_out_exception),
          h5gt::DataSetException);
  }

#endif
}
#endif
