cmake_minimum_required (VERSION 3.3)

project(module2_tests)

set(CMAKE_C_FLAGS "-pthread -lrt")
set(CMAKE_CXX_FLAGS "-pthread -lrt -std=c++11")

#Add google mock/test engine
set(GOOGLE_MOCK_RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}/../googletest)
include_directories(${GOOGLE_MOCK_RELATIVE}/googlemock/include
		    ${GOOGLE_MOCK_RELATIVE}/googletest/include)

#Add tests helper
set(TESTS_HELPER ${CMAKE_CURRENT_SOURCE_DIR}/../tests_helper)
include_directories(${TESTS_HELPER}/public)

set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/source)
set(PRIVATE_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

set(TESTNAMES connectionChecker_test1 mailDevice_test1 mailSender_test1)

foreach(testname ${TESTNAMES})

	add_executable(${testname} ${SRC_DIR}/${testname}.cpp)
	target_link_libraries(${testname} gmock_main tests_helper module2)
	target_include_directories(${testname} PRIVATE ${PRIVATE_INCLUDE_DIR})
	add_dependencies(${testname} module2 tests_helper)

	install(TARGETS ${testname}
    	EXPORT ${testname}-targets
    	RUNTIME DESTINATION  bin COMPONENT bin)

endforeach(testname)
