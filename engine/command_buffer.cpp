#include "command_buffer.hpp"

VkRenderer::CommandBuffer::CommandBuffer(Extra::VkVars* vars)
	: m_vars(vars)
{
	m_commandBufferAllocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	m_commandBufferAllocateInfo.commandPool = m_vars->m_commandPool;
	m_commandBufferAllocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	m_commandBufferAllocateInfo.commandBufferCount = 1;

	if (vkAllocateCommandBuffers(m_vars->m_device, &m_commandBufferAllocateInfo, &m_commandBuffer) != VK_SUCCESS) {}
	else {
		Logger::printOnce("Created Command Buffer!", MessageType::Success);
	}
}

void VkRenderer::CommandBuffer::startSingleUse()
{
	VkCommandBufferAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandPool = m_vars->m_commandPool;
	allocInfo.commandBufferCount = 1;

	vkAllocateCommandBuffers(m_vars->m_device, &allocInfo, &m_commandBuffer);

	VkCommandBufferBeginInfo beginInfo{};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

		vkBeginCommandBuffer(m_commandBuffer, &beginInfo);
}

void VkRenderer::CommandBuffer::endSingleUse()
{
		vkEndCommandBuffer(m_commandBuffer);

	VkSubmitInfo submitInfo{};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &m_commandBuffer;

	vkQueueSubmit(m_vars->m_graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
	vkQueueWaitIdle(m_vars->m_graphicsQueue);

	vkFreeCommandBuffers(m_vars->m_device, m_vars->m_commandPool, 1, &m_commandBuffer);
}