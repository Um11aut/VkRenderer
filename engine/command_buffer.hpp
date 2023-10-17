#pragma once

#include "extra/extra.hpp"
#include "logger.hpp"
#include "device.hpp"
#include "swap_chain.hpp"
#include "graphics_pipeline.hpp"
#include <vulkan/vulkan.h>

namespace VkRenderer {
    class CommandBuffer {
    private:
        Extra::VkVars* m_vars;

        std::shared_ptr<SwapChain> m_swapChain;
        std::shared_ptr<GraphicsPipeline> m_graphicsPipeline;
           
        VkCommandPool m_commandPool{};
        Extra::QueueFamilyIndices m_queueFamilyIndices;
        VkCommandPoolCreateInfo m_poolInfo{};

        VkCommandBufferAllocateInfo m_commandBufferAllocateInfo{};
    public:
        CommandBuffer(Extra::VkVars* vars, std::shared_ptr<SwapChain> swapChain, std::shared_ptr<GraphicsPipeline> graphicsPipeline);

        void record(VkCommandBuffer buffer, uint32_t imageIndex);
        void destroy();
    };
}