#include "drawer.hpp"
void VkRenderer::Syncher::createSyncObjects(VkDevice& device)
{
	m_imageAvailableSemaphore.resize(Extra::FRAMES_IN_FLIGHT);
	m_renderFinishedSemaphore.resize(Extra::FRAMES_IN_FLIGHT);
	m_inFlightFence.resize(Extra::FRAMES_IN_FLIGHT);

	VkSemaphoreCreateInfo semaphoreInfo{};
	semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

	VkFenceCreateInfo fenceInfo{};
	fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

	for (size_t i = 0; i < Extra::FRAMES_IN_FLIGHT; i++) {
		if (vkCreateSemaphore(device, &semaphoreInfo, nullptr, &m_imageAvailableSemaphore[i]) != VK_SUCCESS
			|| vkCreateSemaphore(device, &semaphoreInfo, nullptr, &m_renderFinishedSemaphore[i]) != VK_SUCCESS
			|| vkCreateFence(device, &fenceInfo, nullptr, &m_inFlightFence[i]) != VK_SUCCESS) {
		}
		else {
			Logger::printOnce("Created Semaphores", MessageType::Success);
		}
	}
}

void VkRenderer::Syncher::destroy(VkDevice& device)
{
	for (size_t i = 0; i < Extra::FRAMES_IN_FLIGHT; i++) {
		vkDestroySemaphore(device, m_imageAvailableSemaphore[i], nullptr);
		vkDestroySemaphore(device, m_renderFinishedSemaphore[i], nullptr);
		vkDestroyFence(device, m_inFlightFence[i], nullptr);
	}
}
