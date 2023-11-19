
add_executable(client
  chatclient.cpp
  ChatWindow.cpp
)

set_target_properties(client
                      PROPERTIES
                      LINKER_LANGUAGE CXX
                      CXX_STANDARD 17
                      PREFIX ""
)

find_package(Curses REQUIRED)
find_package(Threads REQUIRED)

target_link_libraries(client
  networking
  ${CURSES_LIBRARIES}
  ${CMAKE_THREAD_LIBS_INIT}
)

install(TARGETS client
  RUNTIME DESTINATION bin
)

