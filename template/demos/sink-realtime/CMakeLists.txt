set(TARGET_NAME "SinkRealtimeDemo")

switchboard_add_console_app(${TARGET_NAME} TARGETS SwitchboardExampleDSP)

target_sources(${TARGET_NAME} PRIVATE
    main.cpp
)

install(
    FILES SinkRealtimeDemo.json
    DESTINATION ${CMAKE_INSTALL_BINDIR}
)
