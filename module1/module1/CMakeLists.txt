cmake_minimum_required (VERSION 3.3)

project(module1)

set(CMAKE_C_FLAGS "-pthread -lrt -g -O0 --coverage")
set(CMAKE_CXX_FLAGS "-pthread -lrt -std=c++17 -g -O0 --coverage")

set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/source)
set(INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/public)
set(PRIVATE_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

file(GLOB allCodeFiles "${SRC_DIR}/*.cpp")

#----------- Shared library -------------------------
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
add_library(${PROJECT_NAME} SHARED ${allCodeFiles})

target_include_directories(${PROJECT_NAME}
    PUBLIC $<BUILD_INTERFACE:${INCLUDE_DIR}>
    PRIVATE ${PRIVATE_INCLUDE_DIR}
)

install(TARGETS ${PROJECT_NAME}
    EXPORT ${PROJECT_NAME}-targets
    RUNTIME DESTINATION  bin COMPONENT bin
    ARCHIVE DESTINATION  lib COMPONENT shlib
    LIBRARY DESTINATION  lib COMPONENT shlib
    INCLUDES DESTINATION include/${PROJECT_NAME})

install(DIRECTORY ${INCLUDE_DIR}/ DESTINATION include/${PROJECT_NAME} COMPONENT dev)
