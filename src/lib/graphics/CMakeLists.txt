include_directories(${Boost_INCLUDE_DIRS})
include_directories(${SFML2_INCLUDE_DIRS})

set(LIBGAMEGRAPHICS0_SRC
  Animation.cc
  Entity.cc
  Group.cc
  Resource.cc
)

add_library(gamegraphics0 SHARED
  ${LIBGAMEGRAPHICS0_SRC}
)

target_link_libraries(gamegraphics0
  ${Boost_LIBRARIES}
  ${SFML2_LIBRARIES}
)

set_target_properties(gamegraphics0
  PROPERTIES
  VERSION ${CPACK_PACKAGE_VERSION}
  SOVERSION ${CPACK_PACKAGE_VERSION_MAJOR}
)

install(
  TARGETS gamegraphics0
  LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
  ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
)
