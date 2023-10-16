#pragma once

#include "../extra/extra.hpp"
#include "../logger.hpp"
#include "../device.hpp"
#include "../swap_chain.hpp"
#include "imgui.h"
#include "imgui_impl_vulkan.h"
#include "imgui_impl_glfw.h"
#include <vulkan/vulkan.h>

namespace VkRenderer {
    class GUI {
    private:
        Extra::VkVars* m_vars;

        std::shared_ptr<VkRenderer::SwapChain> m_swapChain;

        ImGui_ImplVulkan_InitInfo initInfo{};
    public:
        GUI(Extra::VkVars* vars, std::shared_ptr<VkRenderer::SwapChain> swapChain);
        ~GUI();
    };
}