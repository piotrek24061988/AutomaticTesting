cmake_minimum_required (VERSION 3.3)

project(module1_tests)

include_directories(${PROJECT_SOURCE_DIR})

set(GOOGLE_MOCK_RELATIVE ../../googletest)
add_subdirectory(${GOOGLE_MOCK_RELATIVE} ${CMAKE_BINARY_DIR}/cmake/googlemock)
include_directories(${PROJECT_SOURCE_DIR}/${GOOGLE_MOCK_RELATIVE}/googlemock/include
		    ${PROJECT_SOURCE_DIR}/${GOOGLE_MOCK_RELATIVE}/googletest/include)

set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/source)
set(PRIVATE_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

ENABLE_TESTING()

set(TESTNAMES smsPlanner_test1 timeKeeper_test1 smsSender_test1)

foreach(testname ${TESTNAMES})
	add_executable(${testname} ${SRC_DIR}/${testname}.cpp)
	target_link_libraries(${testname} gmock_main module1)
	target_include_directories(${testname} PRIVATE ${PRIVATE_INCLUDE_DIR})
	add_dependencies(${testname} module1)

	install(TARGETS ${testname}
    	EXPORT ${testname}-targets
    	RUNTIME DESTINATION  bin COMPONENT bin)

	add_test(${testname} ${testname})

endforeach(testname)
