#pragma once

#include "extra/extra.hpp"
#include "logger.hpp"
#include "device.hpp"
#include "swap_chain.hpp"
#include "graphics_pipeline.hpp"
#include "drawer.hpp"
#include <vulkan/vulkan.h>

namespace VkRenderer {



    class DrawCommandBuffer {
    private:
        Extra::VkVars* m_vars;

        std::shared_ptr<SwapChain> m_swapChain;
        std::shared_ptr<GraphicsPipeline> m_graphicsPipeline;
        std::shared_ptr<VertexBuffer> m_vertexBuffer;

        std::vector<VkCommandBuffer> m_commandBuffers;
        VkCommandPool m_commandPool{};
        Extra::QueueFamilyIndices m_queueFamilyIndices;
        VkCommandPoolCreateInfo m_poolInfo{};

        VkCommandBufferAllocateInfo m_commandBufferAllocateInfo{};
    public:
        DrawCommandBuffer(Extra::VkVars* vars, std::shared_ptr<SwapChain> swapChain, std::shared_ptr<GraphicsPipeline> graphicsPipeline, std::shared_ptr<VertexBuffer> vertexBuffer);

        void submit(const uint32_t currentFrame, const Syncher& syncher);
        void record(const uint32_t currentFrame, uint32_t imageIndex);
        void destroy();
    };
}