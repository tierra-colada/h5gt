project(h5gt_examples VERSION ${COMMON_PROJECT_VERSION} LANGUAGES C CXX)
message("project: ${PROJECT_NAME}")

include_directories(${HDF5_INCLUDE_DIRS})

function(compile_example exemple_source)

    get_filename_component(example_filename ${exemple_source} NAME)
    string(REPLACE ".cpp" "_bin" example_name ${example_filename})

    if(${example_filename} MATCHES ".*boost.*")
        if(NOT H5GT_USE_BOOST)
            return()
        endif()
    endif()

    if(${example_filename} MATCHES ".*parallel_hdf5.*")
        if(NOT HDF5_PREFER_PARALLEL)
            return()
        endif()
    endif()

    add_executable(${example_name} ${exemple_source})
    target_link_libraries(${example_name} h5gt)

endfunction()

file(GLOB list_example "*.cpp")

foreach(example_src ${list_example})
    compile_example(${example_src})
endforeach()
