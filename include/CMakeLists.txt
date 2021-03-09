set(files
  h5gt/H5Attribute.hpp
  h5gt/H5DataSet.hpp
  h5gt/H5DataSpace.hpp
  h5gt/H5DataType.hpp
  h5gt/H5Easy.hpp
  h5gt/h5easy_bits
  h5gt/H5Exception.hpp
  h5gt/H5File.hpp
  h5gt/H5FileDriver.hpp
  h5gt/H5Group.hpp
  h5gt/H5Object.hpp
  h5gt/H5PropertyList.hpp
  h5gt/H5Reference.hpp
  h5gt/H5Selection.hpp
  h5gt/H5Utility.hpp
  h5gt/H5Version.hpp.in

h5gt/bits/H5Annotate_traits.hpp
h5gt/bits/H5Annotate_traits_misc.hpp
h5gt/bits/H5Attribute_misc.hpp
h5gt/bits/H5ConverterEigen_misc.hpp
h5gt/bits/H5Converter_misc.hpp
h5gt/bits/H5DataSet_misc.hpp
h5gt/bits/H5Dataspace_misc.hpp
h5gt/bits/H5DataType_misc.hpp
h5gt/bits/H5Exception_misc.hpp
h5gt/bits/H5FileDriver_misc.hpp
h5gt/bits/H5File_misc.hpp
h5gt/bits/H5Iterables_misc.hpp
h5gt/bits/H5Node_traits.hpp
h5gt/bits/H5Node_traits_misc.hpp
h5gt/bits/H5Object_misc.hpp
h5gt/bits/H5PropertyList_misc.hpp
h5gt/bits/H5ReadWrite_misc.hpp
h5gt/bits/H5Reference_misc.hpp
h5gt/bits/H5Selection_misc.hpp
h5gt/bits/H5Slice_traits.hpp
h5gt/bits/H5Slice_traits_misc.hpp
h5gt/bits/H5Utils.hpp
h5gt/bits/H5_definitions.hpp

h5gt/h5easy_bits/H5Easy_Eigen.hpp
h5gt/h5easy_bits/H5Easy_misc.hpp
h5gt/h5easy_bits/H5Easy_opencv.hpp
h5gt/h5easy_bits/H5Easy_public.hpp
h5gt/h5easy_bits/H5Easy_scalar.hpp
h5gt/h5easy_bits/H5Easy_vector.hpp
h5gt/h5easy_bits/H5Easy_xtensor.hpp
  )

foreach(file ${files})
  file(REAL_PATH
    ${file}
    file
  )

  list(APPEND include_files ${file})
endforeach()


file(GLOB _binder_files
     "../cmake_bindings/*.cpp"
     "../cmake_bindings/std/*.cpp"
     "../cmake_bindings/unknown/*.cpp"
)

foreach(binder_file ${_binder_files})
  file(REAL_PATH
    ${binder_file}
    binder_file
  )

  list(APPEND binder_files ${binder_file})
endforeach()

set(binder_files ${binder_files} PARENT_SCOPE)
