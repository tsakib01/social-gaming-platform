add_executable(server
    server.cpp
)

set_target_properties(server
                      PROPERTIES
                      LINKER_LANGUAGE CXX
                      CXX_STANDARD 17
                      PREFIX ""
)

find_package(Threads REQUIRED)

target_link_libraries(server
  userManager
  gameInstanceManager
  networking
  ${CMAKE_THREAD_LIBS_INIT}
)

install(TARGETS server
  RUNTIME DESTINATION bin
)