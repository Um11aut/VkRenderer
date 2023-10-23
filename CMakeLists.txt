# CMakeList.txt: проект CMake для VkRenderer; включите исходный код и определения,
# укажите здесь логику для конкретного проекта.
#
cmake_minimum_required (VERSION 3.8)

# Включение горячей перезагрузки для компиляторов MSVC, если поддерживается.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("VkRenderer")

set(source "${CMAKE_SOURCE_DIR}/common")
set(destination "${CMAKE_CURRENT_BINARY_DIR}/common")

set(IMGUI_DIR ${CMAKE_SOURCE_DIR}/deps/imgui_t)

# Добавьте источник в исполняемый файл этого проекта.
add_executable (VkRenderer 
"main.cpp" "engine/entry.hpp" 

"engine/renderer.hpp" "engine/renderer.cpp" 
"engine/window.cpp" "engine/window.hpp" 
"engine/instance.cpp" "engine/instance.hpp" 
"engine/logger.cpp" "engine/logger.hpp" 
"engine/validation_layers.cpp" "engine/validation_layers.hpp"
"engine/debugger.hpp" "engine/debugger.cpp" 
"engine/device.hpp" "engine/device.cpp"
"engine/surface.hpp" "engine/surface.cpp" 
"engine/swap_chain.cpp"  
"engine/shader.cpp" 
"engine/graphics_pipeline.cpp" 
"engine/render_pass.cpp"
"engine/command_buffer.cpp" "engine/command_buffer.hpp" 
"engine/drawer.cpp" "engine/drawer.hpp" 
"engine/gui/gui.cpp" "engine/gui/gui.hpp" "engine/layers/triangle.cpp" 
"engine/layers/parent/layer_factory.hpp" "engine/layers/parent/layer.hpp" 
"engine/buffers/buffer.hpp" "engine/buffers/buffer.cpp" 
"engine/buffers/vertex_buffer.cpp" "engine/buffers/vertex_buffer.hpp"
"engine/command_pool.cpp" "engine/command_pool.hpp" 
"engine/draw_command_buffer.cpp" 
"engine/buffers/index_buffer.hpp"
"engine/buffers/staging_buffer.hpp" 
"engine/buffers/uniform_buffer.hpp" 
"engine/descriptor.hpp" "engine/descriptor.cpp")


add_custom_command(TARGET VkRenderer
COMMAND ${CMAKE_COMMAND} -E create_symlink ${source} ${destination}
DEPENDS ${destination}
COMMENT "symbolic link common folder from ${source} => ${destination}")

add_subdirectory(deps/glfw)
add_subdirectory(deps/glm)

find_package(Vulkan REQUIRED)

add_library(imgui
    "${IMGUI_DIR}/imgui.h"
    "${IMGUI_DIR}/imgui.cpp"

    "${IMGUI_DIR}/imgui_demo.cpp"
    "${IMGUI_DIR}/imgui_draw.cpp"
    "${IMGUI_DIR}/imgui_widgets.cpp"
)
add_library(imgui_impl_vulkan STATIC
        ${IMGUI_DIR}/imgui_impl_vulkan.cpp
        ${IMGUI_DIR}/imgui_impl_glfw.cpp
        )

include_directories(${Vulkan_INCLUDE_DIRS} ${GLFW_INCLUDE_DIRS} ${IMGUI_DIR})

target_include_directories(imgui PUBLIC ${IMGUI_DIR})
target_include_directories(imgui_impl_vulkan PUBLIC ${IMGUI_DIR} ${VULKAN_LIBRARIES} ${GLFW_LIBRARIES})

target_link_libraries(VkRenderer 
    ${Vulkan_LIBRARIES} 
    glfw 
    ${GLFW_LIBRARIES} 
    Vulkan::Vulkan 
    glm
)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET VkRenderer PROPERTY CXX_STANDARD 20)
endif()

# TODO: Добавьте тесты и целевые объекты, если это необходимо.
