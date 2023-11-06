add_library(serverManager
    src/ServerManager.cpp)

target_include_directories(serverManager
    PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(serverManager
  networking
  userManager
  gameCommunicator
  gameInstanceManager
)

install(TARGETS serverManager
    ARCHIVE DESTINATION lib
)
