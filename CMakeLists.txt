cmake_minimum_required(VERSION 3.16)

project(
  CPP_TEMPLATE
  VERSION 1.0
  DESCRIPTION "A C++ template"
  LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS on)

if(${CMAKE_VERSION} VERSION_LESS "3.25.2")
  message(
    STATUS
      "CMake version is less than 3.25.2, upgrade to silence unnecesary warnings"
  )
endif()

# ##############################################################################
# Targets

add_executable(${PROJECT_NAME} src/main.cpp)

# ##############################################################################

target_compile_features(${PROJECT_NAME} INTERFACE cxx_std_20)

# add_library(common INTERFACE) target_compile_features(common INTERFACE
# cxx_std_20) # TODO: add iwty required - curently not working on windows
# find_program(iwyu_path NAMES include-what-you-use iwyu) set_target_properties(
# ${PROJECT_NAME} PROPERTIES CXX_INCLUDE_WHAT_YOU_USE ${iwyu_path}
# INTERPROCEDURAL_OPTIMIZATION TRUE)
#
# target_compile_options( common INTERFACE # Prefered warnings
# $<$<CXX_COMPILER_ID:MSVC>: /Wall /WX> $<$<CXX_COMPILER_ID:Clang>:-Weverything>
# $<$<CXX_COMPILER_ID:GCC>:-fconcepts-diagnostics-depth=3>
# $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:
# -Werror -Wall -Wextra -Wpedantic -Wformat=2 -Wshadow -Wwrite-strings
# -Wcast-qual -Wcast-align -Wno-switch -Wconversion> # Disable some warnings
# when using clang's Weverything $<$<CXX_COMPILER_ID:Clang>:-Wno-c++98-compat>
# $<$<CXX_COMPILER_ID:Clang>:-Wno-c++98-compat-pedantic>
# $<$<CXX_COMPILER_ID:Clang>:-Wno-c++20-compat>
# $<$<CXX_COMPILER_ID:Clang>:-Wno-c++20-extensions>
# $<$<CXX_COMPILER_ID:Clang>:-Wno-c99-extensions>
# $<$<CXX_COMPILER_ID:Clang>:-Wno-zero-as-null-pointer-constant>
# $<$<CXX_COMPILER_ID:Clang>:-Wno-error=padded> #
# $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-fsanitize=address,undefined> #
# $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-fsanitize-address-use-after-scope>
# $<$<CONFIG:DEBUG>:
# $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:GNU>>:
# -ggdb3 -O0 > $<$<CXX_COMPILER_ID:MSVC>: /GL /LTCG /O3>> $<$<CONFIG:RELEASE>:
# $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:GNU>>:
# -Ofast > # TODO: re-add this later # $<$<CXX_COMPILER_ID:MSVC>: /GL /LTCG /O3
# > >)
#
# target_link_options( common INTERFACE
# $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-fsanitize=address,undefined>)
#
# # Threads needed in gcc set(THREADS_HAVE_PTHREAD_FLAG ON) find_package(Threads
# REQUIRED) target_link_libraries(${PROJECT_NAME} PUBLIC Threads::Threads)
#
# # config target_link_libraries(${PROJECT_NAME} PRIVATE common)
#
# ##############################################################################

# SpdLog find_package(spdlog QUIET) if(spdlog_FOUND)
# target_link_libraries(${PROJECT_NAME} PUBLIC spdlog) else() # Only use this if
# spdlog is not installed on your system message(STATUS "SpdLog not found, using
# bundled version") target_include_directories(${PROJECT_NAME} SYSTEM PUBLIC
# ${CMAKE_SOURCE_DIR}/libs/spdlog/) endif()
# target_compile_definitions(${PROJECT_NAME} PRIVATE SPDLOG_USE_STD_FORMAT)

# Raylib
find_package(raylib QUIET)
if(NOT raylib_FOUND)
  message(STATUS "Raylib (C) not found, downloading latest version")
  include(FetchContent)
  FetchContent_Declare(
    raylib
    GIT_REPOSITORY https://github.com/raysan5/raylib.git
    GIT_TAG master
    GIT_SHALLOW 1
    SYSTEM)
  FetchContent_MakeAvailable(raylib)
endif()

# raylib-cpp
if(NOT raylib_cpp_FOUND)
  message(STATUS "Raylib_cpp (C++) not found, downloading latest version")
  include(FetchContent)
  FetchContent_Declare(
    raylib_cpp
    GIT_REPOSITORY https://github.com/RobLoach/raylib-cpp.git
    GIT_TAG master
    GIT_SHALLOW 1
    SYSTEM)
  FetchContent_MakeAvailable(raylib_cpp)
endif()

# Adding libraries to project
target_link_libraries(${PROJECT_NAME} PRIVATE raylib)
target_link_libraries(${PROJECT_NAME} PRIVATE raylib_cpp)

# #
# ##############################################################################
# # Ctest include(FetchContent) FetchContent_Declare( googletest GIT_REPOSITORY
# https://github.com/google/googletest.git # Always fetch the # # latest version
# GIT_TAG main SYSTEM)
#
# # For Windows: Prevent overriding the parent project's compiler/linker
# settings set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
# FetchContent_MakeAvailable(googletest)
#
# if(PROJECT_NAME STREQUAL CMAKE_PROJECT_NAME) option(PACKAGE_TESTS "Build the
# tests" ON) endif()
#
# if(PACKAGE_TESTS) enable_testing() include(GoogleTest) add_subdirectory(tests)
# endif()

# ##############################################################################
# Set the asset path macro to the absolute path on the dev machine
target_compile_definitions(
  ${PROJECT_NAME} PUBLIC ASSETS_PATH="${CMAKE_CURRENT_SOURCE_DIR}/assets/")
