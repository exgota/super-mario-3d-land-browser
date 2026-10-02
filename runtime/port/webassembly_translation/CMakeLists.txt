cmake_minimum_required(VERSION 3.24)
project(StaticArmTranslation LANGUAGES C CXX)

if(NOT EMSCRIPTEN OR NOT CMAKE_SIZEOF_VOID_P EQUAL 4)
    message(FATAL_ERROR "Translated browser entries require Emscripten wasm32")
endif()
set(ROOT_PORT_TRANSLATED_DIRECTORY "" CACHE PATH "Sealed ignored timed translation directory")
foreach(required_file IN ITEMS recomp.h NativeTiming.h NativeTiming.c entries.c replacement.cpp)
    if(NOT EXISTS "${ROOT_PORT_TRANSLATED_DIRECTORY}/${required_file}")
        message(FATAL_ERROR "Missing sealed translation input: ${required_file}")
    endif()
endforeach()

file(GLOB translated_sources CONFIGURE_DEPENDS "${ROOT_PORT_TRANSLATED_DIRECTORY}/code[0-9][0-9][0-9].c")
if(NOT translated_sources)
    message(FATAL_ERROR "No translated instruction sources")
endif()
add_subdirectory(../floating_point floating_point)
add_library(static_arm_translation STATIC ${translated_sources}
    "${ROOT_PORT_TRANSLATED_DIRECTORY}/entries.c"
    "${ROOT_PORT_TRANSLATED_DIRECTORY}/NativeTiming.c"
    "${ROOT_PORT_TRANSLATED_DIRECTORY}/replacement.cpp")
set_target_properties(static_arm_translation PROPERTIES
    C_STANDARD 11 C_STANDARD_REQUIRED YES
    CXX_STANDARD 20 CXX_STANDARD_REQUIRED YES)
# Generated source is large. Match the verified native optimization level and
# keep arithmetic contraction disabled. SoftFloat has its own integer target.
target_compile_options(static_arm_translation PRIVATE -O1 -ffp-contract=off)
target_compile_options(static_arm_translation PUBLIC -pthread)
target_link_options(static_arm_translation INTERFACE -pthread)
target_include_directories(static_arm_translation PUBLIC
    "${ROOT_PORT_TRANSLATED_DIRECTORY}"
    "${CMAKE_CURRENT_SOURCE_DIR}/..")
target_include_directories(static_arm_translation PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../../..")
target_link_libraries(static_arm_translation PUBLIC port_floating_point)
