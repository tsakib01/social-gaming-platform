add_library(rule
    src/Rule.cpp
    src/BaseRule.cpp
    src/BodyRule.cpp
    src/MessageRule.cpp
)

target_include_directories(rule
    PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(rule
    cpp-tree-sitter
)

install(TARGETS rule
    ARCHIVE DESTINATION lib
)