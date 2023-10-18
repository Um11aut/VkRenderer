#include "buffer.hpp"
VkRenderer::Buffer::Buffer(VkDevice* device, VkPhysicalDevice* physicalDevice, VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties)
	: m_device(device), m_size(size), m_usage(usage), m_properties(properties), m_physicalDevice(physicalDevice)
{
	create();
	allocate();
	bindBuffer();
}

void VkRenderer::Buffer::destroyBuffer()
{
	vkFreeMemory(*m_device, m_memory, nullptr);
	vkDestroyBuffer(*m_device, m_buffer, nullptr);
}

void VkRenderer::Buffer::create()
{
	VkBufferCreateInfo bufferInfo{};
	bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	bufferInfo.size = m_size;
	bufferInfo.usage = m_usage;
	bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

	if (vkCreateBuffer(*m_device, &bufferInfo, nullptr, &m_buffer) != VK_SUCCESS) {
		throw std::runtime_error("Failed to create buffer!");
	}
}

void VkRenderer::Buffer::allocate()
{
	VkMemoryRequirements memRequirements;
	vkGetBufferMemoryRequirements(*m_device, m_buffer, &memRequirements);

	VkMemoryAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	allocInfo.allocationSize = memRequirements.size;
	allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, m_properties);

	if (vkAllocateMemory(*m_device, &allocInfo, nullptr, &m_memory) != VK_SUCCESS) {
		throw std::runtime_error("Failed to allocate buffer memory!");
	}
}

void VkRenderer::Buffer::bindBuffer()
{
	vkBindBufferMemory(*m_device, m_buffer, m_memory, 0);
}

uint32_t VkRenderer::Buffer::findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) const
{
	VkPhysicalDeviceMemoryProperties memProperties;
	vkGetPhysicalDeviceMemoryProperties(*m_physicalDevice, &memProperties);

	for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
		if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
			return i;
		}
	}

	throw std::runtime_error("Failed to find suitable memory type!");
}