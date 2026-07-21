add_subdirectory("source-realtime")
add_subdirectory("source-offline")
add_subdirectory("processor-realtime")
add_subdirectory("processor-offline")
# Dynamic loading (dlopen) is supported on macOS and desktop Linux only,
# matching the SDK's ExtensionLoader.
if(NOT CMAKE_SYSTEM_NAME MATCHES "Windows")
    add_subdirectory("processor-dynamic")
endif()
add_subdirectory("sink-realtime")
add_subdirectory("sink-offline")