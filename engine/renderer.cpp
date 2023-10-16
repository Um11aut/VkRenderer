#include "renderer.hpp"

VkRenderer::Renderer::Renderer(Window* window)
	: app_window(window)
{
}

void VkRenderer::Renderer::run()
{
	while (!app_window->shouldClose() && !app_window->KeyPressed(GLFW_KEY_ESCAPE)) {
		glfwPollEvents();
		draw();
	}
	vkDeviceWaitIdle(m_variables.m_device);

	syncher.destroy(m_variables.m_device);
}

void VkRenderer::Renderer::init()
{
	initVk();
}

void VkRenderer::Renderer::initVk()
{
	// So basically the initializing part is make in Constructor. Destructor is automatically called by smart pointers when needed
	
	// Validation Layer(for errors)
	app_validation_layer = std::make_shared<VkRenderer::ValidationLayer>(enableValidationLayer);

	// Create Application Instance
	app_instance = std::make_unique<VkRenderer::Instance>(
		app_window->getInfo().appName,
		app_window->getInfo().engineName,
		&m_variables.m_instance,
		app_validation_layer
	);
	app_instance->appendExtension(VK_KHR_SURFACE_EXTENSION_NAME);
	app_instance->create(); // therefore i need to have a create function

	app_instance->printExtensions(true, true); // required, available
	Logger::printSeparator();

	// creating Window surface
	app_surface = std::make_unique<VkRenderer::Surface>(&m_variables, app_window);
	
	// Intializing debugger for our whole program
	if (app_validation_layer->isEnabled()) {
		app_debugger = std::make_unique<VkRenderer::Debugger>(app_validation_layer, m_variables.m_instance);
	}

	// Creating physical and logical Device
	app_device = std::make_unique<VkRenderer::Device>(&m_variables, app_validation_layer);
	app_device->appendExtension(VK_KHR_SWAPCHAIN_EXTENSION_NAME);
	app_device->create();

	// Create Swap Chain
	app_swapChain = std::make_unique<VkRenderer::SwapChain>(&m_variables, app_window);
	app_swapChain->setPresentMode(Extra::Fifo);
	app_swapChain->create();

	app_renderPass = std::make_unique<VkRenderer::RenderPass>(&m_variables, app_swapChain);

	m_mainPipeline = std::make_shared<VkRenderer::GraphicsPipeline>(&m_variables, shaderModule, app_swapChain);
	
	app_swapChain->createFrameBuffers();

	app_commandBuffer = std::make_shared<VkRenderer::CommandBuffer>(&m_variables, app_swapChain, m_mainPipeline);

	syncher.createSyncObjects(m_variables.m_device);
}

void VkRenderer::Renderer::draw()
{
	vkWaitForFences(m_variables.m_device, 1, &syncher.m_inFlightFence, VK_TRUE, UINT64_MAX);
	vkResetFences(m_variables.m_device, 1, &syncher.m_inFlightFence);

	uint32_t imageIndex;
	vkAcquireNextImageKHR(m_variables.m_device, m_variables.m_swapChain, UINT64_MAX, syncher.m_imageAvailableSemaphore, VK_NULL_HANDLE, &imageIndex);

	vkResetCommandBuffer(m_variables.m_commandBuffer, 0);

	app_commandBuffer->record(imageIndex);

	VkSubmitInfo submitInfo{};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

	VkSemaphore waitSemaphores[] = { syncher.m_imageAvailableSemaphore };
	VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
	submitInfo.waitSemaphoreCount = 1;
	submitInfo.pWaitSemaphores = waitSemaphores;
	submitInfo.pWaitDstStageMask = waitStages;

	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &m_variables.m_commandBuffer;

	VkSemaphore signalSemaphores[] = { syncher.m_renderFinishedSemaphore };
	submitInfo.signalSemaphoreCount = 1;
	submitInfo.pSignalSemaphores = signalSemaphores;

	if (vkQueueSubmit(m_variables.m_graphicsQueue, 1, &submitInfo, syncher.m_inFlightFence) != VK_SUCCESS) {}

	VkPresentInfoKHR presentInfo{};
	presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

	presentInfo.waitSemaphoreCount = 1;
	presentInfo.pWaitSemaphores = signalSemaphores;

	VkSwapchainKHR swapChains[] = { m_variables.m_swapChain };
	presentInfo.swapchainCount = 1;
	presentInfo.pSwapchains = swapChains;
	presentInfo.pImageIndices = &imageIndex;

	vkQueuePresentKHR(m_variables.m_presentQueue, &presentInfo);
}