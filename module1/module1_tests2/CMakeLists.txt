cmake_minimum_required (VERSION 3.3)

project(module1_tests2)

set(CMAKE_C_FLAGS "-pthread -lrt")
set(CMAKE_CXX_FLAGS "-pthread -lrt -std=c++11")

#Add cppunit engine
include (ExternalProject)
set(CPP_UNIT_RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}/../cppunit)
ExternalProject_Add(cppunit
    SOURCE_DIR ${CPP_UNIT_RELATIVE}/src
    UPDATE_COMMAND ${CPP_UNIT_RELATIVE}/autogen.sh
    CONFIGURE_COMMAND ${CPP_UNIT_RELATIVE}/configure CC=${CMAKE_C_COMPILER} CXX=${CMAKE_CXX_COMPILER} --prefix=${CMAKE_INSTALL_PREFIX}
    BUILD_COMMAND make
    INSTALL_COMMAND make install)

add_library(cppunit_shared SHARED IMPORTED GLOBAL)
set_property(TARGET cppunit_shared PROPERTY IMPORTED_LOCATION ${CMAKE_INSTALL_PREFIX}/lib/libcppunit.so)
add_dependencies(cppunit_shared cppunit)
add_library(cppunit_static STATIC IMPORTED GLOBAL)
set_property(TARGET cppunit_static PROPERTY IMPORTED_LOCATION ${CMAKE_INSTALL_PREFIX}/lib/libcppunit.a)
add_dependencies(cppunit_static cppunit)

include_directories(${PROJECT_SOURCE_DIR}/${CPP_UNIT_RELATIVE}/include)

set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/source)
set(PRIVATE_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

set(TESTNAME smsPlanner_test2)

add_executable(${TESTNAME} ${SRC_DIR}/main.cpp)
add_dependencies(${TESTNAME} module1 cppunit_shared)
target_link_libraries(${TESTNAME} cppunit_shared module1)
target_include_directories(${TESTNAME} PRIVATE ${PRIVATE_INCLUDE_DIR})

install(TARGETS ${TESTNAME}
EXPORT ${TESTNAME}-targets
RUNTIME DESTINATION  bin COMPONENT bin)
