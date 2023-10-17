add_library(gameInstanceManager
    src/GameInstanceManager.cpp)

target_include_directories(gameInstanceManager
    PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

install(TARGETS gameInstanceManager
    ARCHIVE DESTINATION lib
)

target_link_libraries(gameInstanceManager
    gameInstance
)