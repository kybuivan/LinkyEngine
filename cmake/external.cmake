# Download all submodules
find_package(Git QUIET)
if(GIT_FOUND AND EXISTS "${PROJECT_SOURCE_DIR}/.git")
# update submodules as needed
    option(GIT_SUBMODULE "Check submodules during build" ON)
    if(GIT_SUBMODULE)
        message(STATUS "submodule update")
        execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive
                        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                        RESULT_VARIABLE GIT_SUBMOD_RESULT)
        if(NOT_GIT_SUBMOD_RESULT EQUAL "0")
            message(FATAL_ERROR "git submodule update --init failed with $(GIT_SUBMOD_RESULT), please checkout submodules")
        endif()
    endif()
endif()

if(NOT EXISTS "${PROJECT_SOURCE_DIR}/external/glfw/CMakeLists.txt")
    message(FATAL_ERROR "The glfw submodules was not downloaded! GIT_SUBMODULES was turned off or failed. Please checkout submodules")
endif()

#if(NOT EXISTS "${PROJECT_SOURCE_DIR}/external/glad/CMakeLists.txt")
#    message(FATAL_ERROR "The glad submodules was not downloaded! GIT_SUBMODULES was turned off or failed. Please checkout submodules")
#endif()

#find_package(OpenGL REQUIRED)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory (${PROJECT_SOURCE_DIR}/external/glfw)

add_subdirectory("${PROJECT_SOURCE_DIR}/external/glad/cmake" glad_cmake)
glad_add_library(glad_gl_core_33 STATIC API gl:core=3.3)
set(GLM_TEST_ENABLE OFF CACHE BOOL "" FORCE)
add_subdirectory(${PROJECT_SOURCE_DIR}/external/glm)

# ImGui setup
set(IMGUI_DIR ${PROJECT_SOURCE_DIR}/external/imgui)
add_library(imgui STATIC)
target_sources( imgui
    PRIVATE
        ${IMGUI_DIR}/imgui_demo.cpp
        ${IMGUI_DIR}/imgui_draw.cpp
        ${IMGUI_DIR}/imgui_tables.cpp
        ${IMGUI_DIR}/imgui_widgets.cpp
        ${IMGUI_DIR}/imgui.cpp
        ${IMGUI_DIR}/misc/cpp/imgui_stdlib.cpp

    PRIVATE
        ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
        ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp
)
target_include_directories( imgui
    PUBLIC ${IMGUI_DIR}
    PUBLIC ${IMGUI_DIR}/backends
    PUBLIC ${PROJECT_SOURCE_DIR}/external/glfw/include
)
add_library(imgui::imgui ALIAS imgui)

# Generate implementation file
file(WRITE "${CMAKE_BINARY_DIR}/external/stb/stb_image.cpp.in" [[
    #define STB_IMAGE_IMPLEMENTATION
    #include <stb_image.h>

    #define STB_IMAGE_WRITE_IMPLEMENTATION
    #include <stb_image_write.h>
]])

configure_file(${CMAKE_BINARY_DIR}/external/stb/stb_image.cpp.in ${CMAKE_BINARY_DIR}/external/stb/stb_image.cpp COPYONLY)

# Define stb library
add_library(stb ${CMAKE_BINARY_DIR}/external/stb/stb_image.cpp)
add_library(stb::stb ALIAS stb)

target_include_directories(stb PUBLIC ${PROJECT_SOURCE_DIR}/external/stb)