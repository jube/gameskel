include_directories(${Boost_INCLUDE_DIRS})
include_directories(${SFML2_INCLUDE_DIRS})

set(LIBGAME0_SRC
  Animation.cc
  Entity.cc
  Particle.cc
  Random.cc
  Resource.cc
  World.cc
)

add_library(game0 SHARED
  ${LIBGAME0_SRC}
)

target_link_libraries(game0
  ${Boost_LIBRARIES}
  ${SFML2_LIBRARIES}
)

install(
  TARGETS game0
  LIBRARY DESTINATION lib
)
