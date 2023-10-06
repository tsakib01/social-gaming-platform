add_executable(main main.cpp)

target_link_libraries(main gameConfigLoader gameRuleEngine)

install(TARGETS main
    RUNTIME DESTINATION bin
)