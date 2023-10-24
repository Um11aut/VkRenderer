#include "triangle.hpp"

Triangle::Triangle(Extra::VkVars* vars, std::shared_ptr<VkRenderer::SwapChain> swapChain)
	: variables(vars), swapChain(swapChain)
{
	commandPool = std::make_unique<VkRenderer::CommandPool>(variables);


	descriptor = std::make_shared<VkRenderer::UniformBufferDescriptor>(variables, VK_SHADER_STAGE_VERTEX_BIT, sizeof(Extra::UniformBufferObject), 0);

	vertexBuffer = std::make_shared<VkRenderer::VertexBuffer>(variables, sizeof(vertices[0]) * vertices.size());
	indexBuffer = std::make_shared<VkRenderer::IndexBuffer>(variables, sizeof(indices[0]) * indices.size(), indices.size());

	vertexBuffer->update(vertices.data(), sizeof(vertices[0]) * vertices.size());
	indexBuffer->update(indices.data(), sizeof(indices[0]) * indices.size());

	shaderModule = std::make_shared<VkRenderer::ShaderModule>(variables, "common/shaders/out/fragment.spv", "common/shaders/out/vertex.spv");
	trianglePipeline = std::make_shared<VkRenderer::GraphicsPipeline>(variables, shaderModule, swapChain, vertexBuffer, descriptor);
	
	commandBuffer = std::make_unique<VkRenderer::DrawCommandBuffer>(
		variables, 
		swapChain, 
		trianglePipeline,
		vertexBuffer,
		indexBuffer,
		descriptor);

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

	static auto startTime = std::chrono::high_resolution_clock::now();

	auto currentTime = std::chrono::high_resolution_clock::now();
	float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

	Extra::UniformBufferObject ubo;
	ubo.model = glm::rotate(glm::mat4(1.0f), time * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	ubo.view = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	ubo.proj = glm::perspective(glm::radians(45.0f), swapChain->getExtent().width / (float)swapChain->getExtent().height, 0.1f, 10.0f);
	ubo.proj[1][1] *= -1;

	descriptor->update(ubo, sizeof(ubo), currentFrame);

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
