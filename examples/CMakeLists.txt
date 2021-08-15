cmake_minimum_required(VERSION 3.14)

project(utcp_example CXX)

file(GLOB_RECURSE SERVER_SRC "${CMAKE_CURRENT_SOURCE_DIR}/server/*.cpp")
add_executable(server ${SERVER_SRC})
target_link_libraries(server udpsocket)


file(GLOB_RECURSE CLIENT_SRC "${CMAKE_CURRENT_SOURCE_DIR}/client/*.cpp")
add_executable(client ${CLIENT_SRC})
target_link_libraries(client udpsocket)
