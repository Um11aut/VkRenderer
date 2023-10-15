#include "renderer.hpp"
#include "window.hpp"
#include "extra/extra.hpp"
#include <thread>

namespace Application {
	VkRenderer::Window* window;
	VkRenderer::Renderer* renderer;
	Extra::AppInfo info;

	void create() {
		info.appName = "Wolfram";
		info.engineName = "Wolfram";
		info.GUI = true;
		info.windowWidth = 1920;
		info.windowHeight = 1080;

		window = new VkRenderer::Window(info);
		renderer = new VkRenderer::Renderer(window);

		std::thread vkInit(&VkRenderer::Renderer::init, renderer);
		vkInit.join();
	}

	void run() {
		renderer->run();
		delete renderer;
	}
}