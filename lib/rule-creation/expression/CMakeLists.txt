add_library(expression INTERFACE)

target_include_directories(expression
    INTERFACE
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

target_link_libraries(expression
    INTERFACE
        gameConfigLoader
)

install(TARGETS expression
    ARCHIVE DESTINATION lib
)