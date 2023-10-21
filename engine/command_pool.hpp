#pragma once

#include "extra/extra.hpp"
#include "logger.hpp"
#include "device.hpp"
#include <vulkan/vulkan.h>

namespace VkRenderer {
    class CommandPool {
    private:
        Extra::VkVars* m_vars;

        Extra::QueueFamilyIndices m_queueFamilyIndices;
        VkCommandPoolCreateInfo m_poolInfo{};
    public:
        CommandPool(Extra::VkVars* vars);

        void destroy();
    };
}