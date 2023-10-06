add_library(gameRuleEngine
    src/GameRuleEngine.cpp
    src/RuleInterpreter.cpp
)

target_include_directories(gameRuleEngine
    PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(gameRuleEngine
    gameConfigLoader
)

install(TARGETS gameRuleEngine
    ARCHIVE DESTINATION lib
)