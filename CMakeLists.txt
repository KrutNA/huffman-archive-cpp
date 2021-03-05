cmake_minimum_required(VERSION 3.10)

set(EXECUTABLE_NAME huffman-archive)

project(${EXECUTABLE_NAME})

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_COMPILER g++)
set(CMAKE_CXX_FLAGS_RELEASE -O3)

if (CMAKE_BUILD_TYPE EQUAL "DEBUG")
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ./bin/debug)
else ()
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ./bin/release)
endif ()

set(INCLUDE_DIR ./include)
set(SRC_DIR ./src)

file(GLOB SRCS
  ${SRC_DIR}/main.cpp
  ${SRC_DIR}/huffman.cpp
  ${SRC_DIR}/binary_map.cpp
  )

include_directories(
  ${INCLUDE_DIR}
  )

add_executable(${EXECUTABLE_NAME} ${SRCS})
