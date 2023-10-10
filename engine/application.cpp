#include "application.hpp"

VkRenderer::App::App(const int w_width, const int w_height) 
	: window_width(w_width), window_height(w_height)
{
	initVk();
}

void VkRenderer::App::run()
{
	while (!app_window->shouldClose() && !app_window->KeyPressed(GLFW_KEY_ESCAPE)) {
		glfwPollEvents();
	}
}

void VkRenderer::App::initVk()
{
	app_window = std::make_shared<VkRenderer::Window>(window_width, window_height);
	app_validation_layer = std::make_shared<VkRenderer::ValidationLayer>(enableValidationLayer);

	app_instance = 
		std::make_unique<VkRenderer::Instance>(
			"Renderer", "Wolfram",
			&m_instance, &m_instanceInfo,
			&m_appInfo, app_validation_layer);

	if (enableValidationLayer) {
		app_instance->appendExtension(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
	}
	app_instance->create();
	app_instance->printExtensions(true, false);

	app_debugger = std::make_unique<VkRenderer::Debugger>(app_validation_layer, &m_instance);

	app_physical_device = std::make_unique<VkRenderer::PhysicalDevise>(m_device, &m_instance);
}
