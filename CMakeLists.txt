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

include_directories(${Boost_INCLUDE_DIRS})
include_directories(${SFML2_INCLUDE_DIRS})

include_directories(${CMAKE_CURRENT_BINARY_DIR})
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/config.h.in ${CMAKE_CURRENT_BINARY_DIR}/config.h @ONLY)

add_executable(game_template
  main.cc
  # base
  game/Assets.cc
  game/Clock.cc
  game/Event.cc
  game/Log.cc
  game/Random.cc
  # graphics
  game/Action.cc
  game/Animation.cc
  game/Camera.cc
  game/Control.cc
  game/Entity.cc
  game/EntityManager.cc
  game/Resources.cc
  game/WindowSettings.cc
  # model
  game/Model.cc
  game/ModelManager.cc
)

target_link_libraries(game_template
  ${CMAKE_THREAD_LIBS_INIT}
  ${Boost_LIBRARIES}
  ${SFML2_LIBRARIES}
)

install(
  TARGETS game_template
  RUNTIME DESTINATION games
)

install(
  DIRECTORY "${CMAKE_SOURCE_DIR}/share/${PROJECT_NAME}"
  DESTINATION "${CMAKE_INSTALL_DATAROOTDIR}/games"
)

find_package(Doxygen)

if(DOXYGEN_FOUND)
  configure_file("${CMAKE_SOURCE_DIR}/Doxyfile.cmake" "${CMAKE_BINARY_DIR}/Doxyfile" @ONLY)
  add_custom_target(doc ${DOXYGEN_EXECUTABLE} "${CMAKE_BINARY_DIR}/Doxyfile")
endif(DOXYGEN_FOUND)
