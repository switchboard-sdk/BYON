set(TARGET_NAME "SourceRealtimeDemo")

switchboard_add_console_app(${TARGET_NAME} TARGETS SwitchboardExampleDSP)

target_sources(${TARGET_NAME} PRIVATE
    main.cpp
)

install(
    FILES SourceRealtimeDemo.json
    DESTINATION ${CMAKE_INSTALL_BINDIR}
)
