include(FetchContent)
FetchContent_Declare(
  pybind11
  GIT_REPOSITORY https://github.com/pybind/pybind11
  GIT_TAG        v2.6
)

FetchContent_MakeAvailable(pybind11)

pybind11_add_module(h5gtpy ${include_files} h5gtpy.cpp)
target_link_directories(h5gtpy PRIVATE pybind11)
install(TARGETS h5gtpy DESTINATION lib EXPORT h5gt-targets)

if(TARGET hdf5-shared)
  target_link_libraries(h5gtpy PRIVATE hdf5-shared)
elseif(TARGET hdf5::hdf5-shared)
  target_link_libraries(h5gtpy PRIVATE hdf5::hdf5-shared)
else()
  message(FATAL_ERROR "To build h5gtpy shared hdf5 lib is needed!")
endif()