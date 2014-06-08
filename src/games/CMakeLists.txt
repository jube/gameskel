include_directories(${Boost_INCLUDE_DIRS})

include_directories(${CMAKE_CURRENT_BINARY_DIR})
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/config.h.in ${CMAKE_CURRENT_BINARY_DIR}/config.h @ONLY)

add_executable(${GAME_NAME}
  main.cc
)

target_link_libraries(${GAME_NAME}
  game0
  ${Boost_LIBRARIES}
)

install(
  TARGETS ${GAME_NAME}
  RUNTIME DESTINATION games
)

# examples

add_executable(example_animation
  example_animation.cc
)

target_link_libraries(example_animation
  game0
  ${Boost_LIBRARIES}
)

add_executable(example_particles
  example_particles.cc
)

target_link_libraries(example_particles
  game0
  ${Boost_LIBRARIES}
)
