#pragma once

#include <vulkan/vulkan.h>
#include "../command_buffer.hpp"
#include "buffer.hpp"
#include "staging_buffer.hpp"
#include <vector>

namespace VkRenderer {
	class UniformBuffer : public Buffer {
	private:
		void* m_mappedMemory;

	public:
		UniformBuffer(Extra::VkVars* vars, VkDeviceSize size)
			: Buffer(
				&vars->m_device,
				&vars->m_physicalDevice,
				size,
				VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
				VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT)
		{
			vkMapMemory(vars->m_device, m_memory, 0, size, 0, &m_mappedMemory);
		}

		inline void* getMappedMemory() const { return m_mappedMemory; }

		inline VkBuffer getBuffer() const { return m_buffer; }

		void update(const void* data, VkDeviceSize size) override {
		}

		void bind(VkCommandBuffer commandBuffer) const override {
		}

		void destroy() {
			destroyAsync();
		}
	};
}