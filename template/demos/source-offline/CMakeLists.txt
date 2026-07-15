set(TARGET_NAME "SourceOfflineDemo")

switchboard_add_console_app(${TARGET_NAME} TARGETS SwitchboardExampleDSP)

target_sources(${TARGET_NAME} PRIVATE
    main.cpp
)

install(
    FILES SourceOfflineDemo.json
    DESTINATION ${CMAKE_INSTALL_BINDIR}
)
