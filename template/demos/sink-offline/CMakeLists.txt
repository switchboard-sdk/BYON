set(TARGET_NAME "SinkOfflineDemo")

switchboard_add_console_app(${TARGET_NAME} TARGETS SwitchboardExampleDSP)

target_sources(${TARGET_NAME} PRIVATE
    main.cpp
)

install(
    FILES SinkOfflineDemo.json input.wav
    DESTINATION ${CMAKE_INSTALL_BINDIR}
)
