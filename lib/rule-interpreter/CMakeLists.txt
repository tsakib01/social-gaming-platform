add_library(ruleInterpreter
    src/RuleInterpreter.cpp
)

target_include_directories(ruleInterpreter
    PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(ruleInterpreter
    cpp-tree-sitter
    rule
    ruleNode
)

install(TARGETS ruleInterpreter
    ARCHIVE DESTINATION lib
)