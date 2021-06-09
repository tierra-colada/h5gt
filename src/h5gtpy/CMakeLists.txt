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
  message("PYTHON_EXECUTABLE: ${PYTHON_EXECUTABLE}")
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

if(NOT HDF5_USE_STATIC_LIBRARIES AND H5GT_USE_EIGEN)
  target_link_libraries(_h5gt PRIVATE h5gt)
else()
  message(FATAL_ERROR "To build h5gtpy shared hdf5 lib and Eigen must be found!")
endif()

#-----------------------------------------------------------------------------
# Install `h5gtpy` as python package
#-----------------------------------------------------------------------------
# set(SETUP_PY_IN "${CMAKE_SOURCE_DIR}/cmake/setup.py.in")
# set(SETUP_PY_OUT "${CMAKE_CURRENT_BINARY_DIR}/setup.py")

# set(MANIFEST_IN "${CMAKE_SOURCE_DIR}/cmake/MANIFEST.in.in")
# set(MANIFEST_OUT "${CMAKE_CURRENT_BINARY_DIR}/MANIFEST.in")

# set(INIT_PY_IN "${CMAKE_SOURCE_DIR}/cmake/__init__.py.in")
# set(INIT_PY_OUT "${CMAKE_CURRENT_BINARY_DIR}/h5gtpy/__init__.py")

# set(TEST_PY_IN "${CMAKE_SOURCE_DIR}/tests/pytest/h5gtpy_test.py")
# set(TEST_PY_OUT "${CMAKE_CURRENT_BINARY_DIR}/h5gtpy/h5gtpy_test.py")

# set(STUBS_INIT_IN "${CMAKE_SOURCE_DIR}/stubs/h5gtpy/_h5gt-stubs/h5gt.pyi")
# set(STUBS_INIT_OUT "${CMAKE_CURRENT_BINARY_DIR}/h5gtpy/h5gtpy/h5gt.pyi")

# file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/h5gtpy)
# file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/h5gtpy/h5gtpy)

# # `MODULE_PATTERN` is also used when configuring `MANIFEST.in.in`
# if(WIN32)
#   set(MODULE_PATTERN "*.pyd")
# else()
#   set(MODULE_PATTERN "*.so")
# endif()

# configure_file(${SETUP_PY_IN} ${SETUP_PY_OUT})
# configure_file(${MANIFEST_IN} ${MANIFEST_OUT})
# configure_file(${INIT_PY_IN} ${INIT_PY_OUT} COPYONLY)
# configure_file(${TEST_PY_IN} ${TEST_PY_OUT} COPYONLY)
# configure_file(${STUBS_INIT_IN} ${STUBS_INIT_OUT} COPYONLY)

# file(GLOB_RECURSE _h5gt_modules ${CMAKE_CURRENT_BINARY_DIR}/${MODULE_PATTERN})

# list(LENGTH _h5gt_modules _h5gt_modules_len)
# if (${_h5gt_modules_len} LESS 1)
#   message(FATAL_ERROR "Can't find compiled _h5gt...${MODULE_PATTERN} module to copy it in python package!")
# else()
#   list(GET _h5gt_modules 0 _h5gt_module)
#   message(_h5gt_module: ${_h5gt_module})
# endif()

# install(CODE 
#   "configure_file(${_h5gt_module} ${CMAKE_CURRENT_BINARY_DIR}/h5gtpy COPYONLY)"
#   )
# install(CODE 
#   "execute_process(COMMAND ${PYTHON_EXECUTABLE} -m pip install ${CMAKE_CURRENT_BINARY_DIR})"
#   )

configure_file(${CMAKE_SOURCE_DIR}/cmake/h5gtpy-install.cmake.in 
  ${CMAKE_CURRENT_BINARY_DIR}/h5gtpy-install.cmake
  @ONLY
  )

install(SCRIPT ${CMAKE_CURRENT_BINARY_DIR}/h5gtpy-install.cmake)
