cmake_minimum_required(VERSION 3.3)

set(MODULE1 module1)
add_subdirectory(${MODULE1} ${CMAKE_BINARY_DIR}/cmake/${MODULE1})
set(MODULE1_TESTS module1_tests)
add_subdirectory(${MODULE1_TESTS} ${CMAKE_BINARY_DIR}/cmake/${MODULE1_TESTS})

