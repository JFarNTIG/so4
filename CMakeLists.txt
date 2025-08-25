cmake_minimum_required(VERSION 3.20)

project(spacegame7 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG   "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Debug")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Release")

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
     "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cxx"
     "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

add_executable(${PROJECT_NAME} ${SOURCES})

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_SOURCE_DIR}/settings.ini"
        $<TARGET_FILE_DIR:${PROJECT_NAME}>
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_SOURCE_DIR}/imgui.ini"
        $<TARGET_FILE_DIR:${PROJECT_NAME}>
)


file(GLOB_RECURSE PROJECT_HEADERS CONFIGURE_DEPENDS
     "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h"
     "${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp"
     "${CMAKE_CURRENT_SOURCE_DIR}/include/*.hxx")

set(ALL_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/include")
foreach(_h ${PROJECT_HEADERS})
  get_filename_component(_dir "${_h}" DIRECTORY)
  list(APPEND ALL_INCLUDE_DIRS "${_dir}")
endforeach()
list(REMOVE_DUPLICATES ALL_INCLUDE_DIRS)

target_include_directories(${PROJECT_NAME} PRIVATE ${ALL_INCLUDE_DIRS})


find_package(SFML 2.5 COMPONENTS audio network window graphics system REQUIRED)
find_package(Freetype REQUIRED)
find_package(OpenGL REQUIRED)


find_package(Lua 5.3 REQUIRED)
if (TARGET Lua::Lua)
    target_link_libraries(${PROJECT_NAME} PRIVATE Lua::Lua)
else()
    target_include_directories(${PROJECT_NAME} PRIVATE ${LUA_INCLUDE_DIR})
    target_link_libraries(${PROJECT_NAME} PRIVATE ${LUA_LIBRARIES})
endif()


target_compile_definitions(${PROJECT_NAME} PRIVATE
    $<$<CONFIG:Debug>:_DEBUG>
    $<$<CONFIG:Release>:NDEBUG>
    _CONSOLE
)

if (WIN32)
    target_compile_definitions(${PROJECT_NAME} PRIVATE UNICODE _UNICODE NOMINMAX)
endif()


target_link_libraries(${PROJECT_NAME} PRIVATE
    sfml-audio sfml-network sfml-window sfml-system sfml-graphics
    Freetype::Freetype
    OpenGL::GL
    ${LUA_LIBRARIES}
)

if (WIN32)
    target_link_libraries(${PROJECT_NAME} PRIVATE
        kernel32 user32 gdi32 winspool comdlg32 advapi32
        shell32 ole32 oleaut32 uuid odbc32 odbccp32
        opengl32 glu32
    )
else()
    target_link_libraries(${PROJECT_NAME} PRIVATE
        pthread
        dl
        m
    )
endif()


if (MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4 /permissive-)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE
        $<$<CONFIG:Debug>:-O0 -g -Wall -Wextra -Wpedantic>
        $<$<CONFIG:Release>:-O3 -pipe -Wall -Wextra -Wpedantic>
    )
endif()

