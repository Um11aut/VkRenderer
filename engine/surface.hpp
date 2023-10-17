#include <vulkan/vulkan.h>

#include <memory>
#include "window.hpp"
#include "logger.hpp"
#include "extra/extra.hpp"

namespace VkRenderer {
    class Surface {
    private:
        Extra::VkVars* m_vars;

        std::shared_ptr<VkRenderer::Window> m_window; // so window is bound on a surface
    public:
        Surface(Extra::VkVars* vars, std::shared_ptr<VkRenderer::Window>& window);

        void destroy();

    };
}