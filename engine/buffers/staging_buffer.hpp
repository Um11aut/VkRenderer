#pragma once

#include "buffer.hpp"
#include <vulkan/vulkan.h>

namespace VkRenderer {
	class StagingBuffer : public Buffer {
	public:
		StagingBuffer(Extra::VkVars* vars, VkDeviceSize size) :
			Buffer(
				&vars->m_device,
				&vars->m_physicalDevice,
				size,
				VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
				VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT) {}
		void update(const void* data, VkDeviceSize size) override {
			void* mappedData;
			vkMapMemory(*m_device, m_memory, 0, size, 0, &mappedData);
			memcpy(mappedData, data, static_cast<size_t>(size));
			vkUnmapMemory(*m_device, m_memory);
		}

		void destroy() {
			destroyAsync();
		}

		void bind(VkCommandBuffer commandBuffer) const override {

		}
	};
}