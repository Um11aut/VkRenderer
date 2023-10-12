#pragma once
#include <string>

namespace Extra {
	struct AppInfo {
		std::string appName;
		std::string engineName;
		int windowWidth, windowHeight;
		bool GUI;
	};
}