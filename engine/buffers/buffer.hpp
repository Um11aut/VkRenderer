#pragma once
#include "../extra/extra.hpp"
#include <vulkan/vulkan.h>
#include <stdexcept>

namespace VkRenderer {
	class Buffer {
	public:
		Buffer(VkDevice* device, VkPhysicalDevice* physicalDevice, VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties);
		virtual ~Buffer() = default;

		void destroyBuffer();

		virtual void update(const void* data, VkDeviceSize size) = 0;
		virtual void bind(VkCommandBuffer buffer) const = 0;

	protected:
		VkDevice* m_device;
		VkPhysicalDevice* m_physicalDevice;

		VkDeviceSize m_size;
		VkBufferUsageFlags m_usage;
		VkMemoryPropertyFlags m_properties;
		VkDeviceMemory m_memory;

		VkBuffer m_buffer;

		void create();
		void allocate();
		void bindBuffer();

		uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) const;
	};
}