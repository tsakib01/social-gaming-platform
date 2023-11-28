add_library(rule
    src/AssignmentRule.cpp
    src/BaseRule.cpp
    src/BodyRule.cpp
    src/DiscardRule.cpp
    src/ExtendRule.cpp
    src/ForRule.cpp
    src/InputRule.cpp
    src/MessageRule.cpp
    src/ParallelForRule.cpp
    src/Rule.cpp
    src/ScoresRule.cpp
    src/ReverseRule.cpp
)

target_include_directories(rule
    PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(rule
    ruleNode
    cpp-tree-sitter
)

install(TARGETS rule
    ARCHIVE DESTINATION lib
)