add_executable(main
    main.cpp
)

target_link_libraries(main 
    gameConfigLoader 
    gameInstance
    gameInstanceManager
)

install(TARGETS main
    RUNTIME DESTINATION bin
)