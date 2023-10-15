#include "drawer.hpp"
void VkRenderer::Syncher::createSyncObjects(VkDevice& device)
{
	VkSemaphoreCreateInfo semaphoreInfo{};
	semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

	VkFenceCreateInfo fenceInfo{};
	fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

	if (vkCreateSemaphore(device, &semaphoreInfo, nullptr, &m_imageAvailableSemaphore) != VK_SUCCESS
		|| vkCreateSemaphore(device, &semaphoreInfo, nullptr, &m_renderFinishedSemaphore) != VK_SUCCESS
		|| vkCreateFence(device, &fenceInfo, nullptr, &m_inFlightFence) != VK_SUCCESS) {
	}
	else {
		Logger::printOnce("Created Semaphores", MessageType::Success);
	}
}

void VkRenderer::Syncher::destroy(VkDevice& device)
{
	vkDestroySemaphore(device, m_imageAvailableSemaphore, nullptr);
	vkDestroySemaphore(device, m_renderFinishedSemaphore, nullptr);
	vkDestroyFence(device, m_inFlightFence, nullptr);
}
