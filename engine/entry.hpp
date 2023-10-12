#include "renderer.hpp"
#include "extra/extra.hpp"

namespace Application {
	VkRenderer::Renderer* renderer;
	Extra::AppInfo info;

	void create() {
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