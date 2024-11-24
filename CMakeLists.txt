cmake_minimum_required(VERSION 3.14)
project(foxogram_lib
        VERSION 1.0.0
        DESCRIPTION "Library for foxogram"
        LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(USE_OPEN_SSL "ON")
set(USE_TLS "ON")
set(USE_ZLIB "OFF")

include(cmake/utils.cmake)
include(GNUInstallDirs)

string(COMPARE EQUAL "${CMAKE_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}" is_top_level)

option(FOXOGRAM_LIB_BUILD_TESTS "Build foxogram_lib tests" OFF)
option(FOXOGRAM_LIB_BUILD_EXAMPLES "Build foxogram_lib examples" OFF)
option(FOXOGRAM_LIB_BUILD_DOCS "Build foxogram_lib documentation" OFF)
option(FOXOGRAM_LIB_INSTALL "Generate target for installing foxogram_lib" ${is_top_level})
set_if_undefined(FOXOGRAM_LIB_INSTALL_CMAKEDIR "${CMAKE_INSTALL_LIBDIR}/cmake/foxogram_lib" CACHE STRING
        "Install path for foxogram_lib package-related CMake files")

if(DEFINED FOXOGRAM_LIB_SHARED_LIBS)
    set(BUILD_SHARED_LIBS ${FOXOGRAM_LIB_SHARED_LIBS})
endif()

if(NOT DEFINED CMAKE_BUILD_TYPE AND NOT DEFINED CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

set_if_undefined(CMAKE_CXX_VISIBILITY_PRESET hidden)
set_if_undefined(CMAKE_VISIBILITY_INLINES_HIDDEN ON)

add_library(foxogram_lib)
add_library(foxogram_lib::foxogram_lib ALIAS foxogram_lib)

include(FetchContent)
FetchContent_Declare(nlohmann_json GIT_REPOSITORY https://github.com/nlohmann/json.git
        GIT_TAG bc889afb4c5bf1c0d8ee29ef35eaaf4c8bef8a5d)
FetchContent_MakeAvailable(nlohmann_json)
target_link_libraries(foxogram_lib PUBLIC nlohmann_json)

FetchContent_Declare(ixwebsocket GIT_REPOSITORY https://github.com/machinezone/IXWebSocket.git
        GIT_TAG 688af99747bc109d3476cb3a6703c5e7d37b10d9)
FetchContent_MakeAvailable(ixwebsocket)
target_link_libraries(foxogram_lib PUBLIC ixwebsocket)

set_target_properties(ixwebsocket PROPERTIES PUBLIC_HEADER "")

include(GenerateExportHeader)
set(export_file_name "export_shared.h")

if(NOT BUILD_SHARED_LIBS)
    set(export_file_name "export_static.h")
endif()

generate_export_header(foxogram_lib EXPORT_FILE_NAME include/foxogram_lib/${export_file_name})

set(sources
        include/foxogram/export.h
        include/foxogram/exceptions.h
        include/foxogram/Enums.h
        include/foxogram/HttpClient.h
        include/foxogram/User.h
        include/foxogram/Me.h
        include/foxogram/Message.h
        include/foxogram/Channel.h
        include/foxogram/Member.h
        src/HttpClient.cpp
        src/User.cpp
        src/Me.cpp
        src/Message.cpp
        src/Channel.cpp
        src/Member.cpp
)
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${sources})

include(CMakePackageConfigHelpers)

target_sources(foxogram_lib PRIVATE ${sources})
target_compile_definitions(foxogram_lib PUBLIC "$<$<NOT:$<BOOL:${BUILD_SHARED_LIBS}>>:FOXOGRAM_LIB_STATIC_DEFINE>")

target_include_directories(foxogram_lib
        PUBLIC
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>")

set_target_properties(foxogram_lib PROPERTIES
        SOVERSION ${PROJECT_VERSION_MAJOR}
        VERSION ${PROJECT_VERSION})

if(FOXOGRAM_LIB_INSTALL AND NOT CMAKE_SKIP_INSTALL_RULES)
    configure_package_config_file(cmake/foxogram_lib-config.cmake.in foxogram_lib-config.cmake
            INSTALL_DESTINATION "${FOXOGRAM_LIB_INSTALL_CMAKEDIR}")

    write_basic_package_version_file(foxogram_lib-config-version.cmake
            COMPATIBILITY SameMajorVersion)
    install(TARGETS foxogram_lib nlohmann_json ixwebsocket
            EXPORT foxogram_lib_export
            RUNTIME COMPONENT foxogram_lib
            LIBRARY COMPONENT foxogram_lib NAMELINK_COMPONENT foxogram_lib-dev
            ARCHIVE COMPONENT foxogram_lib-dev
            INCLUDES DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}")

    install(DIRECTORY include/
            TYPE INCLUDE
            COMPONENT foxogram_lib-dev)
    install(FILES "${CMAKE_CURRENT_BINARY_DIR}/include/foxogram_lib/${export_file_name}"
            COMPONENT foxogram_lib-dev
            DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/foxogram_lib")

    set(targets_file "foxogram_lib-shared-targets.cmake")

    if(NOT BUILD_SHARED_LIBS)
        set(targets_file "foxogram_lib-static-targets.cmake")
    endif()

    install(EXPORT foxogram_lib_export
            COMPONENT foxogram_lib-dev
            FILE "${targets_file}"
            DESTINATION "${FOXOGRAM_LIB_INSTALL_CMAKEDIR}"
            NAMESPACE foxogram_lib::)

    install(FILES
            "${CMAKE_CURRENT_BINARY_DIR}/foxogram_lib-config.cmake"
            "${CMAKE_CURRENT_BINARY_DIR}/foxogram_lib-config-version.cmake"
            COMPONENT foxogram_lib-dev
            DESTINATION "${FOXOGRAM_LIB_INSTALL_CMAKEDIR}")

    if(MSVC)
        set(pdb_file "")
        set(pdb_file_destination "")

        if(BUILD_SHARED_LIBS)
            set(pdb_file "$<TARGET_PDB_FILE:foxogram_lib>")
            set(pdb_file_destination "${CMAKE_INSTALL_BINDIR}")
        else()
            set(pdb_file "$<TARGET_FILE_DIR:foxogram_lib>/$<TARGET_FILE_PREFIX:foxogram_lib>$<TARGET_FILE_BASE_NAME:foxogram_lib>.pdb")
            set(pdb_file_destination "${CMAKE_INSTALL_LIBDIR}")
        endif()

        install(FILES "${pdb_file}"
                COMPONENT foxogram_lib-dev
                CONFIGURATIONS Debug RelWithDebInfo
                DESTINATION "${pdb_file_destination}"
                OPTIONAL)
    endif()
endif()

if(FOXOGRAM_LIB_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

if(FOXOGRAM_LIB_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

if(FOXOGRAM_LIB_BUILD_DOCS)
    find_package(Doxygen REQUIRED)
    doxygen_add_docs(docs include)
endif()
