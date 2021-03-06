/*
 *  Copyright (c), 2017, Adrien Devresse <adrien.devresse@epfl.ch>
 *
 *  Distributed under the Boost Software License, Version 1.0.
 *    (See accompanying file LICENSE_1_0.txt or copy at
 *          http://www.boost.org/LICENSE_1_0.txt)
 *
 */
#ifndef H5FILE_MISC_HPP
#define H5FILE_MISC_HPP

#include <string>

#include <H5Fpublic.h>

#include "../H5Utility.hpp"

namespace h5gt {

namespace {  // unnamed

// libhdf5 uses a preprocessor trick on their oflags
// we can not declare them constant without a mapper
inline unsigned convert_open_flag(unsigned openFlags) {
  unsigned res_open = 0;
  if (openFlags & File::ReadOnly)
    res_open |= H5F_ACC_RDONLY;
  if (openFlags & File::ReadWrite)
    res_open |= H5F_ACC_RDWR;
  if (openFlags & File::Create)
    res_open |= H5F_ACC_CREAT;
  if (openFlags & File::Truncate)
    res_open |= H5F_ACC_TRUNC;
  if (openFlags & File::Excl)
    res_open |= H5F_ACC_EXCL;
  return res_open;
}
}  // namespace


inline File::File(const std::string& filename, unsigned openFlags,
                  const FileAccessProps& fileAccessProps){

  openFlags = convert_open_flag(openFlags);

  unsigned createMode = openFlags & (H5F_ACC_TRUNC | H5F_ACC_EXCL);
  unsigned openMode = openFlags & (H5F_ACC_RDWR | H5F_ACC_RDONLY);
  bool mustCreate = createMode > 0;
  bool openOrCreate = (openFlags & H5F_ACC_CREAT) > 0;

  // open is default. It's skipped only if flags require creation
  // If open fails it will try create() if H5F_ACC_CREAT is set
  if (!mustCreate) {
    // Silence open errors if create is allowed
    std::unique_ptr<SilenceHDF5> silencer;
    if (openOrCreate) silencer.reset(new SilenceHDF5());

    _hid = H5Fopen(filename.c_str(), openMode, fileAccessProps.getId(false));

    if (isValid()) return;  // Done

    if (openOrCreate) {
      // Will attempt to create ensuring wont clobber any file
      createMode = H5F_ACC_EXCL;
    } else {
      HDF5ErrMapper::ToException<FileException>(
            std::string("Unable to open file " + filename));
    }
  }

  if ((_hid = H5Fcreate(filename.c_str(), createMode, H5P_DEFAULT,
                        fileAccessProps.getId(false))) < 0) {
    HDF5ErrMapper::ToException<FileException>(
          std::string("Unable to create file " + filename +
                      ". Probably you don't have permissions, incorrect directory or the file is busy by another process"));
  }
}

inline bool File::operator==(const File& other) const {
#if (H5_VERS_MAJOR >= 1 && H5_VERS_MINOR >= 12)
  unsigned long fileNumLeft, fileNumRight;

  if (H5Fget_fileno(_hid, &fileNumLeft) < 0){
    HDF5ErrMapper::ToException<FileException>(
          std::string("Unable to get file number for " + getFileName()));
  }

  if (H5Fget_fileno(other.getId(false), &fileNumRight) < 0){
    HDF5ErrMapper::ToException<FileException>(
          std::string("Unable to get file number for " + other.getFileName()));
  }
  return fileNumLeft == fileNumRight;
#else
  return getObjectInfo().getFileNumber() == other.getObjectInfo().getFileNumber();
#endif
}

inline bool File::operator!=(const File& other) const {
  return !(*this == other);
}

}  // namespace h5gt

#endif  // H5FILE_MISC_HPP
