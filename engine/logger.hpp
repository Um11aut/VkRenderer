#pragma once

#include <iostream>
#include <stdexcept>
#include <vector>

#define RED_TEXT "\033[1;31m"
#define YELLOW_TEXT "\033[1;33m"
#define GREEN_TEXT "\033[1;32m"
#define BLUE_TEXT "\033[1;34m"

#define RESET_COLOR "\033[0m"

namespace VkRenderer {
	enum MessageType {
		Error = -1,
		Warning = 0,
		Success = 1,

		Nothing = 2
	};

	struct Logger {
		static void print(std::vector<std::string> messages, MessageType type = MessageType::Nothing);
		static void printOnce(std::string msg, MessageType type = MessageType::Nothing);
		static void printSeparator();
	};

	struct DebugLogger {
		static void print(std::string msg, MessageType type = MessageType::Nothing);
	};
}