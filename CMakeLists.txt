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


# Добавьте источник в исполняемый файл этого проекта.
add_executable (VkRenderer "main.cpp" "engine/entry.hpp" 
"engine/renderer.hpp" "engine/renderer.cpp" 
"engine/window.cpp" "engine/window.hpp" 
"engine/instance.cpp" "engine/instance.hpp" 
"engine/logger.cpp" "engine/logger.hpp" 
"engine/validation_layers.cpp" "engine/validation_layers.hpp"
"engine/debugger.hpp" "engine/debugger.cpp" 
"engine/device.hpp" "engine/device.cpp"
"engine/surface.hpp" "engine/surface.cpp" "engine/swap_chain.cpp" )

add_subdirectory(deps/glfw)
add_subdirectory(deps/glm)

find_package(Vulkan REQUIRED)

include_directories(${Vulkan_INCLUDE_DIRS})
target_link_libraries(VkRenderer ${Vulkan_LIBRARIES})
target_link_libraries(VkRenderer glfw ${GLFW_LIBRARIES} Vulkan::Vulkan)
target_link_libraries(VkRenderer glm)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET VkRenderer PROPERTY CXX_STANDARD 20)
endif()

# TODO: Добавьте тесты и целевые объекты, если это необходимо.
