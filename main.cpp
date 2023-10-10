#include <iostream>
#include "engine/application.hpp"

int main() {
	VkRenderer::App renderer(1920, 1080);

	renderer.run();
}