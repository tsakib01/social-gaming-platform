add_library(ruleNode
    src/RuleNode.cpp
)

target_include_directories(ruleNode
    PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(ruleNode
    cpp-tree-sitter
    rule
)

install(TARGETS ruleNode
    ARCHIVE DESTINATION lib
)