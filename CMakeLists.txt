cmake_minimum_required(VERSION 3.16)

set(LANGUAGES CXX)
project(
  CPP_TEMPLATE
  VERSION 1.0
  DESCRIPTION "A C++ template"
  LANGUAGES ${LANGUAGES})

set(CMAKE_EXPORT_COMPILE_COMMANDS on)

if(${CMAKE_VERSION} VERSION_LESS "3.25.2")
  message(
    STATUS
      "CMake version is less than 3.25.2, upgrade to silence unnecessary warnings"
  )
endif()

# ##############################################################################
# Dependencies fetching
include(FetchContent)

# CROW (api)
FetchContent_Declare(
  Crow
  GIT_REPOSITORY https://github.com/CrowCpp/Crow
  GIT_TAG v1.2.0
  SYSTEM)

# Ctest
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG f8d7d77c06936315286eb55f8de22cd23c188571
  SYSTEM)
FetchContent_MakeAvailable(Crow googletest)

# # Raylib
set(RAYLIB_VERSION 5.5)
find_package(raylib ${RAYLIB_VERSION} QUIET)
if(NOT raylib_FOUND)
  message(STATUS "Raylib (C) not found, downloading latest
version")

  FetchContent_Declare(
    raylib
    DOWNLOAD_EXTRACT_TIMESTAMP OFF
    URL https://github.com/raysan5/raylib/archive/refs/tags/${RAYLIB_VERSION}.tar.gz
        SYSTEM)

  FetchContent_MakeAvailable(raylib)

endif()

# raylib-cpp
if(NOT raylib_cpp_FOUND)
  message(STATUS "Raylib_cpp (C++) not
found, downloading latest version")
  FetchContent_Declare(
    raylib_cpp
    GIT_REPOSITORY https://github.com/RobLoach/raylib-cpp.git
    GIT_TAG master
    GIT_SHALLOW 1
    SYSTEM)
  FetchContent_MakeAvailable(raylib_cpp)
endif()

# ##############################################################################
# Targets

set(PROJECT_APP ${PROJECT_NAME}_app)
set(PROJECT_LIB ${PROJECT_NAME}_lib)

add_library(${PROJECT_LIB} src/utils.cpp)
add_executable(${PROJECT_APP} src/main.cpp)

target_include_directories(${PROJECT_LIB} PUBLIC include/)

# ##############################################################################

add_library(common INTERFACE)
target_compile_features(common INTERFACE cxx_std_20)

# Optional IPO. Do not use IPO if it's not supported by compiler.
include(CheckIPOSupported)
check_ipo_supported(
  RESULT result
  OUTPUT output
  LANGUAGES ${LANGUAGES})
if(result)
  message(STATUS "IPO is supported")
  # TODO: verify if IPO should be enabled set_target_properties(${PROJECT_LIB}
  # PROPERTIES INTERPROCEDURAL_OPTIMIZATION TRUE)
else()
  message(WARNING "IPO is not supported")
endif()

target_compile_options(
  common
  INTERFACE
    # Preferred warnings
    $<$<CXX_COMPILER_ID:MSVC>:
    /Wall
    >
    $<$<CXX_COMPILER_ID:Clang>:-Weverything>
    $<$<CXX_COMPILER_ID:GCC>:-fconcepts-diagnostics-depth=3>
    $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:
    -Werror
    -Wall
    -Wextra
    -Wpedantic
    -Wformat=2
    -Wshadow
    -Wwrite-strings
    -Wcast-qual
    -Wcast-align
    -Wno-switch
    -Wconversion
    -Wno-ignored-optimization-argument
    >
    # Disable some warnings when using clang's Weverything
    $<$<CXX_COMPILER_ID:Clang>:
    -Wno-c++98-compat
    -Wno-c++98-compat-pedantic
    -Wno-c++20-compat
    -Wno-c++20-extensions
    -Wno-c99-extensions
    -Wno-zero-as-null-pointer-constant
    -Wno-padded
    -Wno-global-constructors
    -Wno-exit-time-destructors # Temporarily removed due to bug
                               # (https://github.com/llvm/llvm-project/issues/59572)
    -Wno-unused-macros
    >
    $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-fsanitize=address,undefined> # #
    $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-fsanitize-address-use-after-scope>
    $<$<CONFIG:DEBUG>:
    $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:GNU>>:
    -ggdb3
    -O0
    >
    $<$<CXX_COMPILER_ID:MSVC>:
    /GL
    /LTCG
    /O3>
    >
    $<$<CONFIG:RELEASE>:
    $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:GNU>>:
    -O3
    >
    >)

target_link_options(
  common INTERFACE
  $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-fsanitize=address,undefined>)

# Threads needed in gcc
set(THREADS_HAVE_PTHREAD_FLAG ON)
find_package(Threads REQUIRED QUIET)
target_link_libraries(${PROJECT_LIB} PUBLIC Threads::Threads)

# config
target_link_libraries(${PROJECT_LIB} PUBLIC common)

# ##############################################################################
# libraries linking

# SpdLog
find_package(spdlog QUIET)
if(spdlog_FOUND)
  target_link_libraries(${PROJECT_LIB} PUBLIC spdlog)
else()
  # Only use this if spdlog is not installed on your system
  message(STATUS "SpdLog not found, using bundled version")
  target_include_directories(${PROJECT_LIB} SYSTEM
                             PUBLIC ${CMAKE_SOURCE_DIR}/libs/spdlog/)
endif()
target_compile_definitions(${PROJECT_LIB} PUBLIC SPDLOG_USE_STD_FORMAT)
#
# link fetched libraries
target_link_libraries(${PROJECT_LIB} PUBLIC raylib raylib_cpp Crow::Crow common)

# ##############################################################################
# link to executables
target_link_libraries(${PROJECT_APP} PRIVATE ${PROJECT_LIB})

# ##############################################################################
# GTEST setup

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt
    ON
    CACHE BOOL "" FORCE)

if(${PROJECT_NAME} STREQUAL ${CMAKE_PROJECT_NAME})
  option(PACKAGE_TESTS "Build
the tests" ON)
endif()

if(${PACKAGE_TESTS})
  enable_testing()
  include(GoogleTest)
  add_subdirectory(tests)
endif()

# ##############################################################################
# Set the asset path macro to the absolute path on the dev machine
# target_compile_definitions( ${PROJECT_APP} PUBLIC
# ASSETS_PATH="${CMAKE_CURRENT_SOURCE_DIR}/assets/")
