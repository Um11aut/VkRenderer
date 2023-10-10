#include "logger.hpp"

void VkRenderer::Logger::print(std::vector<std::string> messages, MessageType type)
{
	switch (type)
	{
	case VkRenderer::Error:
		std::cout << RED_TEXT << "[ERROR] " << RESET_COLOR;
		for (const auto& msg : messages) {
			std::cerr << RED_TEXT << msg << RESET_COLOR;
		}
		std::cout << std::endl;
		break;
	case VkRenderer::Warning:
		std::cout << YELLOW_TEXT << "[WARNING] " << RESET_COLOR;
		for (const auto& msg : messages) {
			std::cout << YELLOW_TEXT << msg << RESET_COLOR;
		}
		std::cout<<std::endl;
		break;
	case VkRenderer::Success:
		std::cout << GREEN_TEXT << "[SUCCESS] " << RESET_COLOR;
		for (const auto& msg : messages) {
			std::cout << GREEN_TEXT << msg << RESET_COLOR;
		}
		std::cout<<std::endl;
		break;
	case VkRenderer::Nothing:
		std::cout << "[MESSAGE] ";
		for (const auto& msg : messages) {
			std::cout << msg;
		}
		std::cout<<std::endl;
		break;
	default:
		break;
	}
}

void VkRenderer::Logger::printOnce(std::string msg, MessageType type)
{
	switch (type)
	{
	case VkRenderer::Error:
		std::cout << RED_TEXT << "[ERROR] " << msg << RESET_COLOR;
		std::cout << std::endl;
		break;
	case VkRenderer::Warning:
		std::cout << YELLOW_TEXT << "[WARNING] " << msg << RESET_COLOR;
		std::cout << std::endl;
		break;
	case VkRenderer::Success:
		std::cout << GREEN_TEXT << "[SUCCESS] " << msg << RESET_COLOR;
		std::cout << std::endl;
		break;
	case VkRenderer::Nothing:
		std::cout << "[MESSAGE] " << msg;
		std::cout << std::endl;
		break;
	default:
		break;
	}
}

void VkRenderer::Logger::printSeparator()
{
	std::cout << "---------------------------------------" << std::endl;
}

void VkRenderer::DebugLogger::print(
	std::string msg,
	MessageType type)
{
	switch (type)
	{
	case VkRenderer::Error:
		std::cout << RED_TEXT << "[VALIDATION LAYER] " << msg << RESET_COLOR;
		std::cout << std::endl;
		break;
	case VkRenderer::Warning:
		std::cout << YELLOW_TEXT << "[VALIDATION LAYER] " << msg << RESET_COLOR;
		std::cout << std::endl;
		break;
	case VkRenderer::Nothing:
		std::cout << BLUE_TEXT << "[VALIDATION LAYER] " << msg << RESET_COLOR;
		std::cout << std::endl;
		break;
	default:
		break;
	}
}
