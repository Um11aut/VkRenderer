#include "command_pool.hpp"

VkRenderer::CommandPool::CommandPool(Extra::VkVars* vars)
	: m_vars(vars)
{
	m_queueFamilyIndices = Device::findSupportedQueueFamilies(m_vars->m_physicalDevice, m_vars->m_surface);

	m_poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	m_poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
	m_poolInfo.queueFamilyIndex = 0;

	if (vkCreateCommandPool(m_vars->m_device, &m_poolInfo, nullptr, &m_vars->m_commandPool) != VK_SUCCESS) {}
	else {
		Logger::printOnce("Created Command Pool", MessageType::Success);
	}
}

void VkRenderer::CommandPool::destroy()
{
	vkDestroyCommandPool(m_vars->m_device, m_vars->m_commandPool, nullptr);
}
