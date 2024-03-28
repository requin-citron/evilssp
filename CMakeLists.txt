cmake_minimum_required(VERSION 3.8.2) # 3.8.0 is minimum version that allow Linux remote building and debugging

project(evilssp)

set(CMAKE_SYSTEM_NAME "Windows")
set(CMAKE_SYSTEM_PROCESSOR "x86_64")
set(CMAKE_SHARED_LIBRARY_PREFIX "")


# Remove trailing '\' and '/' at the end of the path, so can reliably compare
STRING(REGEX REPLACE "[/\\]+$" "" CMAKE_SOURCE_DIR ${CMAKE_SOURCE_DIR})
STRING(REGEX REPLACE "[/\\]+$" "" CMAKE_CURRENT_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})

# Source files
file(GLOB SRC_FILES "src/*.c")

add_library(evilssp SHARED ${SRC_FILES})

# set dll extention
set_target_properties(evilssp PROPERTIES SUFFIX ".dll")

target_include_directories(evilssp PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_link_libraries(evilssp PRIVATE ws2_32 dnsapi winhttp)


