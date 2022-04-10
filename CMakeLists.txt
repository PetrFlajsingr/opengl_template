cmake_minimum_required(VERSION 3.20)
project(opengl_template)

if (MSVC AND CMAKE_CXX_COMPILER_VERSION VERSION_GREATER_EQUAL 19.29.30129 AND CMAKE_VERSION VERSION_GREATER 3.20.3)
    set(CMAKE_CXX_STANDARD 23) # /std:c++latest - unlocks the non stable cpp20 features. For new 16.11 versions
else ()
    set(CMAKE_CXX_STANDARD 20) # /std:c++latest for msvc and -std=c++20 for everyone else.
endif ()


add_compile_definitions(GLFW_INCLUDE_NONE)
add_compile_definitions(GLM_FORCE_SWIZZLE)
#add_compile_definitions(FMT_HEADER_ONLY )
#add_compile_definitions(SPDLOG_FMT_EXTERNAL  )

find_package(OpenGL REQUIRED)

include_directories(include)
include_directories(src)
include_directories(include/stb)

find_package(GLFW3 REQUIRED)

add_compile_definitions(PF_GLFW_OPENGL)

find_package(pf_common CONFIG REQUIRED)
find_package(magic_enum CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(range-v3 CONFIG REQUIRED)
find_package(tomlplusplus CONFIG REQUIRED)
find_package(pf_glfw CONFIG REQUIRED)
find_package(pf_mainloop CONFIG REQUIRED)
find_package(pf_imgui CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)

add_library(glad src/glad.c)

file(GLOB GEGL_SOURCES src/geGL/*.cpp src/geGL/private/*.cpp)

add_executable(opengl_template
        src/main.cpp
        src/utils/files.h
        src/utils/files.cpp
        src/ui/DemoImGui.cpp
        src/ui/DemoImGui.h
        src/imgui/ImGuiGlfwOpenGLInterface.cpp
        src/imgui/impl/imgui_impl_glfw.cpp
        src/imgui/impl/imgui_impl_opengl3.cpp
        src/renderers/DemoRenderer.cpp src/renderers/DemoRenderer.h
        src/renderers/Renderer.h
        src/geGL/Shader.cpp
        src/geGL/Shader.h
        ${GEGL_SOURCES})


target_link_libraries(opengl_template glad ${OPENGL_LIBRARIES} pf_common::pf_common pf_imgui::pf_imgui pf_mainloop::pf_mainloop pf_glfw::pf_glfw spdlog::spdlog_header_only
        )

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        "${PROJECT_SOURCE_DIR}/config.toml"
        $<TARGET_FILE_DIR:${PROJECT_NAME}>/config.toml)


add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        "${PROJECT_SOURCE_DIR}/src/shaders/simple_shader.frag"
        "${PROJECT_SOURCE_DIR}/resources/shaders/simple_shader.frag")

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        "${PROJECT_SOURCE_DIR}/src/shaders/simple_shader.vert"
        "${PROJECT_SOURCE_DIR}/resources/shaders/simple_shader.vert")


#get_cmake_property(_variableNames VARIABLES)
#list (SORT _variableNames)
#foreach (_variableName ${_variableNames})
#    message(STATUS "${_variableName}=${${_variableName}}")
#endforeach()