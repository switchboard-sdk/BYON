set(TARGET_NAME "ProcessorDynamicDemo")

# Note: unlike the other demos, this one does NOT link the extension
# (no TARGETS SwitchboardExampleDSP) — it loads the .so at runtime via dlopen.
switchboard_add_console_app(${TARGET_NAME})

target_sources(${TARGET_NAME} PRIVATE
    main.cpp
)

target_link_libraries(${TARGET_NAME} PRIVATE ${CMAKE_DL_LIBS})

install(
    FILES ProcessorDynamicDemo-Offline.json ProcessorDynamicDemo-Realtime.json input.wav
    DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# Ship the extension .so next to the demo so it can be dlopen'd with the
# default relative path.
install(
    FILES $<TARGET_FILE:SwitchboardExampleDSP>
    DESTINATION ${CMAKE_INSTALL_BINDIR}
)
