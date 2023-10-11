add_executable(main main.cpp)

target_link_libraries(main 
    gameConfigLoader 
    gameRuleEngine 
    gameInstance
    gameInstanceManager
)

install(TARGETS main
    RUNTIME DESTINATION bin
)