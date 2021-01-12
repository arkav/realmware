cmake_minimum_required(VERSION 3.16)
project(realmware)

set(CMAKE_CXX_STANDARD 17)

set(SUBHOOK_STATIC ON)
set(SUBHOOK_TESTS OFF)
add_subdirectory(thirdparty/minhook)
add_subdirectory(src)
