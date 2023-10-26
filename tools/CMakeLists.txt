add_executable(main
    main.cpp
)

target_link_libraries(main 
    gameConfigLoader 
    gameInstance
    gameInstanceManager
    cpp-tree-sitter
)

install(TARGETS main
    RUNTIME DESTINATION bin
)

add_subdirectory(client-ncurses)
add_subdirectory(server)