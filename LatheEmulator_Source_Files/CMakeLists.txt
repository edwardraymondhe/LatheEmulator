cmake_minimum_required(VERSION 3.17)
project(CG_OpenGL)

set(CMAKE_CXX_STANDARD 11)

#glfw
option(GLFW_BUILD_TESTS OFF)
option(GLFW_BUILD_DOCS OFF)
option(GLFW_BUILD_EXAMPLES OFF)
add_subdirectory(External/glfw-master)
include_directories(External/glfw-master/include)
LIST(APPEND LIBS glfw ${GLFW_LIBRARIES})

#glad
set(GLAD_SRC External/glad-c/src/glad.c)
if(NOT_WIN32)
    LIST(APPEND LIBS dl)
endif()
include_directories(External/glad-c/include)

#glm
include_directories(External/glm-0.9.9.7)

#assimp
option(ASSIMP_BUILD_ASSIMP_TOOLS OFF)
option(ASSIMP_BUILD_TESTS OFF)
option(INJECT_DEBUG_POSTFIX OFF)  #on if debug
add_subdirectory(External/assimp-5.0.1)
include_directories(External/assimp-5.0.1/include)
include_directories(${CMAKE_CURRENT_BINARY_DIR}/External/assimp-5.0.1/include)
LIST(APPEND LIBS assimp)

#ours
include_directories(include)


set(SOURCE_FILES main.cpp ${GLAD_SRC} )
add_executable(CG_OpenGL ${SOURCE_FILES} "main.cpp" "include/camera.h" "include/cylinder.h" "include/imgui/imconfig.h" "include/imgui/imgui.cpp" "include/imgui/imgui.h" "include/imgui/imgui_demo.cpp" "include/imgui/imgui_draw.cpp" "include/imgui/imgui_impl_glfw.h" "include/imgui/imgui_impl_glut.h" "include/imgui/imgui_impl_opengl3.h" "include/imgui/imgui_internal.h" "include/imgui/imgui_tables.cpp" "include/imgui/imgui_widgets.cpp" "include/imgui/imstb_rectpack.h" "include/imgui/imstb_textedit.h" "include/imgui/imstb_truetype.h" "include/mesh.h" "include/model.h" "include/particle.h" "include/shader.h" "include/stb_image.h")
target_link_libraries(CG_OpenGL ${LIBS})