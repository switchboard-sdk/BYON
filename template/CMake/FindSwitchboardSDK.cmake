# FindSwitchboardSDK.cmake - CMake find module

# Specify the components available in SwitchboardSDK
set(SwitchboardSDK_FOUND FALSE)

if(NOT DEFINED SWITCHBOARD_PACKAGE_VERSION)
    set(SWITCHBOARD_PACKAGE_VERSION "3.2.6") # Default version
endif()

# Detect platform (adjust as needed)
if(CMAKE_SYSTEM_NAME MATCHES "Linux")
    set(SwitchboardSDK_PLATFORM "linux")
elseif(CMAKE_SYSTEM_NAME MATCHES "Darwin")
    set(SwitchboardSDK_PLATFORM "macos")
elseif(CMAKE_SYSTEM_NAME MATCHES "Windows")
    set(SwitchboardSDK_PLATFORM "windows")
else()
    message(FATAL_ERROR "Unsupported platform: ${CMAKE_SYSTEM_NAME}")
endif()

# Normalize arch name to match SDK zip layout (Windows reports AMD64, zips use x86_64)
if(CMAKE_SYSTEM_PROCESSOR STREQUAL "AMD64")
    set(SwitchboardSDK_ARCH "x86_64")
else()
    set(SwitchboardSDK_ARCH "${CMAKE_SYSTEM_PROCESSOR}")
endif()

set(SwitchboardSDK_DIR "${CMAKE_BINARY_DIR}/SwitchboardSDK")

# Function to download and extract a zip package
function(download_and_extract url file_name output_dir)
    set(zip_file "${SwitchboardSDK_DIR}/Downloads/${file_name}.zip")

    # Ensure the Downloads directory exists
    file(MAKE_DIRECTORY "${SwitchboardSDK_DIR}/Downloads")

    # Download if the zip file does not exist
    if(NOT EXISTS ${zip_file})
        message(STATUS "Downloading ${file_name} from ${url}")
        file(DOWNLOAD ${url} ${zip_file}
                SHOW_PROGRESS
                STATUS download_status
                LOG download_log)
        list(GET download_status 0 status_code)
        list(GET download_status 1 status_string)
        if(NOT status_code EQUAL 0)
            message(FATAL_ERROR "Download failed with status ${status_code}: ${status_string}\nLog:\n${download_log}")
        endif()
    endif()

    # Ensure output directory exists and extract only if needed
    if(NOT EXISTS ${output_dir})
        file(MAKE_DIRECTORY ${output_dir})
        message(STATUS "Extracting ${zip_file} to ${output_dir}")
        execute_process(COMMAND ${CMAKE_COMMAND} -E tar xzf ${zip_file} WORKING_DIRECTORY ${output_dir})
    endif()

    # Ensure extraction was successful
    if(NOT EXISTS "${output_dir}")
        message(FATAL_ERROR "Failed to extract ${file_name}")
    endif()
endfunction()

