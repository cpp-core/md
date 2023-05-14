cmake_minimum_required (VERSION 3.24 FATAL_ERROR)
project (md VERSION 0.1 LANGUAGES C CXX)

# Make including this file idempotent
#
if(TARGET md OR TARGET md::md)
  return()
endif()

# If this is the current project.
#
if(PROJECT_NAME STREQUAL CMAKE_PROJECT_NAME)
  # Laod the cmake recipes and utilities.
  #
  include(${CMAKE_CURRENT_LIST_DIR}/cmake/load_cmake_helpers.cmake)
  
  # Options for generating tests and documentation.
  #
  option(MD_TEST "Generate the tests." ON)
  option(MD_DOCS "Generate the docs." OFF)

  # compile_commands.json
  #
  symlink_compile_commands()

  # Convenience targets for git operations
  #
  make_submodule_pull_target()
  
else()
  option(MD_TEST "Generate the tests." OFF)
  option(MD_DOCS "Generate the docs." OFF)
endif()

# Put executables in the top-level binary directory
#
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Output configuration
#
message("-- md: Configuration: ${CMAKE_CURRENT_SOURCE_DIR}")
message("-- md: Included from: ${CMAKE_SOURCE_DIR}")
message("-- md: Install prefix ${CMAKE_INSTALL_PREFIX}")
message("-- md: test ${MD_TEST}")
message("-- md: docs ${MD_DOCS}")

# Setup compilation
#
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_compile_options(-Wall)
add_compile_options(-stdlib=libc++)
add_compile_options(-O)
add_link_options(-stdlib=libc++)

# Add our dependencies
#
add_mdspan()

# Build the library
#
add_library(md INTERFACE)
add_library(md::md ALIAS md) # Allow interop between FetchContent and FindPackage.

# Define the header files for this library.
#
file(GLOB_RECURSE PUBLIC_INCLUDE_FILES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} include/*.h)
target_sources(md INTERFACE FILE_SET HEADERS BASE_DIRS include FILES ${PUBLIC_INCLUDE_FILES})

target_link_libraries(md INTERFACE std::mdspan)

foreach(prog md0)
  add_executable(${prog} src/tools/${prog}.cpp)
  target_link_libraries(${prog} md::md)
endforeach()

# Optionally configure the tests
#
if(MD_TEST)
  add_gtest()
  add_subdirectory(test)
endif()

# Optionally configure the documentation
#
if(MD_DOCS)
  add_subdirectory(docs)
endif()

# Install config
#
install_config(md)
