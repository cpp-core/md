cmake_minimum_required (VERSION 3.24 FATAL_ERROR)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/test/bin)

find_package(Threads REQUIRED)

set(TESTS
  md/multi_index
  md/span
  md/shared_data
  )

set(LIBRARIES
  md
  GTest::gtest
  Threads::Threads
  )

configure_tests("core" "${LIBRARIES}" ${TESTS})
