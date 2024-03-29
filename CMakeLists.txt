cmake_minimum_required(VERSION 3.16.0)

# C++14 is required by pybind and xtensor
# C++17 is required by python src -> pack fold
if(NOT DEFINED CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 17)
endif()
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
message(STATUS "Setting C++ standard - " ${CMAKE_CXX_STANDARD})

cmake_policy(SET CMP0074 NEW)
cmake_policy(SET CMP0077 NEW)
cmake_policy(SET CMP0087 NEW) # add generator expressions to `install` command

# `COMMON_PROJECT_VERSION` is also used when configuring `setup.py.in`
set(COMMON_PROJECT_VERSION 0.2.0)
project(h5gt VERSION ${COMMON_PROJECT_VERSION} LANGUAGES C CXX)
message("project: ${CMAKE_PROJECT_NAME} ${COMMON_PROJECT_VERSION}")

if(CMAKE_CXX_COMPILER_IS_ICC)
  # ICC gets mad if we shorten "int"s
  add_definitions("-wd1682")
endif()

#-----------------------------------------------------------------------------
# Add files to search for targets needed
#-----------------------------------------------------------------------------
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

# OPTIONS
option(H5GT_USE_EIGEN "Enable Eigen testing (also needed for `h5gtpy`)" ON)
option(H5GT_USE_XTENSOR "Enable xtensor testing" OFF)
option(H5GT_USE_OPENCV "Enable OpenCV testing" OFF)
option(H5GT_BUILD_TESTS "Build tests" ON)
option(H5GT_EXAMPLES "Compile examples" ON)
option(H5GT_BUILD_h5gtpy "Build python wrapper (make sure to disable HDF5_USE_STATIC_LIBRARIES and enable H5GT_USE_EIGEN)" ON)
option(HDF5_USE_STATIC_LIBRARIES "Use static hdf5 lib" OFF)
option(HDF5_PREFER_PARALLEL "Prefer parallel hdf5 if available" OFF)

set(gtest_force_shared_crt ON CACHE BOOL "Prevent overriding the parent project's compiler/linker settings on Windows")

file(GLOB_RECURSE hpp_files
     RELATIVE ${PROJECT_SOURCE_DIR}
     ${PROJECT_SOURCE_DIR}/include/h5gt/*.hpp
     )

if(${CMAKE_VERSION} VERSION_LESS "3.19.0")
  add_library(h5gt INTERFACE)
else()
  add_library(h5gt INTERFACE ${hpp_files})
endif()

target_include_directories(h5gt INTERFACE
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

find_package(HDF5 REQUIRED)
if(HDF5_PREFER_PARALLEL AND HDF5_IS_PARALLEL)
  target_compile_definitions(h5gt INTERFACE H5GT_PARALLEL)
elseif(HDF5_PREFER_PARALLEL AND NOT HDF5_IS_PARALLEL)
  message(SEND_ERROR "Parallel requested but chosen hdf5 doesn't support it")
endif()

target_link_libraries(h5gt INTERFACE ${HDF5_C_LIBRARIES})
target_include_directories(h5gt INTERFACE ${HDF5_INCLUDE_DIRS})
target_compile_definitions(h5gt INTERFACE ${HDF5_DEFINITIONS})

find_package(ZLIB REQUIRED)
target_link_libraries(h5gt INTERFACE ZLIB::ZLIB)

if(H5GT_BUILD_h5gtpy)
  add_subdirectory(src/h5gtpy)
endif()

if(H5GT_USE_EIGEN)
  find_package(Eigen3 REQUIRED)
  target_include_directories(h5gt INTERFACE ${EIGEN3_INCLUDE_DIRS})
  target_compile_definitions(h5gt INTERFACE H5GT_USE_EIGEN)
endif()

if(H5GT_USE_XTENSOR)
  find_package(xtensor REQUIRED)
  target_link_libraries(h5gt INTERFACE xtensor xtensor::optimize xtensor::use_xsimd)
  target_compile_definitions(h5gt INTERFACE H5GT_USE_XTENSOR)
endif()

if(H5GT_USE_OPENCV)
  find_package(OpenCV REQUIRED)
  target_include_directories(h5gt INTERFACE ${OpenCV_INCLUDE_DIRS})
  target_link_libraries(h5gt INTERFACE ${OpenCV_LIBS})
  target_compile_definitions(h5gt INTERFACE H5GT_USE_OPENCV)
endif()

if(HDF5_PREFER_PARALLEL)
  find_package(MPI REQUIRED)
  target_include_directories(h5gt INTERFACE ${MPI_CXX_INCLUDE_PATH})
  target_link_libraries(h5gt INTERFACE ${MPI_CXX_LIBRARIES})
endif()

if(H5GT_EXAMPLES)
  add_subdirectory(src/examples)
endif()

if(H5GT_BUILD_TESTS)
  include(CTest)
  add_subdirectory(tests)
endif()

#-----------------------------------------------------------------------------
# Create config files
#-----------------------------------------------------------------------------
include(GNUInstallDirs)
include(CMakePackageConfigHelpers)
configure_package_config_file("${CMAKE_CURRENT_SOURCE_DIR}/cmake/h5gt-config.cmake.in"
  "${CMAKE_CURRENT_BINARY_DIR}/h5gt-config.cmake"
  INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/h5gt
  PATH_VARS CMAKE_INSTALL_INCLUDEDIR CMAKE_INSTALL_SYSCONFDIR
  )

install(TARGETS h5gt DESTINATION ${CMAKE_INSTALL_LIBDIR} EXPORT h5gt-targets)
install(EXPORT h5gt-targets DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/h5gt)

write_basic_package_version_file(
  "${CMAKE_CURRENT_BINARY_DIR}/h5gt-config-version.cmake"
  VERSION ${PROJECT_VERSION}
  COMPATIBILITY SameMajorVersion
  )

install(FILES "${CMAKE_CURRENT_BINARY_DIR}/h5gt-config.cmake"
              "${CMAKE_CURRENT_BINARY_DIR}/h5gt-config-version.cmake"
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/h5gt
        )

install(DIRECTORY "${PROJECT_SOURCE_DIR}/include/" DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
