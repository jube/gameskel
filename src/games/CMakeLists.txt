include_directories(${Boost_INCLUDE_DIRS})

include_directories(${CMAKE_CURRENT_BINARY_DIR})
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/config.h.in ${CMAKE_CURRENT_BINARY_DIR}/config.h @ONLY)


# examples

add_executable(example_animation
  example_animation.cc
)

target_link_libraries(example_animation
  gamebase0
  gamegraphics0
  ${Boost_LIBRARIES}
)

add_subdirectory(pong)
add_subdirectory(template)
