cmake_minimum_required(VERSION 3.24)
project(PortFloatingPoint LANGUAGES C)

set(ROOT_PORT_SOFTFLOAT_SOURCE_DIRECTORY "" CACHE PATH "Unmodified SoftFloat 3e source directory")
file(READ "${CMAKE_CURRENT_SOURCE_DIR}/softfloat_sources.json" source_manifest)
string(JSON source_count LENGTH "${source_manifest}" files)
math(EXPR last_source "${source_count} - 1")
set(arithmetic_sources "${CMAKE_CURRENT_SOURCE_DIR}/../NativeFloatingPoint.c")
foreach(index RANGE 0 ${last_source})
    string(JSON relative_path GET "${source_manifest}" files ${index} path)
    string(JSON expected_hash GET "${source_manifest}" files ${index} sha256)
    set(source_path "${ROOT_PORT_SOFTFLOAT_SOURCE_DIRECTORY}/${relative_path}")
    if(NOT EXISTS "${source_path}")
        message(FATAL_ERROR "Missing official SoftFloat 3e file: ${source_path}")
    endif()
    file(SHA256 "${source_path}" actual_hash)
    if(NOT actual_hash STREQUAL expected_hash)
        message(FATAL_ERROR "Official SoftFloat source seal failed: ${relative_path}")
    endif()
    if(relative_path MATCHES "\\.c$")
        list(APPEND arithmetic_sources "${source_path}")
    endif()
endforeach()

add_library(port_floating_point STATIC ${arithmetic_sources})
set_target_properties(port_floating_point PROPERTIES
    C_STANDARD 11 C_STANDARD_REQUIRED YES C_EXTENSIONS NO
    POSITION_INDEPENDENT_CODE YES)
add_custom_command(TARGET port_floating_point POST_BUILD
    COMMAND "${CMAKE_COMMAND}" -E copy_if_different
        "${ROOT_PORT_SOFTFLOAT_SOURCE_DIRECTORY}/../COPYING.txt"
        "${CMAKE_CURRENT_BINARY_DIR}/SoftFloat-COPYING.txt")
target_compile_definitions(port_floating_point PRIVATE
    SOFTFLOAT_FAST_INT64=1 THREAD_LOCAL=_Thread_local INLINE_LEVEL=0)
target_include_directories(port_floating_point PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/..")
target_include_directories(port_floating_point PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}"
    "${ROOT_PORT_SOFTFLOAT_SOURCE_DIRECTORY}/ARM-VFPv2"
    "${ROOT_PORT_SOFTFLOAT_SOURCE_DIRECTORY}/include")
if(EMSCRIPTEN)
    if(NOT CMAKE_SIZEOF_VOID_P EQUAL 4)
        message(FATAL_ERROR "The checked browser arithmetic target requires wasm32")
    endif()
    target_compile_options(port_floating_point PUBLIC -pthread)
    target_link_options(port_floating_point INTERFACE -pthread)
endif()
