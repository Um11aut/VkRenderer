#pragma once
#include "../extra/extra.hpp"
#include <vulkan/vulkan.h>
#include <stdexcept>
#include <memory>
#include <future>
#include "../command_buffer.hpp"

namespace VkRenderer {
	class Buffer {
	public:
		Buffer(VkDevice* device, VkPhysicalDevice* physicalDevice, VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties);
		Buffer() {}
		virtual ~Buffer() = default;
		
		void destroyBuffer();

		void copyTo(std::unique_ptr<VkRenderer::CommandBuffer>& comamndBuffer, VkBuffer& dst, VkDeviceSize size);

		virtual void update(const void* data, VkDeviceSize size) = 0;
		virtual void bind(VkCommandBuffer buffer) const = 0;

		static uint32_t findMemoryType(VkPhysicalDevice& device, uint32_t typeFilter, VkMemoryPropertyFlags properties);
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

		void createAsync();
		void destroyAsync();

	};
}