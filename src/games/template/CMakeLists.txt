add_executable(game_template
  main.cc
)

target_link_libraries(game_template
  gamebase0
  gamegraphics0
)

install(
  TARGETS game_template
  RUNTIME DESTINATION games
)
