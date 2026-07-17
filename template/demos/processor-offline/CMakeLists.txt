set(TARGET_NAME "ProcessorOfflineDemo")

switchboard_add_console_app(${TARGET_NAME} TARGETS SwitchboardExampleDSP)

target_sources(${TARGET_NAME} PRIVATE
    main.cpp
)

install(
    FILES ProcessorOfflineDemo.json input.wav
    DESTINATION ${CMAKE_INSTALL_BINDIR}
)
