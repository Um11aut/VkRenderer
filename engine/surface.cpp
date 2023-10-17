#include "surface.hpp"

VkRenderer::Surface::Surface(Extra::VkVars* vars, std::shared_ptr<VkRenderer::Window>& window)
    : m_vars(vars), m_window(window)
{
    if (glfwCreateWindowSurface(m_vars->m_instance, window->getWindow(), nullptr, &m_vars->m_surface)) {
        Logger::printOnce("Failed to create Window Surface", MessageType::Error);
    }
    else {
        Logger::printOnce("Created Window Surface!", MessageType::Success);
    }
}

void VkRenderer::Surface::destroy()
{
    Logger::printOnce("Destroyed Surface!");
    vkDestroySurfaceKHR(m_vars->m_instance, m_vars->m_surface, nullptr);
}