add_library(gameInstance
    src/GameInstance.cpp
)

target_include_directories(gameInstance
    PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

install(TARGETS gameInstance
    ARCHIVE DESTINATION lib
)

target_link_libraries(gameInstance
    gameConfigLoader
    gameRuleEngine
    gameCommunicator
    cpp-tree-sitter
    ruleExecute
)