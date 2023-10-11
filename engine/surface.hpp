#include <vulkan/vulkan.h>

#include <memory>
#include "window.hpp"
#include "logger.hpp"

namespace VkRenderer {
    class Surface {
    private:
        VkSurfaceKHR* m_surface;
        VkInstance* m_instance;

        std::shared_ptr<VkRenderer::Window> m_window; // so window is bound on a surface
    public:
        Surface(VkSurfaceKHR* surface, std::shared_ptr<VkRenderer::Window> window, VkInstance& instance);
        ~Surface();
    };
}