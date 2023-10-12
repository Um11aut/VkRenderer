#include "renderer.hpp"

VkRenderer::Renderer::Renderer(Window* window)
	: app_window(window)
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
	// app_instance->appendExtension(.....);
	app_instance->create(); // therefore i need to have a create function

	app_instance->printExtensions(true, false); // required, available
	Logger::printSeparator();
	
	// Intializing debugger for our whole program
	if (app_validation_layer->isEnabled()) {
		app_debugger = std::make_unique<VkRenderer::Debugger>(app_validation_layer, m_variables.m_instance);
	}
	// creating Window surface
	app_surface = std::make_unique<VkRenderer::Surface>(&m_variables, app_window);

	// Creating physical and logical Device
	app_device = std::make_unique<VkRenderer::Device>(&m_variables, app_validation_layer);
}
