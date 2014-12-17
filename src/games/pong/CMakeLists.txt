add_executable(pong
  pong.cc
  local/Ball.cc
  local/Ground.cc
  local/Racket.cc
  local/Rules.cc
)

target_link_libraries(pong
  gamebase0
  gamegraphics0
  gamemodel0
)

install(
  TARGETS pong
  RUNTIME DESTINATION games
)
