cmake_minimum_required(VERSION 3.12.0)
cmake_policy(SET CMP0074 NEW)
cmake_policy(SET CMP0077 NEW)
project(h5gt VERSION 0.1.0 LANGUAGES C CXX)

if(CMAKE_CXX_COMPILER_IS_ICC)
  # ICC gets mad if we shorten "int"s
  add_definitions("-wd1682")
endif()

#-----------------------------------------------------------------------------
# Add files to search for targets needed
#-----------------------------------------------------------------------------
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

# OPTIONS
option(H5GT_USE_EIGEN "Enable Eigen testing" ON)
option(H5GT_USE_XTENSOR "Enable xtensor testing" OFF)
option(H5GT_USE_OPENCV "Enable OpenCV testing" OFF)
option(H5GT_UNIT_TESTS "Enable unit tests" ON)
option(H5GT_EXAMPLES "Compile examples" ON)
option(H5GT_PARALLEL_HDF5 "Enable Parallel HDF5 support" OFF)
option(H5GT_USE_SHARED_HDF5 "Use shared hdf5 lib" ON)
option(H5GT_BUILD_h5gtpy "Build python wrapper (uses shared hdf5 lib and Eigen)" ON)

set(gtest_force_shared_crt ON CACHE BOOL "Always use msvcrt.dll") # needed for googletest

find_package(HDF5 REQUIRED CONFIG)

add_subdirectory(include)
add_library(h5gt INTERFACE ${include_files})
target_include_directories(h5gt INTERFACE 
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)
install(TARGETS h5gt DESTINATION lib EXPORT h5gt-targets)

if(H5GT_USE_SHARED_HDF5)
  if(TARGET hdf5-shared)
    target_link_libraries(h5gt INTERFACE hdf5-shared)
  elseif(TARGET hdf5::hdf5-shared)
    target_link_libraries(h5gt INTERFACE hdf5::hdf5-shared)
  else()
    message(FATAL_ERROR "Shared hdf5 lib was not found!")
  endif()
else()
  if(TARGET hdf5-static)
    target_link_libraries(h5gt INTERFACE hdf5-static)
  elseif(TARGET hdf5::hdf5-static)
    target_link_libraries(h5gt INTERFACE hdf5::hdf5-static)
  else()
    message(FATAL_ERROR "Static hdf5 lib was not found!")
  endif()
endif()

if(H5GT_BUILD_h5gtpy)
  # create h5py
  add_subdirectory(src/python)
endif()

if(H5GT_USE_EIGEN)
  find_package(Eigen3 3.3 REQUIRED)
  target_include_directories(h5gt INTERFACE ${EIGEN3_INCLUDE_DIR})
  target_include_directories(h5gtpy PRIVATE ${EIGEN3_INCLUDE_DIR})
  target_compile_definitions(h5gt INTERFACE H5_USE_EIGEN)
endif()

if(H5GT_USE_XTENSOR)
  find_package(xtensor REQUIRED)
  target_link_libraries(h5gt INTERFACE xtensor xtensor::optimize xtensor::use_xsimd)
  target_compile_definitions(h5gt INTERFACE H5_USE_XTENSOR)
endif()

if(H5GT_USE_OPENCV)
  find_package(OpenCV REQUIRED)
  target_include_directories(h5gt INTERFACE ${OpenCV_INCLUDE_DIRS})
  target_link_libraries(h5gt INTERFACE ${OpenCV_LIBS})
  target_compile_definitions(h5gt INTERFACE H5_USE_OPENCV)
endif()

if(H5GT_EXAMPLES)
  add_subdirectory(src/examples)
endif()

if(H5GT_UNIT_TESTS)
  include(CTest)
  add_subdirectory(tests)
endif()

#-----------------------------------------------------------------------------
# Create config files
#-----------------------------------------------------------------------------
set(INCLUDE_INSTALL_DIR "include/")
set(LIB_INSTALL_DIR "lib/")
# set(SYSCONFIG_INSTALL_DIR "etc/h5gt/")

include(CMakePackageConfigHelpers)
configure_package_config_file("${CMAKE_CURRENT_SOURCE_DIR}/cmake/h5gt-config.cmake.in"
  "${CMAKE_CURRENT_BINARY_DIR}/h5gt-config.cmake"
  INSTALL_DESTINATION "${LIB_INSTALL_DIR}/cmake/h5gt/"
  PATH_VARS INCLUDE_INSTALL_DIR
  )

install(EXPORT h5gt-targets DESTINATION "${LIB_INSTALL_DIR}/cmake/h5gt/")
message("PROJECT_VERSION: ${PROJECT_VERSION}")
write_basic_package_version_file(
  "${CMAKE_CURRENT_BINARY_DIR}/h5gt-config-version.cmake"
  VERSION ${PROJECT_VERSION}
  COMPATIBILITY SameMajorVersion
  )

install(FILES "${CMAKE_CURRENT_BINARY_DIR}/h5gt-config.cmake"
              "${CMAKE_CURRENT_BINARY_DIR}/h5gt-config-version.cmake"
        DESTINATION "${LIB_INSTALL_DIR}/cmake/h5gt/"
        )

install(DIRECTORY "${PROJECT_SOURCE_DIR}/include/" DESTINATION "include")
