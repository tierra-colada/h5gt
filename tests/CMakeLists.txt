project(h5gt_tests VERSION ${COMMON_PROJECT_VERSION} LANGUAGES C CXX)
message("project: ${PROJECT_NAME}")

include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        release-1.11.0
)

FetchContent_GetProperties(googletest)
if(NOT googletest_POPULATED)
  FetchContent_Populate(googletest)
  add_subdirectory(${googletest_SOURCE_DIR} ${googletest_BINARY_DIR} EXCLUDE_FROM_ALL)  # we need to exclude googletest from installation
endif()

add_subdirectory(unit)
