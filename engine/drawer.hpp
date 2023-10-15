#pragma once

#include "extra/extra.hpp"
#include "logger.hpp"
#include <vulkan/vulkan.h>

namespace VkRenderer {
    namespace Syncher {
        inline VkSemaphore m_imageAvailableSemaphore;
        inline VkSemaphore m_renderFinishedSemaphore;
        inline VkFence m_inFlightFence;

        void createSyncObjects(VkDevice& device);
        void destroy(VkDevice& device);
    }
}