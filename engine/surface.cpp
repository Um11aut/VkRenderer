#include "surface.hpp"

VkRenderer::Surface::Surface(VkSurfaceKHR* surface, std::shared_ptr<VkRenderer::Window>& window, VkInstance& instance)
    : m_surface(surface), m_window(window), m_instance(&instance)
{
    if (glfwCreateWindowSurface(*m_instance, window->getWindow(), nullptr, surface)) {
        Logger::printOnce("Failed to create Window Surface", MessageType::Error);
    }
    else {
        Logger::printOnce("Created Window Surface!", MessageType::Success);
    }
}

VkRenderer::Surface::~Surface()
{
    vkDestroySurfaceKHR(*m_instance, *m_surface, nullptr);
}