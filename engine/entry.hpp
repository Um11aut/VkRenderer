#include "renderer.hpp"

namespace Application {
	VkRenderer::Renderer* renderer;
	VkRenderer::AppInfo info;

	void createApplication() {
		info.appName = "Wolfram";
		info.engineName = "Wolfram";
		info.GUI = true;
		info.windowWidth = 1920;
		info.windowHeight = 1080;

		renderer = new VkRenderer::Renderer(info);
	}

	void run() {
		renderer->run();
		delete renderer;
	}
}