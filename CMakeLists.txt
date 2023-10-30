cmake_minimum_required(VERSION 3.10)

if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project(VkRenderer CXX)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug CACHE STRING "" FORCE)
endif()

set(CMAKE_CXX_STANDARD 20)
set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DVK_PROTOTYPES")
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DVK_PROTOTYPES")

# GLFW
set(GLFW_DIR deps/glfw) # Set this to point to an up-to-date GLFW repo
option(GLFW_BUILD_EXAMPLES "Build the GLFW example programs" OFF)
option(GLFW_BUILD_TESTS "Build the GLFW test programs" OFF)
option(GLFW_BUILD_DOCS "Build the GLFW documentation" OFF)
option(GLFW_INSTALL "Generate installation target" OFF)
option(GLFW_DOCUMENT_INTERNALS "Include internals in documentation" OFF)
add_subdirectory(${GLFW_DIR} binary_dir EXCLUDE_FROM_ALL)
include_directories(${GLFW_DIR}/include)

add_subdirectory(deps/glm)

# Dear ImGui
set(IMGUI_DIR deps/imgui)
include_directories(${IMGUI_DIR} ${IMGUI_DIR}/backends ..)

set(STB_DIR deps/stb)

set(source "${CMAKE_SOURCE_DIR}/common")
set(destination "${CMAKE_CURRENT_BINARY_DIR}/common")

# Libraries
find_package(Vulkan REQUIRED)
set(LIBRARIES "glfw;Vulkan::Vulkan;glm")

# Use vulkan headers from glfw:
include_directories(
    ${IMGUI_DIR}
    ${Vulkan_INCLUDE_DIRS}
    ${GLFW_INCLUDE_DIRS}  # Include GLFW directories
)

file(GLOB sources add_executable VkRenderer 
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
"engine/gui/gui.cpp" "engine/gui/gui.hpp" "engine/layers/triangle.cpp" "engine/layers/triangle.hpp" 
"engine/layers/parent/layer_factory.hpp" "engine/layers/parent/layer.hpp" 
"engine/buffers/buffer.hpp" "engine/buffers/buffer.cpp" 
"engine/buffers/vertex_buffer.cpp" "engine/buffers/vertex_buffer.hpp"
"engine/command_pool.cpp" "engine/command_pool.hpp" 
"engine/draw_command_buffer.cpp" 
"engine/buffers/index_buffer.hpp"
"engine/buffers/staging_buffer.hpp" 
"engine/buffers/uniform_buffer.hpp" 
"engine/ubo_descriptor.hpp" "engine/ubo_descriptor.cpp" 
"engine/layers/interface.cpp" "engine/layers/interface.hpp"
"engine/gui/gui.hpp" "engine/gui/gui.cpp"
"engine/gui/gui_renderer.hpp" "engine/texture/texture.hpp" "engine/texture/texture.cpp")


include_directories(
    ${STB_DIR}
)
add_library(stb_image INTERFACE)
target_include_directories(stb_image INTERFACE ${STB_DIR})

add_executable(VkRenderer ${sources} ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp ${IMGUI_DIR}/backends/imgui_impl_vulkan.cpp ${IMGUI_DIR}/imgui.cpp ${IMGUI_DIR}/imgui_draw.cpp ${IMGUI_DIR}/imgui_demo.cpp ${IMGUI_DIR}/imgui_tables.cpp ${IMGUI_DIR}/imgui_widgets.cpp "engine/texture/texture_descriptor.cpp")

add_custom_command(TARGET VkRenderer
    COMMAND ${CMAKE_COMMAND} -E create_symlink ${source} ${destination}
    DEPENDS ${destination}
    COMMENT "symbolic link common folder from ${source} => ${destination}")

target_link_libraries(VkRenderer ${LIBRARIES} stb_image)
target_compile_definitions(VkRenderer PUBLIC -DImTextureID=ImU64)
