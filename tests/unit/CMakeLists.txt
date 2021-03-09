set(src_files_test
  main.cxx
  tests_high_five.hpp
  tests_high_five_base.cpp
  tests_high_five_easy.cpp
  tests_high_five_multi_dims.cpp
#  tests_high_five_parallel.cpp
#  tests_import_public_headers.cpp
  )

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
