cmake_minimum_required(VERSION 3.20)

project(TERRARIA LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

file(GLOB_RECURSE SOURCES
    "${PROJECT_SOURCE_DIR}/src/*.cpp"
)

add_executable(TERRARIA ${SOURCES})

target_include_directories(TERRARIA PRIVATE
    ${PROJECT_SOURCE_DIR}/src
    ${PROJECT_SOURCE_DIR}/thirdparty/FastNoiseSIMD
)

set(SFML_DIR "C:/SFML/lib/cmake/SFML")

find_package(SFML CONFIG REQUIRED COMPONENTS
    Graphics
    Window
    System
    Audio
)

add_library(FastNoiseSIMD STATIC
    thirdparty/FastNoiseSIMD/FastNoiseSIMD.cpp
    thirdparty/FastNoiseSIMD/FastNoiseSIMD_avx2.cpp
    thirdparty/FastNoiseSIMD/FastNoiseSIMD_avx512.cpp
    thirdparty/FastNoiseSIMD/FastNoiseSIMD_sse41.cpp
    thirdparty/FastNoiseSIMD/FastNoiseSIMD_sse2.cpp
    thirdparty/FastNoiseSIMD/FastNoiseSIMD_internal.cpp
)


target_link_libraries(TERRARIA PRIVATE
    FastNoiseSIMD
    SFML::Graphics
    SFML::Window
    SFML::System
    SFML::Audio
)

set_source_files_properties(
    thirdparty/FastNoiseSIMD/FastNoiseSIMD_avx2.cpp
    PROPERTIES COMPILE_FLAGS /arch:AVX2
)

if (WIN32)
    add_custom_command(TARGET TERRARIA POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "C:/SFML/bin"
        $<TARGET_FILE_DIR:TERRARIA>
    )
endif()

# Copy resources folder next to executable after build
add_custom_command(TARGET TERRARIA POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${PROJECT_SOURCE_DIR}/resources"
    "$<TARGET_FILE_DIR:TERRARIA>/resources"
)