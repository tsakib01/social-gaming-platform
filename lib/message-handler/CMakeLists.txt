add_library(messageHandler
    src/MessageHandler.cpp)

target_include_directories(messageHandler
    PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

install(TARGETS messageHandler
    ARCHIVE DESTINATION lib
)