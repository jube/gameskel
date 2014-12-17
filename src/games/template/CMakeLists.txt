add_executable(gametemplate
  main.cc
)

target_link_libraries(gametemplate
  gamebase0
  gamegraphics0
)

install(
  TARGETS gametemplate
  RUNTIME DESTINATION games
)
