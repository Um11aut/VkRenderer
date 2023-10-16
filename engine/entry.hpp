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
		info.windowWidth = 800;
		info.windowHeight = 600;

		window = new VkRenderer::Window(info);
		renderer = new VkRenderer::Renderer(window);

	}

	void run() {
		renderer->init();
		renderer->run();
		delete renderer;
	}
}