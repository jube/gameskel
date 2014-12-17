add_executable(example_animation
  example_animation.cc
)

target_link_libraries(example_animation
  gamebase0
  gamegraphics0
  ${Boost_LIBRARIES}
)
