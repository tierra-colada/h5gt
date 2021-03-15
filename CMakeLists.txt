cmake_minimum_required(VERSION 3.15.0)

if(NOT DEFINED CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 14) # C++14 is required by pybind and xtensor
endif()
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

cmake_policy(SET CMP0074 NEW)
cmake_policy(SET CMP0077 NEW)

set(COMMON_PROJECT_VERSION 0.1.0)
project(h5gt VERSION ${COMMON_PROJECT_VERSION} LANGUAGES C CXX)
message("project: ${CMAKE_PROJECT_NAME}")

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
option(H5GT_BUILD_TESTS "Build tests" ON)
option(H5GT_EXAMPLES "Compile examples" ON)
option(H5GT_PARALLEL_HDF5 "Enable Parallel HDF5 support" OFF)
option(H5GT_BUILD_h5gtpy "Build python wrapper (make sure to disable HDF5_USE_STATIC_LIBRARIES and enable H5GT_USE_EIGEN)" ON)
option(HDF5_USE_STATIC_LIBRARIES "Use static hdf5 lib" OFF)

set(gtest_force_shared_crt ON CACHE BOOL "Prevent overriding the parent project's compiler/linker settings on Windows")

add_library(h5gt INTERFACE)
target_include_directories(h5gt INTERFACE
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)
install(TARGETS h5gt DESTINATION lib EXPORT h5gt-targets)

find_package(HDF5 REQUIRED)
if(H5GT_PARALLEL_HDF5 AND HDF5_IS_PARALLEL)
  target_compile_definitions(h5gt INTERFACE H5GT_PARALLEL)
elseif(H5GT_PARALLEL_HDF5 AND NOT HDF5_IS_PARALLEL)
  message(SEND_ERROR "Parallel requested but chosen hdf5 doesnt support it")
endif()

target_link_libraries(h5gt INTERFACE ${HDF5_C_LIBRARIES})
target_include_directories(h5gt INTERFACE ${HDF5_INCLUDE_DIRS})
target_compile_definitions(h5gt INTERFACE ${HDF5_DEFINITIONS})

find_package(ZLIB REQUIRED)
target_link_libraries(h5gt INTERFACE ZLIB::ZLIB)

if(H5GT_BUILD_h5gtpy)
  add_subdirectory(src/python)
endif()

if(H5GT_USE_EIGEN)
  find_package(Eigen3 3.3 REQUIRED)
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

if(H5GT_PARALLEL_HDF5)
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
