#pragma once

#include "extra/extra.hpp"
#include "logger.hpp"
#include <vulkan/vulkan.h>

namespace VkRenderer {
    struct Syncher {
        std::vector<VkSemaphore> m_imageAvailableSemaphore;
        std::vector<VkSemaphore> m_renderFinishedSemaphore;
        std::vector<VkFence> m_inFlightFence;

        void createSyncObjects(VkDevice& device);
        void destroy(VkDevice& device);
    };
}