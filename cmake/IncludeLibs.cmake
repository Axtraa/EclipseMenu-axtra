cmake_minimum_required(VERSION 3.21)
add_library(third_party INTERFACE)

# Include libraries
add_library(imgui INTERFACE)
CPMAddPackage(
    NAME imgui
    GIT_REPOSITORY https://git.eclipse.menu/EclipseMenu/imgui.git
    GIT_TAG 8cf1bfc66d88ef84cf312865ed81bf17d6622c34
)
target_include_directories(imgui INTERFACE ${imgui_SOURCE_DIR})
target_sources(imgui INTERFACE
    ${imgui_SOURCE_DIR}/imgui.cpp
    ${imgui_SOURCE_DIR}/imgui_widgets.cpp
    ${imgui_SOURCE_DIR}/imgui_tables.cpp
    ${imgui_SOURCE_DIR}/imgui_draw.cpp
    ${imgui_SOURCE_DIR}/imgui_demo.cpp
    ${imgui_SOURCE_DIR}/misc/cpp/imgui_stdlib.cpp
)
set(HAS_IMGUI ON)
set(IMGUI_HOOK_EARLY OFF)

if (NOT ANDROID AND NOT IOS)
    CPMAddPackage(
        NAME discord-presence
        GIT_REPOSITORY https://git.eclipse.menu/EclipseMenu/discord-presence.git
        GIT_TAG 3b50b0854ec24f9b2d4cc8e28f9145f276bb998e
    )
endif()

CPMAddPackage("gh:prevter/gd-imgui-cocos#e66d02bef18f15105500b914a12f2a2d9bb59f21")
CPMAddPackage("gh:prevter/msgpack-for-matjson#12edd5a3c8eb1afa409669020517dac1e4bc0f96")
CPMAddPackage("gh:maxnut/GDR-converter#71104a9e0ae9bee19f3fc4b784303fc2929eab9d")
CPMAddPackage(
    NAME rift
    GIT_REPOSITORY https://git.eclipse.menu/EclipseMenu/rift.git
    GIT_TAG v2.0.2
)
CPMAddPackage("gh:prevter/AdvancedLabel#17ed1c1a84dac0551c7db975038573280594cd92")
CPMAddPackage("gh:prevter/sinaps#7d2f7b13e0946ddb7ae3de1ac4ae3e5ddb893bcf")
CPMAddPackage("gh:prevter/rock#6a2d29ea1fae8b4a5e8fa5f3517bbd92c577bacc")

if (WIN32)
    CPMAddPackage("gh:zyantific/zydis#ae12a099a1323d1535881b50b5afe72cc0894afa")
endif ()

# Fix debug build
if (CMAKE_BUILD_TYPE STREQUAL "Debug" AND WIN32)
    target_compile_definitions(discord-rpc PRIVATE _ITERATOR_DEBUG_LEVEL=0)
    target_compile_definitions(rift PRIVATE _HAS_ITERATOR_DEBUGGING=0)
endif()

# Link them to the project
target_link_libraries(third_party INTERFACE
    GDRconverter
    libGDR
    imgui
    imgui-cocos
    rift
    advanced_label
    sinaps
    rock
)

if (NOT ANDROID AND NOT IOS)
    target_link_libraries(third_party INTERFACE discord-rpc)
endif()

if (WIN32)
    target_link_libraries(third_party INTERFACE Zydis Zycore)
endif ()
