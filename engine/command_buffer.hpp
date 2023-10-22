#pragma once

#include "extra/extra.hpp"
#include "logger.hpp"
#include <vulkan/vulkan.h>

namespace VkRenderer {
    class CommandBuffer {
    private:
        Extra::VkVars* m_vars;

        VkCommandBuffer m_commandBuffer;

        VkCommandBufferAllocateInfo m_commandBufferAllocateInfo{};
    public:
        CommandBuffer(Extra::VkVars* vars);

        inline const VkCommandBuffer& get() const { return m_commandBuffer; }

        void startSingleUse();
        void endSingleUse();
    };
}