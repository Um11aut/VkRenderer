#include "debugger.hpp"

VKAPI_ATTR VkBool32 VKAPI_CALL VkRenderer::Debugger::debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData)
{
	switch (messageSeverity) {
	case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
		DebugLogger::print(pCallbackData->pMessage, MessageType::Warning);
		break;
	case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
		DebugLogger::print(pCallbackData->pMessage, MessageType::Error);
		std::terminate();
		break;
	default:
		DebugLogger::print(pCallbackData->pMessage);
		break;
	}

	return VK_FALSE;
}

VkResult VkRenderer::Debugger::CreateDebugUtilsMessengerEXT()
{
	auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(*m_instance, "vkCreateDebugUtilsMessengerEXT");
	if (func != nullptr) {
		return func(*m_instance, &m_createInfo, nullptr, &m_debugMessenger);
	}
	else {
		return VK_ERROR_EXTENSION_NOT_PRESENT;
	}
}
void VkRenderer::Debugger::DestroyDebugUtilsMessengerEXT() {
	auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(*m_instance, "vkDestroyDebugUtilsMessengerEXT");
	if (func != nullptr) {
		func(*m_instance, m_debugMessenger, nullptr);
	}
}

void VkRenderer::Debugger::populate(VkDebugUtilsMessengerCreateInfoEXT& createInfo) {
	createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
	createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
	createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
	createInfo.pfnUserCallback = debugCallback;
}


VkRenderer::Debugger::Debugger(
	std::shared_ptr<VkRenderer::ValidationLayer> validation_layer,
	VkInstance& instance)
	
	: m_validationLayer(validation_layer), m_instance(&instance)
{
	setupDebugMessenger();
}
void VkRenderer::Debugger::destroy()
{
	if (m_validationLayer->isEnabled()) {
		DestroyDebugUtilsMessengerEXT();
	}
}

void VkRenderer::Debugger::setupDebugMessenger()
{
	if (!m_validationLayer->isEnabled()) return;

	populate(m_createInfo);

	if (CreateDebugUtilsMessengerEXT() != VK_SUCCESS) {
		Logger::printOnce("Failed to set up debug messenger", MessageType::Error);
		std::terminate();
	}
}
