add_library(userManager
    src/UserManager.cpp)

target_include_directories(userManager
    PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

install(TARGETS userManager
    ARCHIVE DESTINATION lib
)

target_link_libraries(userManager
  networking
)