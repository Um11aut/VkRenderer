#include "buffer.hpp"
VkRenderer::Buffer::Buffer(VkDevice* device, VkPhysicalDevice* physicalDevice, VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties)
	: m_device(device), m_size(size), m_usage(usage), m_properties(properties), m_physicalDevice(physicalDevice)
{
	createAsync();
	allocate();
	bindBuffer();
}

void VkRenderer::Buffer::destroyBuffer()
{
	vkFreeMemory(*m_device, m_memory, nullptr);
	vkDestroyBuffer(*m_device, m_buffer, nullptr);
}

void VkRenderer::Buffer::copyTo(std::unique_ptr<VkRenderer::CommandBuffer>& commandBuffer, VkBuffer& dst, VkDeviceSize size)
{
	commandBuffer->startSingleUse();

		VkBufferCopy copyRegion{};
		copyRegion.size = size;
		vkCmdCopyBuffer(commandBuffer->get(), m_buffer, dst, 1, &copyRegion);

	commandBuffer->endSingleUse();
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
	allocInfo.memoryTypeIndex = findMemoryType(*m_physicalDevice, memRequirements.memoryTypeBits, m_properties);

	if (vkAllocateMemory(*m_device, &allocInfo, nullptr, &m_memory) != VK_SUCCESS) {
		throw std::runtime_error("Failed to allocate buffer memory!");
	}
}

void VkRenderer::Buffer::bindBuffer()
{
	vkBindBufferMemory(*m_device, m_buffer, m_memory, 0);
}

void VkRenderer::Buffer::createAsync()
{
	std::future<void> createFuture = std::async(std::launch::async, &Buffer::create, this);
	createFuture.get();
}

void VkRenderer::Buffer::destroyAsync()
{
	std::future<void> destroyFuture = std::async(std::launch::async, &Buffer::destroyBuffer, this);
	destroyFuture.get(); // Wait for completion
}

uint32_t VkRenderer::Buffer::findMemoryType(VkPhysicalDevice& device, uint32_t typeFilter, VkMemoryPropertyFlags properties)
{
	VkPhysicalDeviceMemoryProperties memProperties;
	vkGetPhysicalDeviceMemoryProperties(device, &memProperties);

	for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
		if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
			return i;
		}
	}

	throw std::runtime_error("Failed to find suitable memory type!");
}