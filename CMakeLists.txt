cmake_minimum_required(VERSION 2.8)

project(GAME
  LANGUAGES C CXX
)

set(PROJECT_NAME "TODO")
set(PROJECT_VERSION 0.3.0)

include(GNUInstallDirs)

find_package(Threads)
find_package(Boost REQUIRED COMPONENTS filesystem system)

find_package(PkgConfig REQUIRED)
pkg_check_modules(SFML2 REQUIRED sfml-graphics>=2.1 sfml-audio>=2.1)

add_definitions(-Wall -g -O2)
add_definitions(-std=c++11)

add_subdirectory(code)

install(
  DIRECTORY "${CMAKE_SOURCE_DIR}/data/${PROJECT_NAME}"
  DESTINATION "${CMAKE_INSTALL_DATAROOTDIR}/games"
)

find_package(Doxygen)

if(DOXYGEN_FOUND)
  configure_file("${CMAKE_SOURCE_DIR}/Doxyfile.cmake" "${CMAKE_BINARY_DIR}/Doxyfile" @ONLY)
  add_custom_target(doc ${DOXYGEN_EXECUTABLE} "${CMAKE_BINARY_DIR}/Doxyfile")
endif(DOXYGEN_FOUND)
