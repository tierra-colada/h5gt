project(h5gtpy VERSION ${COMMON_PROJECT_VERSION} LANGUAGES C CXX)

message("project: ${PROJECT_NAME} ${COMMON_PROJECT_VERSION}")

set(HDF5_RUNTIME_DIR "" CACHE PATH "hdf5 runtime is a dependency of h5geopy")
option(COPY_H5GTPY_RUNTIME_DEPS "Copy hdf5 runtime to python site-packages
  (otherwise it will simply add to ENV PATH to hdf5 runtime dir in `__init__.py`)" ON)

include(FetchContent)
FetchContent_Declare(
  pybind11
  GIT_REPOSITORY https://github.com/pybind/pybind11
  GIT_TAG        v2.7.1
)

# FetchContent_MakeAvailable(pybind11) # requires cmake ^14 
if(NOT pybind11_POPULATED)
  FetchContent_Populate(pybind11)
  add_subdirectory(${pybind11_SOURCE_DIR} ${pybind11_BINARY_DIR} EXCLUDE_FROM_ALL)  # we need to exclude pybind11 from installation
  message(STATUS "PYTHON_EXECUTABLE: ${PYTHON_EXECUTABLE}")
endif()

file(GLOB_RECURSE h5gtpy_src
     RELATIVE ${PROJECT_SOURCE_DIR}
     ${CMAKE_SOURCE_DIR}/src/h5gtpy/*.cpp
     )
file(GLOB_RECURSE h5gtpy_h
     RELATIVE ${PROJECT_SOURCE_DIR}
     ${CMAKE_SOURCE_DIR}/include/h5gtpy/*.h
     )

pybind11_add_module(_h5gt
  ${h5gtpy_src}
  ${h5gtpy_h}
  )
target_include_directories(_h5gt PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_link_directories(_h5gt PRIVATE pybind11)

if(NOT HDF5_USE_STATIC_LIBRARIES AND EIGEN3_FOUND)
  target_link_libraries(_h5gt PRIVATE h5gt)
else()
  message(FATAL_ERROR "To build h5gtpy shared hdf5 lib and Eigen must be found!")
endif()


#-----------------------------------------------------------------------------
# Install `h5gtpy` as python package
#-----------------------------------------------------------------------------
configure_file(${CMAKE_SOURCE_DIR}/cmake/h5gtpy-install.cmake.in 
  ${CMAKE_CURRENT_BINARY_DIR}/h5gtpy-install.cmake
  @ONLY
  )

# $<CONFIG> at the end of filename is necessary for multiconfig generators
# such as MSVC or Ninja multiconfig (gives error without it:
# "Evaluation file to be written multiple times with different content."
file(GENERATE
  OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/h5gtpy-install-$<CONFIG>.cmake
  INPUT ${CMAKE_CURRENT_BINARY_DIR}/h5gtpy-install.cmake
  )

install(SCRIPT ${CMAKE_CURRENT_BINARY_DIR}/h5gtpy-install-$<CONFIG>.cmake)
