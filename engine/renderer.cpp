#include "renderer.hpp"

VkRenderer::Renderer::Renderer(AppInfo info)
	: m_info(info)
{
	initVk();
}

void VkRenderer::Renderer::run()
{
	while (!app_window->shouldClose() && !app_window->KeyPressed(GLFW_KEY_ESCAPE)) {
		glfwPollEvents();
	}
}

void VkRenderer::Renderer::initVk()
{
	app_window = std::make_shared<VkRenderer::Window>(m_info.windowWidth, m_info.windowHeight, m_info.appName);
	app_validation_layer = std::make_shared<VkRenderer::ValidationLayer>(enableValidationLayer);

	app_instance = std::make_unique<VkRenderer::Instance>(
		m_info.appName, m_info.engineName, &m_instance, app_validation_layer);

	app_instance->create();
	app_instance->printExtensions(true, false);
	
	if (app_validation_layer->isEnabled()) {
		app_debugger = std::make_unique<VkRenderer::Debugger>(app_validation_layer, &m_instance);
	}

	app_device = std::make_unique<VkRenderer::Device>(&m_physicalDevice, &m_device, m_instance, app_validation_layer);
}
