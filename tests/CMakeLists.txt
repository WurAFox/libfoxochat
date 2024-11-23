cmake_minimum_required(VERSION 3.14)
project(foxogram_lib-tests)

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

FetchContent_Declare(dotenv GIT_REPOSITORY https://github.com/laserpants/dotenv-cpp
        GIT_TAG master)
FetchContent_MakeAvailable(dotenv)


if(is_top_level)
    find_package(foxogram_lib REQUIRED)
endif()

set(sources
        main.cpp)
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${sources})

add_executable(foxogram_lib-tests)
target_sources(foxogram_lib-tests PRIVATE ${sources})

target_link_libraries(foxogram_lib-tests
        PRIVATE
        foxogram_lib::foxogram_lib
        gtest_main
        dotenv
)

if(NOT is_top_level)
    win_copy_deps_to_target_dir(foxogram_lib-tests foxogram_lib::foxogram_lib)
endif()

include(GoogleTest)
gtest_discover_tests(foxogram_lib-tests)
