#pragma once
#include <string>

namespace Extra {
	struct AppInfo {
		std::string appName;
		std::string engineName;
		int windowWidth, windowHeight;
		bool GUI;
	};
	struct QueueFamilyIndices {
		std::optional<uint32_t> graphicsFamily;
		std::optional<uint32_t> presentFamily;
	};
}