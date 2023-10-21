#include "triangle.hpp"

Triangle::Triangle(Extra::VkVars* vars, std::shared_ptr<VkRenderer::SwapChain> swapChain)
	: variables(vars), swapChain(swapChain)
{
	renderPass = std::make_unique<VkRenderer::RenderPass>(variables, swapChain);

	vertexBuffer = std::make_shared<VkRenderer::VertexBuffer>(variables, sizeof(vertices[0]) * vertices.size());

	vertexBuffer->update(vertices.data(), sizeof(vertices[0]) * vertices.size());

	shaderModule = std::make_shared<VkRenderer::ShaderModule>(variables, "common/shaders/out/fragment.spv", "common/shaders/out/vertex.spv");
	trianglePipeline = std::make_shared<VkRenderer::GraphicsPipeline>(variables, shaderModule, swapChain, vertexBuffer);

	commandPool = std::make_unique<VkRenderer::CommandPool>(variables);

	commandBuffer = std::make_unique<VkRenderer::DrawCommandBuffer>(variables, swapChain, trianglePipeline, vertexBuffer);

	syncher = std::make_unique<VkRenderer::Syncher>();

	syncher->createSyncObjects(variables->m_device);
}


void Triangle::draw()
{
	vkWaitForFences(variables->m_device, 1, &syncher->m_inFlightFence[currentFrame], VK_TRUE, UINT64_MAX);

	uint32_t imageIndex;
	VkResult result = vkAcquireNextImageKHR(variables->m_device, variables->m_swapChain, UINT64_MAX, syncher->m_imageAvailableSemaphore[currentFrame], VK_NULL_HANDLE, &imageIndex);

	if (result == VK_ERROR_OUT_OF_DATE_KHR) {
		swapChain->recreate();
		return;
	}
	else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
		throw std::runtime_error("failed to acquire swap chain image!");
	}

	vkResetFences(variables->m_device, 1, &syncher->m_inFlightFence[currentFrame]);
	commandBuffer->record(currentFrame, imageIndex);

	commandBuffer->submit(currentFrame, *syncher);

	VkSemaphore signalSemaphores[] = { syncher->m_renderFinishedSemaphore[currentFrame] };

	VkPresentInfoKHR presentInfo{};
	presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

	presentInfo.waitSemaphoreCount = 1;
	presentInfo.pWaitSemaphores = signalSemaphores;

	VkSwapchainKHR swapChains[] = { variables->m_swapChain };
	presentInfo.swapchainCount = 1;
	presentInfo.pSwapchains = swapChains;
	presentInfo.pImageIndices = &imageIndex;

	result = vkQueuePresentKHR(variables->m_presentQueue, &presentInfo);

	if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR) {
		swapChain->recreate();
	}
	else if (result != VK_SUCCESS) {
		throw std::runtime_error("failed to present swap chain image!");
	}

	currentFrame = (currentFrame + 1) % Extra::FRAMES_IN_FLIGHT;
	frames++;
}
