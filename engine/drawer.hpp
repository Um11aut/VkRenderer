#pragma once

#include "extra/extra.hpp"
#include "logger.hpp"
#include <vulkan/vulkan.h>

namespace VkRenderer {
    struct Syncher {
        VkSemaphore m_imageAvailableSemaphore;
        VkSemaphore m_renderFinishedSemaphore;
        VkFence m_inFlightFence;

        void createSyncObjects(VkDevice& device);
        void destroy(VkDevice& device);
    };
}