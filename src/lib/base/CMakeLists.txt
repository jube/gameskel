include_directories(${Boost_INCLUDE_DIRS})

set(LIBGAMEBASE0_SRC
  Assets.cc
  Event.cc
  Log.cc
  Random.cc
)

add_library(gamebase0 SHARED
  ${LIBGAMEBASE0_SRC}
)

target_link_libraries(gamebase0
  ${Boost_LIBRARIES}
)

set_target_properties(gamebase0
  PROPERTIES
  VERSION ${CPACK_PACKAGE_VERSION}
  SOVERSION ${CPACK_PACKAGE_VERSION_MAJOR}
)

install(
  TARGETS gamebase0
  LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
  ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
)
