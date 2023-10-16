#include "triangle.hpp"

Triangle::Triangle(Extra::VkVars* vars, std::shared_ptr<VkRenderer::SwapChain> swapChain)
	: variables(vars), app_swapChain(swapChain)
{
	app_renderPass = std::make_unique<VkRenderer::RenderPass>(variables, app_swapChain);

	m_mainPipeline = std::make_shared<VkRenderer::GraphicsPipeline>(variables, shaderModule, app_swapChain);

	app_swapChain->createFrameBuffers();

	app_commandBuffer = std::make_unique<VkRenderer::CommandBuffer>(variables, app_swapChain, m_mainPipeline);

	app_gui = std::make_unique<VkRenderer::GUI>(variables, app_swapChain);

	syncher = std::make_unique<VkRenderer::Syncher>();

	syncher->createSyncObjects(variables->m_device);
}


void Triangle::draw()
{
	vkWaitForFences(variables->m_device, 1, &syncher->m_inFlightFence, VK_TRUE, UINT64_MAX);

	uint32_t imageIndex;
	vkAcquireNextImageKHR(variables->m_device, variables->m_swapChain, UINT64_MAX, syncher->m_imageAvailableSemaphore, VK_NULL_HANDLE, &imageIndex);

	vkResetFences(variables->m_device, 1, &syncher->m_inFlightFence);
	app_commandBuffer->record(imageIndex);

	VkSubmitInfo submitInfo{};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

	VkSemaphore waitSemaphores[] = { syncher->m_imageAvailableSemaphore };
	VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
	submitInfo.waitSemaphoreCount = 1;
	submitInfo.pWaitSemaphores = waitSemaphores;
	submitInfo.pWaitDstStageMask = waitStages;

	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &variables->m_commandBuffer;

	VkSemaphore signalSemaphores[] = { syncher->m_renderFinishedSemaphore };
	submitInfo.signalSemaphoreCount = 1;
	submitInfo.pSignalSemaphores = signalSemaphores;

	if (vkQueueSubmit(variables->m_graphicsQueue, 1, &submitInfo, syncher->m_inFlightFence) != VK_SUCCESS) {}

	VkPresentInfoKHR presentInfo{};
	presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

	presentInfo.waitSemaphoreCount = 1;
	presentInfo.pWaitSemaphores = signalSemaphores;

	VkSwapchainKHR swapChains[] = { variables->m_swapChain };
	presentInfo.swapchainCount = 1;
	presentInfo.pSwapchains = swapChains;
	presentInfo.pImageIndices = &imageIndex;

	vkQueuePresentKHR(variables->m_presentQueue, &presentInfo);
}
