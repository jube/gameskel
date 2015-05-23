cmake_minimum_required(VERSION 3.0)

project(GAME
  VERSION 0.2.0
  LANGUAGES C CXX
)

include(GNUInstallDirs)

find_package(Threads)
find_package(Boost REQUIRED COMPONENTS filesystem system)

find_package(PkgConfig REQUIRED)
pkg_check_modules(SFML2 REQUIRED sfml-graphics>=2.1 sfml-audio>=2.1)

add_definitions(-Wall -g -O2)
add_definitions(-std=c++11)

add_subdirectory(template)

install(
  DIRECTORY "${CMAKE_SOURCE_DIR}/share/foo"
  DESTINATION "${CMAKE_INSTALL_DATAROOTDIR}/games"
)

find_package(Doxygen)

if(DOXYGEN_FOUND)
  configure_file("${CMAKE_SOURCE_DIR}/Doxyfile.cmake" "${CMAKE_BINARY_DIR}/Doxyfile" @ONLY)
  add_custom_target(doc ${DOXYGEN_EXECUTABLE} "${CMAKE_BINARY_DIR}/Doxyfile")
endif(DOXYGEN_FOUND)
