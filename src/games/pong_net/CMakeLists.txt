include_directories(${SFML2_NETWORK_INCLUDE_DIRS})

set(GAME_PROTOCOL_SRC
  protocol/Request.cc
  protocol/Response.cc
)

add_executable(pong_client
  pong_client.cc
  client/ClientBall.cc
  client/ClientGround.cc
  client/ClientPoints.cc
  client/ClientRacket.cc
  ${GAME_PROTOCOL_SRC}
)

target_link_libraries(pong_client
  gamebase0
  gamegraphics0
  ${SFML2_NETWORK_LIBRARIES}
)

install(
  TARGETS pong_client
  RUNTIME DESTINATION games
)

add_executable(pong_server
  pong_server.cc
  server/ServerBall.cc
  server/ServerRacket.cc
  server/ServerRules.cc
  ${GAME_PROTOCOL_SRC}
)

target_link_libraries(pong_server
  gamebase0
  gamemodel0
  ${SFML2_NETWORK_LIBRARIES}
)

install(
  TARGETS pong_server
  RUNTIME DESTINATION games
)
