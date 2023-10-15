#include "renderer.hpp"

VkRenderer::Renderer::Renderer(Window* window)
	: app_window(window)
{
}

void VkRenderer::Renderer::run()
{
	while (!app_window->shouldClose() && !app_window->KeyPressed(GLFW_KEY_ESCAPE)) {
		glfwPollEvents();
	}
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
	
	// Intializing debugger for our whole program
	if (app_validation_layer->isEnabled()) {
		app_debugger = std::make_unique<VkRenderer::Debugger>(app_validation_layer, m_variables.m_instance);
	}
	// creating Window surface
	app_surface = std::make_unique<VkRenderer::Surface>(&m_variables, app_window);

	// Creating physical and logical Device
	app_device = std::make_unique<VkRenderer::Device>(&m_variables, app_validation_layer);
	app_device->appendExtension(VK_KHR_SWAPCHAIN_EXTENSION_NAME);
	app_device->create();

	// Create Swap Chain
	app_swapChain = std::make_unique<VkRenderer::SwapChain>(&m_variables, app_window);
	app_swapChain->setPresentMode(Extra::MailBox);
	app_swapChain->create();

	ShaderModule shader{&m_variables};

	app_renderPass = std::make_unique<VkRenderer::RenderPass>(&m_variables, app_swapChain);

	m_mainPipeline = std::make_unique<VkRenderer::GraphicsPipeline>(&m_variables, shader, app_swapChain);
}
