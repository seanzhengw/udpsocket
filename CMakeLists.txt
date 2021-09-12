cmake_minimum_required(VERSION 3.14)

project(udpsocket CXX)

option(BUILD_EXAMPLES off)

set(CMAKE_CXX_STANDARD 11)

file(GLOB_RECURSE UDPSOCKET_SRC "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

# object library
add_library(udpsocket_obj OBJECT ${UDPSOCKET_SRC})
set_property(TARGET udpsocket_obj PROPERTY POSITION_INDEPENDENT_CODE 1)
target_include_directories(udpsocket_obj PUBLIC include/)

# shared library
add_library(udpsocket_shared SHARED $<TARGET_OBJECTS:udpsocket_obj>)
set_target_properties(udpsocket_shared PROPERTIES LIBRARY_OUTPUT_NAME udpsocket)
target_include_directories(udpsocket_shared PUBLIC include/)

# static library
add_library(udpsocket STATIC $<TARGET_OBJECTS:udpsocket_obj>)
target_include_directories(udpsocket PUBLIC include/)

if(WIN32)
target_link_libraries(udpsocket_obj PUBLIC ws2_32)
target_link_libraries(udpsocket_shared PUBLIC ws2_32)
target_link_libraries(udpsocket PUBLIC ws2_32)
endif()

# public headers
set(UDPSOCKET_HEADERS 
    include/udpsocket/SockAddr.h 
    include/udpsocket/UDPSocket.h
)
set_target_properties(udpsocket PROPERTIES PUBLIC_HEADER "${UDPSOCKET_HEADERS}")

install(
    TARGETS udpsocket_obj udpsocket_shared udpsocket
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib/static
    PUBLIC_HEADER DESTINATION include/udpsocket
)

if(BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()
