add_library(ruleExecute
    src/RuleExecute.cpp
    src/OutgoingMessages.cpp
)

target_include_directories(ruleExecute
    PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(ruleExecute
    expression
    gameConfigLoader
    userManager
)

install(TARGETS ruleExecute
    ARCHIVE DESTINATION lib
)