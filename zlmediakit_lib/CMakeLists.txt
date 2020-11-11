cmake_minimum_required(VERSION 3.0)
set (CMAKE_CXX_STANDARD 17)
project(lib_ZLMediaKit_LIB CXX)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")

#Disable ZLMediaKit tests
set(ENABLE_TESTS OFF FORCE)
option(ENABLE_TESTS OFF)
#No openssl for ZLMediaKit for now, because it uses 1.1-dev and ubuntu bionic only has 1.0-dev
#TODO: enable OpenSSL since we're building in openvpn3 
set(ENABLE_OPENSSL OFF FORCE)
option(ENABLE_OPENSSL OFF)

add_subdirectory(ZLMediaKit)

add_library(zlmediakit_cpp_interface STATIC interface.cpp ZLMedia.cpp CustomSocket.cpp)
target_include_directories(zlmediakit_cpp_interface PUBLIC 
. 
${CMAKE_CURRENT_SOURCE_DIR}/ZLMediaKit/src 
${CMAKE_CURRENT_SOURCE_DIR}/ZLMediaKit/3rdpart/ZLToolKit/src)
target_link_libraries(zlmediakit_cpp_interface mpeg mov flv libstdc++ zlmediakit zltoolkit)
#install(TARGETS zlmediakit zltoolkit mpeg mov flv DESTINATION .)
install(TARGETS zlmediakit_cpp_interface zltoolkit zlmediakit mpeg mov flv DESTINATION .)