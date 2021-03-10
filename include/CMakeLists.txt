set(files
  include/h5gt/H5Attribute.hpp
  include/h5gt/H5DataSet.hpp
  include/h5gt/H5DataSpace.hpp
  include/h5gt/H5DataType.hpp
  include/h5gt/H5Easy.hpp
  include/h5gt/h5easy_bits
  include/h5gt/H5Exception.hpp
  include/h5gt/H5File.hpp
  include/h5gt/H5FileDriver.hpp
  include/h5gt/H5Group.hpp
  include/h5gt/H5Object.hpp
  include/h5gt/H5PropertyList.hpp
  include/h5gt/H5Reference.hpp
  include/h5gt/H5Selection.hpp
  include/h5gt/H5Utility.hpp
  include/h5gt/H5Version.hpp.in
  include/h5gt/bits/H5Annotate_traits.hpp
  include/h5gt/bits/H5Annotate_traits_misc.hpp
  include/h5gt/bits/H5Attribute_misc.hpp
  include/h5gt/bits/H5ConverterEigen_misc.hpp
  include/h5gt/bits/H5Converter_misc.hpp
  include/h5gt/bits/H5DataSet_misc.hpp
  include/h5gt/bits/H5Dataspace_misc.hpp
  include/h5gt/bits/H5DataType_misc.hpp
  include/h5gt/bits/H5Exception_misc.hpp
  include/h5gt/bits/H5FileDriver_misc.hpp
  include/h5gt/bits/H5File_misc.hpp
  include/h5gt/bits/H5Iterables_misc.hpp
  include/h5gt/bits/H5Node_traits.hpp
  include/h5gt/bits/H5Node_traits_misc.hpp
  include/h5gt/bits/H5Object_misc.hpp
  include/h5gt/bits/H5PropertyList_misc.hpp
  include/h5gt/bits/H5ReadWrite_misc.hpp
  include/h5gt/bits/H5Reference_misc.hpp
  include/h5gt/bits/H5Selection_misc.hpp
  include/h5gt/bits/H5Slice_traits.hpp
  include/h5gt/bits/H5Slice_traits_misc.hpp
  include/h5gt/bits/H5Utils.hpp
  include/h5gt/bits/H5_definitions.hpp
  include/h5gt/h5easy_bits/H5Easy_Eigen.hpp
  include/h5gt/h5easy_bits/H5Easy_misc.hpp
  include/h5gt/h5easy_bits/H5Easy_opencv.hpp
  include/h5gt/h5easy_bits/H5Easy_public.hpp
  include/h5gt/h5easy_bits/H5Easy_scalar.hpp
  include/h5gt/h5easy_bits/H5Easy_vector.hpp
  include/h5gt/h5easy_bits/H5Easy_xtensor.hpp
  )

foreach(file ${files})
  # file(REAL_PATH
  #   ${file}
  #   file
  # )

  list(APPEND include_files ${file})
endforeach()
