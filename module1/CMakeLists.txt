cmake_minimum_required(VERSION 3.3)

set(MODULE1 module1)
add_subdirectory(${MODULE1} ${CMAKE_BINARY_DIR}/cmake/${MODULE1})

#enable IntegrationTests flag for integration tests, otherwise unit tests enabled by default
#add_definitions(-DIntegrationTests)

#tests written based on googletest and googlemock
set(MODULE1_TESTS module1_tests)
add_subdirectory(${MODULE1_TESTS} ${CMAKE_BINARY_DIR}/cmake/${MODULE1_TESTS})

#tests written based on cppunit and googlemock 
#set(MODULE1_TESTS2 module1_tests2)
#add_subdirectory(${MODULE1_TESTS2} ${CMAKE_BINARY_DIR}/cmake/${MODULE1_TESTS2})

###############################################################################################

set(MODULE2 module2)
add_subdirectory(${MODULE2} ${CMAKE_BINARY_DIR}/cmake/${MODULE2})


