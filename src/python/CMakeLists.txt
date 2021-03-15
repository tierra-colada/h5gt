project(h5gtpy VERSION ${COMMON_PROJECT_VERSION} LANGUAGES C CXX)
message("project: ${PROJECT_NAME}")

include(FetchContent)
FetchContent_Declare(
  pybind11
  GIT_REPOSITORY https://github.com/pybind/pybind11
  GIT_TAG        v2.6
)

# FetchContent_MakeAvailable(pybind11) # requires cmake ^14 
if(NOT pybind11_POPULATED)
  FetchContent_Populate(pybind11)
  add_subdirectory(${pybind11_SOURCE_DIR} ${pybind11_BINARY_DIR} EXCLUDE_FROM_ALL)  # we need to exclude pybind11 from installation
endif()

pybind11_add_module(h5gtpy h5gtpy.cpp)
target_link_directories(h5gtpy PRIVATE pybind11)
install(TARGETS h5gtpy DESTINATION lib EXPORT h5gt-targets)

if(NOT HDF5_USE_STATIC_LIBRARIES AND H5GT_USE_EIGEN)
  target_link_libraries(h5gtpy PRIVATE h5gt)
else()
  message(FATAL_ERROR "To build h5gtpy shared hdf5 lib and Eigen must be found!")
endif()
