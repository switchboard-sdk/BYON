set(TARGET_NAME "ProcessorRealtimeDemo")

switchboard_add_console_app(${TARGET_NAME} TARGETS SwitchboardExampleDSP)

target_sources(${TARGET_NAME} PRIVATE
    main.cpp
)

install(
    FILES ProcessorRealtimeDemo.json
    DESTINATION ${CMAKE_INSTALL_BINDIR}
)
