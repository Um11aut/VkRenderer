#include "application.hpp"

App::App(const int w_width, const int w_height) 
	: window_width(w_width), window_height(w_height)
{
	initVk();
}

void App::run()
{
	while (!app_window->shouldClose() && !app_window->KeyPressed(GLFW_KEY_ESCAPE)) {
		glfwPollEvents();
	}
}

void App::initVk()
{
	app_window = std::make_shared<VkRenderer::Window>(window_width, window_height);
	app_validation_layer = std::make_shared<VkRenderer::ValidationLayer>(enableValidationLayer);

	app_instance = 
		std::make_unique<VkRenderer::Instance>(
			"Renderer", "Wolfram",
			&m_instance, &m_instanceInfo,
			&m_appInfo, app_validation_layer);

	if (app_validation_layer->isEnabled()) {
		app_instance->appendExtension(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
	}
	app_instance->create();
	app_instance->printExtensions(true, false);
	
	if (app_validation_layer->isEnabled()) {
		app_debugger = std::make_unique<VkRenderer::Debugger>(app_validation_layer, &m_instance);
	}

	app_device = std::make_unique<VkRenderer::Device>(&m_physicalDevice, &m_device, m_instance, app_validation_layer);
}
