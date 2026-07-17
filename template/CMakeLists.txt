cmake_minimum_required(VERSION 3.13...3.29 FATAL_ERROR)

project(SwitchboardExampleDSP VERSION 2.0.0 LANGUAGES CXX C)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/CMake")
include(SwitchboardSDK)
include(GNUInstallDirs)

set(SWITCHBOARD_PROJECT_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/../..")

set(TARGET_NAME "SwitchboardExampleDSP")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(${TARGET_NAME} SHARED "")

if(CMAKE_SYSTEM_NAME MATCHES "Windows")
    set_target_properties(${TARGET_NAME} PROPERTIES WINDOWS_EXPORT_ALL_SYMBOLS ON)
    add_definitions(-D_USE_MATH_DEFINES)
endif ()

target_include_directories(${TARGET_NAME} PUBLIC "include")

add_subdirectory("src" "SwitchboardExampleDSP")

# SWITCHBOARD SDK
find_package(SwitchboardSDK REQUIRED)
target_link_libraries(${TARGET_NAME} PRIVATE "SwitchboardSDK")

if (CMAKE_SYSTEM_NAME MATCHES "iOS|Darwin|watchOS")

    # Uncomment the following lines to enable Objective-C and Swift support
    # enable_language(OBJCXX)
    # enable_language(Swift)

    get_property(PUBLIC_FRAMEWORK_HEADERS_VAR TARGET ${TARGET_NAME} PROPERTY PUBLIC_FRAMEWORK_HEADERS)
    target_sources(${TARGET_NAME} PRIVATE "${PUBLIC_FRAMEWORK_HEADERS_VAR}")

    set_target_properties(${TARGET_NAME} PROPERTIES
        FRAMEWORK TRUE
        MACOSX_FRAMEWORK_IDENTIFIER "com.synervoz.${TARGET_NAME}"
        VERSION 1.0.0
        SOVERSION 1.0.0
        PUBLIC_HEADER "${PUBLIC_FRAMEWORK_HEADERS_VAR}"
    )

    set_target_properties(${TARGET_NAME} PROPERTIES
        XCODE_ATTRIBUTE_PRODUCT_BUNDLE_IDENTIFIER "com.synervoz.${TARGET_NAME}"
        XCODE_ATTRIBUTE_DEFINES_MODULE "NO"
        XCODE_ATTRIBUTE_PRODUCT_MODULE_NAME "${TARGET_NAME}"
        XCODE_ATTRIBUTE_BUILD_LIBRARY_FOR_DISTRIBUTION "YES"
    )
endif ()

install(
    TARGETS "SwitchboardExampleDSP"
    FRAMEWORK DESTINATION lib
)

# Tests
#add_catch_test_target(${TARGET_NAME})


# Demos
add_subdirectory("demos" "SwitchboardExampleDSPDemos")
