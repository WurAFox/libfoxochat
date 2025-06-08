cmake_minimum_required(VERSION 3.14)
project(libfoxochat
        VERSION 0.2.0
        DESCRIPTION "Library for foxochat"
        LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
option(LIBFOXOCHAT_USE_TLS "Build libfoxochat with tls support" ON)
if (LIBFOXOCHAT_USE_TLS)
    set(USE_TLS ON)
    set(USE_OPEN_SSL ON)
    find_package(OpenSSL REQUIRED)
endif ()
set(USE_ZLIB "OFF")
set(BUILD_SHARED_LIBS "OFF")

include(cmake/utils.cmake)
include(GNUInstallDirs)

string(COMPARE EQUAL "${CMAKE_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}" is_top_level)

option(LIBFOXOCHAT_BUILD_TESTS "Build libfoxochat tests" OFF)
option(LIBFOXOCHAT_BUILD_EXAMPLES "Build libfoxochat examples" OFF)
option(LIBFOXOCHAT_BUILD_DOCS "Build libfoxochat documentation" OFF)
OPTION(LIBFOXOCHAT_INSTALL "Install libfoxochat" ON)
set_if_undefined(LIBFOXOCHAT_INSTALL_CMAKEDIR "${CMAKE_INSTALL_LIBDIR}/cmake/libfoxochat" CACHE STRING
        "Install path for libfoxochat package-related CMake files")
option(LIBFOXOCHAT_USE_DEV "Use foxochat dev api" OFF)
set(LIBFOXOCHAT_BASE_URL ".foxochat.app")

if(LIBFOXOCHAT_USE_DEV)
    set(LIBFOXOCHAT_BASE_URL "-dev.${LIBFOXOCHAT_BASE_URL}")
endif()

if(DEFINED LIBFOXOCHAT_SHARED_LIBS)
    set(BUILD_SHARED_LIBS ${LIBFOXOCHAT_SHARED_LIBS})
endif()

if(NOT DEFINED CMAKE_BUILD_TYPE AND NOT DEFINED CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

set_if_undefined(CMAKE_CXX_VISIBILITY_PRESET hidden)
set_if_undefined(CMAKE_VISIBILITY_INLINES_HIDDEN ON)

add_library(libfoxochat)
add_library(libfoxochat::libfoxochat ALIAS libfoxochat)
target_compile_definitions(libfoxochat PUBLIC FOXOCHAT_URL="${LIBFOXOCHAT_BASE_URL}")

include(FetchContent)
FetchContent_Declare(ixwebsocket GIT_REPOSITORY https://github.com/machinezone/IXWebSocket.git
        GIT_TAG 688af99747bc109d3476cb3a6703c5e7d37b10d9)
FetchContent_MakeAvailable(ixwebsocket)
target_link_libraries(libfoxochat PUBLIC ixwebsocket)

set_target_properties(ixwebsocket PROPERTIES PUBLIC_HEADER "")

include(GenerateExportHeader)
set(export_file_name "export_shared.h")

if(NOT BUILD_SHARED_LIBS)
    set(export_file_name "export_static.h")
endif()

generate_export_header(libfoxochat EXPORT_FILE_NAME include/libfoxochat/${export_file_name})

set(sources
        include/foxochat/Utils.h
        include/foxochat/Cache.h
        include/foxochat/Logger.h
        include/nlohmann/json.hpp
        include/nlohmann/json_fwd.hpp
        include/foxochat/export.h
        include/foxochat/exceptions.h
        include/foxochat/Enums.h
        include/foxochat/HttpClient.h
        include/foxochat/User.h
        include/foxochat/Me.h
        include/foxochat/Message.h
        include/foxochat/Channel.h
        include/foxochat/Member.h
        include/foxochat/Gateway.h
        include/foxochat/EventTypes.h
        include/foxochat/Events.h
        include/foxochat/Attachment.h
        include/foxochat/Serializer.h
        src/Utils.cpp
        src/Logger.cpp
        src/HttpClient.cpp
        src/User.cpp
        src/Me.cpp
        src/Message.cpp
        src/Channel.cpp
        src/Member.cpp
        src/EventTypes.cpp
        src/Gateway.cpp
        src/Events/ChannelCreate.cpp
        src/Events/ChannelDelete.cpp
        src/Events/ChannelUpdate.cpp
        src/Events/Hello.cpp
        src/Events/MemberAdd.cpp
        src/Events/MemberRemove.cpp
        src/Events/MemberUpdate.cpp
        src/Events/MessageCreate.cpp
        src/Events/MessageDelete.cpp
        src/Events/MessageUpdate.cpp
        src/Events/Pong.cpp
        src/Attachment.cpp
        src/Serializer.cpp
)
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${sources})

include(CMakePackageConfigHelpers)

target_sources(libfoxochat PRIVATE ${sources})
target_compile_definitions(libfoxochat PUBLIC "$<$<NOT:$<BOOL:${BUILD_SHARED_LIBS}>>:LIBFOXOCHAT_STATIC_DEFINE>")

target_include_directories(libfoxochat
        PUBLIC
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>"
)
set_target_properties(libfoxochat PROPERTIES
        SOVERSION ${PROJECT_VERSION_MAJOR}
        VERSION ${PROJECT_VERSION})
configure_package_config_file(cmake/libfoxochat-config.cmake.in libfoxochat-config.cmake
        INSTALL_DESTINATION "${LIBFOXOCHAT_INSTALL_CMAKEDIR}")
write_basic_package_version_file(libfoxochat-config-version.cmake
        COMPATIBILITY SameMajorVersion)
if (LIBFOXOCHAT_INSTALL)
    install(TARGETS libfoxochat ixwebsocket
            EXPORT libfoxochat_export
            RUNTIME COMPONENT libfoxochat
            LIBRARY COMPONENT libfoxochat NAMELINK_COMPONENT libfoxochat-dev
            ARCHIVE COMPONENT libfoxochat-dev
            INCLUDES DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}")
    install(DIRECTORY include/
            TYPE INCLUDE
            COMPONENT libfoxochat-dev)
    install(FILES "${CMAKE_CURRENT_BINARY_DIR}/include/libfoxochat/${export_file_name}"
            COMPONENT libfoxochat-dev
            DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/libfoxochat")
    set(targets_file "libfoxochat-shared-targets.cmake")
    if(NOT BUILD_SHARED_LIBS)
        set(targets_file "libfoxochat-static-targets.cmake")
    endif()

    install(EXPORT libfoxochat_export
            COMPONENT libfoxochat-dev
            FILE "${targets_file}"
            DESTINATION "${LIBFOXOCHAT_INSTALL_CMAKEDIR}"
            NAMESPACE libfoxochat::)

    install(FILES
            "${CMAKE_CURRENT_BINARY_DIR}/libfoxochat-config.cmake"
            "${CMAKE_CURRENT_BINARY_DIR}/libfoxochat-config-version.cmake"
            COMPONENT libfoxochat-dev
            DESTINATION "${LIBFOXOCHAT_INSTALL_CMAKEDIR}")
    set_if_undefined(OpenSSL_INSTALL_DIR ${CMAKE_INSTALL_BINDIR})
    if (WIN32)
        if(OPENSSL_VERSION VERSION_GREATER_EQUAL "3.0.0")
            set(openssldlls "$<TARGET_PROPERTY:OpenSSL::SSL,INTERFACE_INCLUDE_DIRECTORIES>/../libssl-3-x64.dll;$<TARGET_PROPERTY:OpenSSL::SSL,INTERFACE_INCLUDE_DIRECTORIES>/../libcrypto-3-x64.dll")
        else ()
            set(openssldlls "$<TARGET_PROPERTY:OpenSSL::SSL,INTERFACE_INCLUDE_DIRECTORIES>/../libssl-1_1-x64.dll;$<TARGET_PROPERTY:OpenSSL::SSL,INTERFACE_INCLUDE_DIRECTORIES>/../libcrypto-1_1-x64.dll")
        endif ()
    elseif(APPLE)
        set(openssldlls ${OPENSSL_LIBRARIES})
    elseif(UNIX)
        set(openssldlls ${OPENSSL_LIBRARIES})
    endif ()

    install(FILES ${openssldlls}
            DESTINATION "${OpenSSL_INSTALL_DIR}")

    if(MSVC)
        set(pdb_file "")
        set(pdb_file_destination "")
        if(BUILD_SHARED_LIBS)
            set(pdb_file "$<TARGET_PDB_FILE:libfoxochat>")
            set(pdb_file_destination "${CMAKE_INSTALL_BINDIR}")
        else()
            set(pdb_file "$<TARGET_FILE_DIR:libfoxochat>/$<TARGET_FILE_PREFIX:libfoxochat>$<TARGET_FILE_BASE_NAME:libfoxochat>.pdb")
            set(pdb_file_destination "${CMAKE_INSTALL_LIBDIR}")
        endif()

        install(FILES "${pdb_file}"
                COMPONENT libfoxochat-dev
                CONFIGURATIONS Debug RelWithDebInfo
                DESTINATION "${pdb_file_destination}"
                OPTIONAL)
    endif()
endif()
if(LIBFOXOCHAT_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

if(LIBFOXOCHAT_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

if(LIBFOXOCHAT_BUILD_DOCS)
    find_package(Doxygen REQUIRED)
    doxygen_add_docs(docs include)
endif()
