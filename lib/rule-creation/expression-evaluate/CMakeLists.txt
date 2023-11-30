add_library(expressionEvaluate
    src/ExpressionEvaluator.cpp
)

target_include_directories(expressionEvaluate
    PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(expressionEvaluate
    gameConfigLoader
    expression
)

install(TARGETS expressionEvaluate
    ARCHIVE DESTINATION lib
)