# Function to find and set up a Switchboard package (SDK or extension)
function(find_switchboard_package PACKAGE_NAME PACKAGE_VERSION)

    # If target already exists, skip setup
    if (TARGET ${PACKAGE_NAME})
        set(${PACKAGE_NAME}_FOUND TRUE PARENT_SCOPE)
        return()
    endif ()

    # Public zips are named <package>-<platform>-<version>.
    set(SWITCHBOARD_PACKAGE_FILE_NAME "${PACKAGE_NAME}-${SwitchboardSDK_PLATFORM}-${PACKAGE_VERSION}")
    set(SWITCHBOARD_PACKAGE_URL "https://switchboard-sdk-public.s3.amazonaws.com/builds/release/${PACKAGE_VERSION}/${SwitchboardSDK_PLATFORM}/${SWITCHBOARD_PACKAGE_FILE_NAME}.zip")
    set(SWITCHBOARD_PACKAGE_DIR "${SwitchboardSDK_DIR}/libs/${PACKAGE_NAME}/${SwitchboardSDK_PLATFORM}/${PACKAGE_VERSION}")

    # Download and extract the package
    download_and_extract(${SWITCHBOARD_PACKAGE_URL} ${SWITCHBOARD_PACKAGE_FILE_NAME} ${SWITCHBOARD_PACKAGE_DIR})

    # Define package as an INTERFACE library
    add_library(${PACKAGE_NAME} SHARED IMPORTED)
    if(${SwitchboardSDK_PLATFORM} STREQUAL "macos")
        target_include_directories(${PACKAGE_NAME} INTERFACE ${SWITCHBOARD_PACKAGE_DIR}/Release/include)
    else()
        target_include_directories(${PACKAGE_NAME} INTERFACE ${SWITCHBOARD_PACKAGE_DIR}/${SwitchboardSDK_ARCH}/include)
    endif()
    if(${SwitchboardSDK_PLATFORM} STREQUAL "macos")
        set_target_properties(${PACKAGE_NAME} PROPERTIES
            IMPORTED_LOCATION "${SWITCHBOARD_PACKAGE_DIR}/Release/${PACKAGE_NAME}.xcframework"
        )
        # if macos-arm64_x86_64 directory exists, use it
        if(EXISTS "${SWITCHBOARD_PACKAGE_DIR}/Release/${PACKAGE_NAME}.xcframework/macos-arm64_x86_64")
            set(FRAMEWORK_PATH "${SWITCHBOARD_PACKAGE_DIR}/Release/${PACKAGE_NAME}.xcframework/macos-arm64_x86_64/${PACKAGE_NAME}.framework")
        else()
            set(FRAMEWORK_PATH "${SWITCHBOARD_PACKAGE_DIR}/Release/${PACKAGE_NAME}.xcframework/macos-arm64/${PACKAGE_NAME}.framework")
        endif()
        set(SwitchboardSDK_FRAMEWORK_PATHS ${SwitchboardSDK_FRAMEWORK_PATHS} ${FRAMEWORK_PATH} PARENT_SCOPE)
    elseif(${SwitchboardSDK_PLATFORM} STREQUAL "windows")
        set_target_properties(${PACKAGE_NAME} PROPERTIES
            IMPORTED_IMPLIB "${SWITCHBOARD_PACKAGE_DIR}/${SwitchboardSDK_ARCH}/lib/${PACKAGE_NAME}.lib"
            IMPORTED_LOCATION "${SWITCHBOARD_PACKAGE_DIR}/${SwitchboardSDK_ARCH}/bin/${PACKAGE_NAME}.dll"
        )
        list(APPEND SwitchboardSDK_PACKAGE_DIRECTORIES_RELEASE ${SWITCHBOARD_PACKAGE_DIR}/${SwitchboardSDK_ARCH}/bin PARENT_SCOPE)
    elseif(${SwitchboardSDK_PLATFORM} STREQUAL "linux")
        set_target_properties(${PACKAGE_NAME} PROPERTIES
            IMPORTED_LOCATION "${SWITCHBOARD_PACKAGE_DIR}/${SwitchboardSDK_ARCH}/lib/lib${PACKAGE_NAME}.so"
        )
    else ()
        message(FATAL_ERROR "Unsupported platform: ${CMAKE_SYSTEM_NAME}")
    endif()

    # Mark as found
    set(${PACKAGE_NAME}_FOUND TRUE PARENT_SCOPE)
endfunction()

find_switchboard_package("SwitchboardSDK" "${SWITCHBOARD_PACKAGE_VERSION}")
if (SwitchboardSDK_FOUND)
    set(SwitchboardSDK_LIBRARIES ${SwitchboardSDK_LIBRARIES} "SwitchboardSDK")
    set(SwitchboardSDK_PACKAGE_DIRECTORIES_RELEASE ${SwitchboardSDK_PACKAGE_DIRECTORIES_RELEASE} "${SwitchboardSDK_DIR}/libs/SwitchboardSDK/${SwitchboardSDK_PLATFORM}/${SWITCHBOARD_PACKAGE_VERSION}/${SwitchboardSDK_ARCH}/bin")
else()
    message(SEND_ERROR "Could not find SwitchboardSDK")
endif ()

# Check if required components are specified
set(_missing_components "")
foreach(_comp IN LISTS SwitchboardSDK_FIND_COMPONENTS)
    find_switchboard_package(${_comp} "${SWITCHBOARD_PACKAGE_VERSION}")
    if(${_comp}_FOUND)
        set(package_dir "${SwitchboardSDK_DIR}/libs/${_comp}/${SwitchboardSDK_PLATFORM}/${SWITCHBOARD_PACKAGE_VERSION}")
        set(SwitchboardSDK_PACKAGE_DIRECTORIES_RELEASE ${SwitchboardSDK_PACKAGE_DIRECTORIES_RELEASE} ${package_dir}/${SwitchboardSDK_ARCH}/bin)
        set(SwitchboardSDK_LIBRARIES ${SwitchboardSDK_LIBRARIES} ${_comp})
    else()
        list(APPEND _missing_components ${_comp})
    endif ()
endforeach()

# If any required component is missing, fail
if(_missing_components)
    set(SwitchboardSDK_FOUND FALSE)
    message(SEND_ERROR "Missing SwitchboardSDK components: ${_missing_components}")
else()
    set(SwitchboardSDK_FOUND TRUE)
endif()
