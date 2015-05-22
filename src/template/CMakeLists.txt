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
  game/Control.cc
  game/Entity.cc
  game/EntityManager.cc
  game/Resources.cc
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

