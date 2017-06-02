cmake_minimum_required(VERSION 3.3)

#make install DESTDIR=out
#https://www.youtube.com/watch?v=HHpRyNO1J0g&t=321s 12m

set(MODULE1 module1)
add_subdirectory(${MODULE1} ${CMAKE_BINARY_DIR}/cmake/${MODULE1})
set(MODULE1_TESTS module1_tests)
add_subdirectory(${MODULE1_TESTS} ${CMAKE_BINARY_DIR}/cmake/${MODULE1_TESTS})

set(CMAKE_C_FLAGS "-pthread -lrt" CACHE STRING "Compiler flags" FORCE)
set(CMAKE_CXX_FLAGS "-pthread -lrt -frtti -std=c++11" CACHE STRING "Compiler flags" FORCE)
