cmake_minimum_required(VERSION 3.14)
project(libfoxochat-tests)

include("../cmake/utils.cmake")
string(COMPARE EQUAL "${CMAKE_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}" is_top_level)

if(is_top_level)
    enable_testing()
endif()

include(FetchContent)
FetchContent_Declare(googletest URL https://github.com/google/googletest/archive/refs/tags/release-1.12.1.tar.gz)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
set(INSTALL_GTEST OFF)
set(BUILD_SHARED_LIBS OFF)
FetchContent_MakeAvailable(googletest)

if(is_top_level)
    find_package(libfoxochat REQUIRED)
endif()

set(sources
        main.cpp)
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${sources})

add_executable(libfoxochat-tests)
target_sources(libfoxochat-tests PRIVATE ${sources})

target_link_libraries(libfoxochat-tests
        PRIVATE
        libfoxochat::libfoxochat
        gtest_main
)
if(NOT is_top_level)
    win_copy_deps_to_target_dir(libfoxochat-tests libfoxochat::libfoxochat)
endif()

include(GoogleTest)
gtest_discover_tests(libfoxochat-tests)
