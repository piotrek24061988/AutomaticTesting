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

#Add google mock engine
set(GOOGLE_MOCK_RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}/../googletest)
include_directories(${GOOGLE_MOCK_RELATIVE}/googlemock/include
		    ${GOOGLE_MOCK_RELATIVE}/googletest/include)

add_library(cppunit_shared SHARED IMPORTED GLOBAL)
set_property(TARGET cppunit_shared PROPERTY IMPORTED_LOCATION ${CMAKE_INSTALL_PREFIX}/lib/libcppunit.so)
add_dependencies(cppunit_shared cppunit)
add_library(cppunit_static STATIC IMPORTED GLOBAL)
set_property(TARGET cppunit_static PROPERTY IMPORTED_LOCATION ${CMAKE_INSTALL_PREFIX}/lib/libcppunit.a)
add_dependencies(cppunit_static cppunit)

include_directories(${CPP_UNIT_RELATIVE}/include)

set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/source)
set(PRIVATE_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

set(TESTNAMES smsPlanner_test2 timeKeeper_test2 smsSender_test2)

foreach(testname ${TESTNAMES})

	add_executable(${testname} ${SRC_DIR}/${testname}.cpp)
	target_link_libraries(${testname} cppunit_shared gmock_main module1)
	target_include_directories(${testname} PRIVATE ${PRIVATE_INCLUDE_DIR})
	add_dependencies(${testname} cppunit_shared module1)

	install(TARGETS ${testname}
    	EXPORT ${testname}-targets
    	RUNTIME DESTINATION  bin COMPONENT bin)

endforeach(testname)
