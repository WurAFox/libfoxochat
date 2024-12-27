cmake_minimum_required(VERSION 3.14)
project(libfoxogram
        VERSION 1.0.0
        DESCRIPTION "Library for foxogram"
        LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
option(LIBFOXOGRAM_USE_TLS "Build libfoxogram with tls support" ON)
if (LIBFOXOGRAM_USE_TLS)
    set(USE_TLS ON)
    set(USE_OPEN_SSL ON)
endif ()
set(USE_ZLIB "OFF")
set(BUILD_SHARED_LIBS "OFF")

include(cmake/utils.cmake)
include(GNUInstallDirs)

string(COMPARE EQUAL "${CMAKE_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}" is_top_level)

option(LIBFOXOGRAM_BUILD_TESTS "Build libfoxogram tests" OFF)
option(LIBFOXOGRAM_BUILD_EXAMPLES "Build libfoxogram examples" OFF)
option(LIBFOXOGRAM_BUILD_DOCS "Build libfoxogram documentation" OFF)
set(LIBFOXOGRAM_INSTALL ON)
set_if_undefined(LIBFOXOGRAM_INSTALL_CMAKEDIR "${CMAKE_INSTALL_LIBDIR}/cmake/libfoxogram" CACHE STRING
        "Install path for libfoxogram package-related CMake files")
option(LIBFOXOGRAM_USE_DEV "Use foxogram dev api" OFF)
set(LIBFOXOGRAM_BASE_URL "foxogram.su")
set(LIBFOXOGRAM_API_VERSION "v1")

if(LIBFOXOGRAM_USE_DEV)
    set(LIBFOXOGRAM_BASE_URL "dev-${FOXOGRAM_BASE_API_URL}")
endif()

if(DEFINED LIBFOXOGRAM_SHARED_LIBS)
    set(BUILD_SHARED_LIBS ${LIBFOXOGRAM_SHARED_LIBS})
endif()

if(NOT DEFINED CMAKE_BUILD_TYPE AND NOT DEFINED CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

set_if_undefined(CMAKE_CXX_VISIBILITY_PRESET hidden)
set_if_undefined(CMAKE_VISIBILITY_INLINES_HIDDEN ON)

add_library(libfoxogram)
add_library(libfoxogram::libfoxogram ALIAS libfoxogram)
target_compile_definitions(libfoxogram PUBLIC FOXOGRAM_URL="${LIBFOXOGRAM_BASE_URL}" FOXOGRAM_API_VERSION="/${LIBFOXOGRAM_API_VERSION}")

include(FetchContent)
FetchContent_Declare(ixwebsocket GIT_REPOSITORY https://github.com/machinezone/IXWebSocket.git
        GIT_TAG 688af99747bc109d3476cb3a6703c5e7d37b10d9)
FetchContent_MakeAvailable(ixwebsocket)
target_link_libraries(libfoxogram PUBLIC ixwebsocket)

set_target_properties(ixwebsocket PROPERTIES PUBLIC_HEADER "")

include(GenerateExportHeader)
set(export_file_name "export_shared.h")

if(NOT BUILD_SHARED_LIBS)
    set(export_file_name "export_static.h")
endif()

generate_export_header(libfoxogram EXPORT_FILE_NAME include/libfoxogram/${export_file_name})

set(sources
        include/foxogram/Cache.h
        include/foxogram/Logger.h
        include/nlohmann/json.hpp
        include/nlohmann/json_fwd.hpp
        include/foxogram/export.h
        include/foxogram/exceptions.h
        include/foxogram/Enums.h
        include/foxogram/HttpClient.h
        include/foxogram/User.h
        include/foxogram/Me.h
        include/foxogram/Message.h
        include/foxogram/Channel.h
        include/foxogram/Member.h
        src/Logger.cpp
        src/HttpClient.cpp
        src/User.cpp
        src/Me.cpp
        src/Message.cpp
        src/Channel.cpp
        src/Member.cpp
)
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${sources})

include(CMakePackageConfigHelpers)

target_sources(libfoxogram PRIVATE ${sources})
target_compile_definitions(libfoxogram PUBLIC "$<$<NOT:$<BOOL:${BUILD_SHARED_LIBS}>>:LIBFOXOGRAM_STATIC_DEFINE>")

target_include_directories(libfoxogram
        PUBLIC
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>"
)
set_target_properties(libfoxogram PROPERTIES
        SOVERSION ${PROJECT_VERSION_MAJOR}
        VERSION ${PROJECT_VERSION})
configure_package_config_file(cmake/libfoxogram-config.cmake.in libfoxogram-config.cmake
        INSTALL_DESTINATION "${LIBFOXOGRAM_INSTALL_CMAKEDIR}")
write_basic_package_version_file(libfoxogram-config-version.cmake
        COMPATIBILITY SameMajorVersion)
install(TARGETS libfoxogram ixwebsocket
        EXPORT libfoxogram_export
        RUNTIME COMPONENT libfoxogram
        LIBRARY COMPONENT libfoxogram NAMELINK_COMPONENT libfoxogram-dev
        ARCHIVE COMPONENT libfoxogram-dev
        INCLUDES DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}")
install(DIRECTORY include/
        TYPE INCLUDE
        COMPONENT libfoxogram-dev)
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/include/libfoxogram/${export_file_name}"
        COMPONENT libfoxogram-dev
        DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/libfoxogram")
set(targets_file "libfoxogram-shared-targets.cmake")
if(NOT BUILD_SHARED_LIBS)
    set(targets_file "libfoxogram-static-targets.cmake")
endif()

install(EXPORT libfoxogram_export
        COMPONENT libfoxogram-dev
        FILE "${targets_file}"
        DESTINATION "${LIBFOXOGRAM_INSTALL_CMAKEDIR}"
        NAMESPACE libfoxogram::)

install(FILES
        "${CMAKE_CURRENT_BINARY_DIR}/libfoxogram-config.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/libfoxogram-config-version.cmake"
        COMPONENT libfoxogram-dev
        DESTINATION "${LIBFOXOGRAM_INSTALL_CMAKEDIR}")
if(MSVC)
    set(pdb_file "")
    set(pdb_file_destination "")
    if(BUILD_SHARED_LIBS)
        set(pdb_file "$<TARGET_PDB_FILE:libfoxogram>")
        set(pdb_file_destination "${CMAKE_INSTALL_BINDIR}")
    else()
        set(pdb_file "$<TARGET_FILE_DIR:libfoxogram>/$<TARGET_FILE_PREFIX:libfoxogram>$<TARGET_FILE_BASE_NAME:libfoxogram>.pdb")
        set(pdb_file_destination "${CMAKE_INSTALL_LIBDIR}")
    endif()

    install(FILES "${pdb_file}"
            COMPONENT libfoxogram-dev
            CONFIGURATIONS Debug RelWithDebInfo
            DESTINATION "${pdb_file_destination}"
            OPTIONAL)
endif()

if(LIBFOXOGRAM_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

if(LIBFOXOGRAM_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

if(LIBFOXOGRAM_BUILD_DOCS)
    find_package(Doxygen REQUIRED)
    doxygen_add_docs(docs include)
endif()
