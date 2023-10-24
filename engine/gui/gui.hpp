#pragma once

#include "../extra/extra.hpp"
#include "../logger.hpp"
#include "../device.hpp"
#include "../swap_chain.hpp"
#include "gui_render_pass.hpp"
#include <vulkan/vulkan.h>

namespace VkRenderer {
    class GUI {
    private:
        Extra::VkVars* m_vars;

        std::shared_ptr<VkRenderer::SwapChain> m_swapChain;

        std::unique_ptr<Gui::RenderPass> m_GuiRenderer;
    public:
        GUI(Extra::VkVars* vars, std::shared_ptr<VkRenderer::SwapChain> swapChain);

        void update() {
            
            m_GuiRenderer->update();
        }

        ~GUI();
    };
}