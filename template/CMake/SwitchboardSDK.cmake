#[=======================================================================[.rst:
SwitchboardSDK
--------------

This file contains functions for creating applications using the Switchboard SDK.

.. cmake:command:: switchboard_add_console_app

  .. code-block:: cmake

    switchboard_add_console_app(<target_name> [EXTENSIONS <extension1> <extension2> ...])

  Create a target for a console application that uses the Switchboard SDK.
  The target name is specified as the first argument. The optional ``EXTENSIONS`` argument
  specifies a list of extensions to be included in the application.
  The extensions are specified as a space-separated list of names. The extensions must be
  available in the Switchboard SDK package.

#]=======================================================================]

# TODO: Generate documentation for this file using the procedure described in https://gitlab.com/Pro1/doxygen-cmake-sphinx

function(switchboard_add_console_app)
    cmake_parse_arguments(ARGS "" "" "EXTENSIONS;TARGETS" ${ARGN})
    set(TARGET_NAME ${ARGV0})
    set(EXTENSIONS ${ARGS_EXTENSIONS})
    set(TARGETS ${ARGS_TARGETS})
    add_executable(${TARGET_NAME})

    find_package(SwitchboardSDK REQUIRED COMPONENTS ${EXTENSIONS})
    target_link_libraries(${TARGET_NAME} PRIVATE ${SwitchboardSDK_LIBRARIES})
    target_link_libraries(${TARGET_NAME} PRIVATE ${TARGETS})

    if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
        set(TARGET_FILE_DIRS "")
        foreach(target ${TARGETS})
            get_target_property(dir ${target} RUNTIME_OUTPUT_DIRECTORY)
            if(NOT dir)
            set(dir "$<TARGET_FILE_DIR:${target}>")
            endif()
            list(APPEND TARGET_FILE_DIRS "${dir}")
        endforeach()
        install(TARGETS ${TARGET_NAME}
                RUNTIME_DEPENDENCIES
                PRE_EXCLUDE_REGEXES "api-ms-" "ext-ms-" "nvcuda.dll" "cublas" "cudart"
                POST_EXCLUDE_REGEXES ".*system32/.*\\.dll"
                DIRECTORIES
                    ${SwitchboardSDK_PACKAGE_DIRECTORIES_RELEASE}
                    ${TARGET_FILE_DIRS}
        )
        foreach(library ${SwitchboardSDK_LIBRARIES})
            install(IMPORTED_RUNTIME_ARTIFACTS ${library})
        endforeach()
    endif()
    if(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
        # From Professional CMake 19th Edition, section 25.11:
        # A bug in CMake leads to a fatal error if an imported framework target is listed in XCODE_EMBED_FRAMEWORKS.
        # Until that bug is fixed, set XCODE_LINK_BUILD_PHASE_MODE to KNOWN_LOCATION, and list the full path to the
        # framework or XCFramework in XCODE_EMBED_FRAMEWORKS. Details of the issue can be found in the following bug report:
        set_target_properties(${TARGET_NAME} PROPERTIES
                XCODE_EMBED_FRAMEWORKS "${SwitchboardSDK_FRAMEWORK_PATHS}"
                XCODE_LINK_BUILD_PHASE_MODE KNOWN_LOCATION
                INSTALL_RPATH @executable_path/../lib
        )

        set_target_properties(${TARGET_NAME} PROPERTIES
                BUILD_RPATH "@executable_path/../lib"
        )

        install(TARGETS ${TARGET_NAME})
        foreach(library ${SwitchboardSDK_LIBRARIES})
            install(IMPORTED_RUNTIME_ARTIFACTS ${library})
        endforeach()
    endif()
    if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
        set_target_properties(${TARGET_NAME} PROPERTIES
                INSTALL_RPATH "$ORIGIN/../lib"
        )
        install(TARGETS ${TARGET_NAME})
        foreach(library ${SwitchboardSDK_LIBRARIES})
            install(IMPORTED_RUNTIME_ARTIFACTS ${library})
        endforeach()
    endif()
endfunction()
