cmake_minimum_required(VERSION 3.13)

# Copy example files to the build directory
file(GLOB EXAMPLE_FILES "*.tri")
foreach(EXAMPLE_FILE ${EXAMPLE_FILES})
    get_filename_component(FILENAME ${EXAMPLE_FILE} NAME)
    configure_file(${EXAMPLE_FILE} ${CMAKE_CURRENT_BINARY_DIR}/${FILENAME} COPYONLY)
endforeach()