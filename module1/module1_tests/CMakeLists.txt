cmake_minimum_required (VERSION 3.3)

project(module1_tests)

set(CMAKE_C_FLAGS "-pthread -lrt")
set(CMAKE_CXX_FLAGS "-pthread -lrt -std=c++17")

#Add google mock/test engine
set(GOOGLE_MOCK_RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}/../googletest)
add_subdirectory(${GOOGLE_MOCK_RELATIVE} ${CMAKE_BINARY_DIR}/cmake/googlemock)
include_directories(${GOOGLE_MOCK_RELATIVE}/googlemock/include
		    ${GOOGLE_MOCK_RELATIVE}/googletest/include)

#Add tests helper
set(TESTS_HELPER ${CMAKE_CURRENT_SOURCE_DIR}/../tests_helper)
add_subdirectory(${TESTS_HELPER} ${CMAKE_BINARY_DIR}/cmake/tests_helper)
include_directories(${TESTS_HELPER}/public)

set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/source)
set(PRIVATE_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

set(TESTNAMES smsPlanner_test1 timeKeeper_test1 smsSender_test1 smsDevice_test1)

foreach(testname ${TESTNAMES})

	add_executable(${testname} ${SRC_DIR}/${testname}.cpp)
	target_link_libraries(${testname} gmock_main tests_helper module1)
	target_include_directories(${testname} PRIVATE ${PRIVATE_INCLUDE_DIR})
	add_dependencies(${testname} module1 tests_helper)

	install(TARGETS ${testname}
    	EXPORT ${testname}-targets
    	RUNTIME DESTINATION  bin COMPONENT bin)

endforeach(testname)
