set(src_files_test
  main.cxx
  tests_high_five.hpp
  tests_high_five_base.cpp
  tests_high_five_easy.cpp
  tests_high_five_multi_dims.cpp
  )

if(H5GT_PARALLEL_HDF5)
  list(APPEND src_files_test tests_high_five_parallel.cpp)
endif()

add_executable(H5GTTest
  ${src_files_test}
  )

target_link_libraries(H5GTTest 
  PRIVATE ${GMOCK_BOTH_LIBRARIES}
  PRIVATE gtest_main
  PRIVATE gmock_main
  PRIVATE h5gt
  )

add_test(
  NAME H5GTTest
  COMMAND H5GTTest
  )

file(GLOB public_headers LIST_DIRECTORIES false RELATIVE ${PROJECT_SOURCE_DIR}/include ${PROJECT_SOURCE_DIR}/include/h5gt/*.hpp)
foreach(PUBLIC_HEADER ${public_headers})
  get_filename_component(CLASS_NAME ${PUBLIC_HEADER} NAME_WE)
  configure_file(tests_import_public_headers.cpp "tests_${CLASS_NAME}.cpp" @ONLY)
  add_executable("tests_include_${CLASS_NAME}" "${CMAKE_CURRENT_BINARY_DIR}/tests_${CLASS_NAME}.cpp")
  target_link_libraries("tests_include_${CLASS_NAME}" h5gt)
endforeach()
