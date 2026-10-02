cmake_minimum_required(VERSION 3.24)
project(StaticArmWebAssembly LANGUAGES CXX)

if(NOT EMSCRIPTEN OR NOT CMAKE_SIZEOF_VOID_P EQUAL 4)
    message(FATAL_ERROR "This target requires an Emscripten wasm32 toolchain")
endif()

set(ROOT_PORT_AZAHAR_SOURCE_DIRECTORY "" CACHE PATH "Pinned public Azahar checkout")
set(ROOT_PORT_TRANSLATED_DIRECTORY "" CACHE PATH "Ignored generated declarations directory")
foreach(required_header IN ITEMS
        "${ROOT_PORT_AZAHAR_SOURCE_DIRECTORY}/src/core/arm/arm_interface.h"
        "${ROOT_PORT_AZAHAR_SOURCE_DIRECTORY}/externals/boost/boost/serialization/version.hpp"
        "${ROOT_PORT_AZAHAR_SOURCE_DIRECTORY}/externals/fmt/include/fmt/format.h"
        "${ROOT_PORT_TRANSLATED_DIRECTORY}/recomp.h")
    if(NOT EXISTS "${required_header}")
        message(FATAL_ERROR "Missing pinned header: ${required_header}")
    endif()
endforeach()

# Compile the real adapter without native binary loading. This archive does not
# resolve the platform implementation, generated functions or guest arithmetic.
add_library(static_arm_webassembly STATIC
    ../StaticArmBackend.cpp
    ../TranslatedFunctionModule.cpp
    ../NativeBlockSchedule.cpp
    ../GuestMemoryTrace.cpp)
target_compile_features(static_arm_webassembly PUBLIC cxx_std_20)
target_compile_definitions(static_arm_webassembly PRIVATE ROOT_PORT_STATIC_MODULE_ONLY=1)
target_compile_options(static_arm_webassembly PUBLIC
    -pthread -fwasm-exceptions -ffp-contract=off)
target_link_options(static_arm_webassembly INTERFACE -pthread -fwasm-exceptions)
target_include_directories(static_arm_webassembly PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/.."
    "${ROOT_PORT_TRANSLATED_DIRECTORY}")
target_include_directories(static_arm_webassembly SYSTEM PUBLIC
    "${ROOT_PORT_AZAHAR_SOURCE_DIRECTORY}/src"
    "${ROOT_PORT_AZAHAR_SOURCE_DIRECTORY}/externals/boost"
    "${ROOT_PORT_AZAHAR_SOURCE_DIRECTORY}/externals/fmt/include")
