add_library(gameCommunicator
    src/GameCommunicator.cpp)

target_include_directories(gameCommunicator
    PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

install(TARGETS gameCommunicator
    ARCHIVE DESTINATION lib
)

target_link_libraries(gameCommunicator
  networking
  userManager
)