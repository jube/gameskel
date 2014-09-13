cmake_minimum_required(VERSION 2.8)

project(GAME CXX)

include(CPackConfig.cmake)

find_package(Boost REQUIRED COMPONENTS filesystem system)

find_package(PkgConfig REQUIRED)
pkg_check_modules(SFML2 REQUIRED sfml-graphics>=2.1 sfml-audio>=2.1)

add_definitions(-Wall -g -O2)
add_definitions(-std=c++11)

include_directories("${CMAKE_SOURCE_DIR}/include")

add_subdirectory(lib)
add_subdirectory(games)

install(
  DIRECTORY "${CMAKE_SOURCE_DIR}/include/game"
  DESTINATION include
)

install(
  DIRECTORY "${CMAKE_SOURCE_DIR}/share/foo"
  DESTINATION share/games
)
