#include "instance.hpp"

#include <stdexcept>
#include <iostream>

void VkRenderer::Instance::createInstance()
{
	if (m_validationLayer->isEnabled()) {
		Logger::printOnce("Validation Layers enabled", MessageType::Nothing);

		if (!m_validationLayer->checkValidationLayerSupport()) {
			Logger::printOnce("Validation Layer requested, but not available!", MessageType::Error);
			std::terminate();
		}
	}

	m_appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	m_appInfo.pApplicationName = m_appName.c_str();
	m_appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	m_appInfo.pEngineName = m_engineName.c_str();
	m_appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
	m_appInfo.apiVersion = VK_API_VERSION_1_3;
}

void VkRenderer::Instance::createInfo()
{
	m_instanceInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	m_instanceInfo.pApplicationInfo = &m_appInfo;
}

void VkRenderer::Instance::enableExtensions()
{
	glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
}

void VkRenderer::Instance::getRequiredExtensions()
{
	for (uint32_t i = 0; i < glfwExtensionCount; i++) {
		requiredExtensions.emplace_back(glfwExtensions[i]);
	}

	requiredExtensions.emplace_back(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);
	m_instanceInfo.flags |= VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;

	// so it can be called without append
}

void VkRenderer::Instance::getAvailableExtensions()
{
	vkEnumerateInstanceExtensionProperties(nullptr, &availableExtensionsCount, nullptr);

	availibleExtensions.resize(availableExtensionsCount);

	vkEnumerateInstanceExtensionProperties(nullptr, &availableExtensionsCount, availibleExtensions.data());
}

void VkRenderer::Instance::useExtensions()
{
	m_instanceInfo.enabledExtensionCount = static_cast<uint32_t>(requiredExtensions.size());
	m_instanceInfo.ppEnabledExtensionNames = requiredExtensions.data();

	if (m_validationLayer->isEnabled()) {

		m_instanceInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
		m_instanceInfo.ppEnabledLayerNames = validationLayers.data();

		Debugger::populate(debugCreateInfo);
		m_instanceInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debugCreateInfo;
	}
	else {
		m_instanceInfo.enabledLayerCount = 0;

		m_instanceInfo.pNext = nullptr;
	}
}

void VkRenderer::Instance::appendExtension(const char* extensionName)
{
	requiredExtensions.push_back(extensionName);
}

void VkRenderer::Instance::create()
{
	useExtensions();
	if (vkCreateInstance(&m_instanceInfo, nullptr, m_instance) != VK_SUCCESS) {
		Logger::printOnce("Failed to create Vulkan Instance", MessageType::Error);
		std::terminate();
	}
	else {
		Logger::print({ "Created Instance! Application Name: ", m_appInfo.pApplicationName }, MessageType::Success);
	}
}

VkRenderer::Instance::Instance(std::string appName, std::string engineName,
	VkInstance* instance, std::shared_ptr<VkRenderer::ValidationLayer> validationLayer)

	: m_appName(appName), m_engineName(engineName), m_instance(instance), m_validationLayer(validationLayer)
{
	createInstance();
	createInfo();
	enableExtensions();
	getRequiredExtensions();
	if (m_validationLayer->isEnabled()) {
		appendExtension(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
	}
}

void VkRenderer::Instance::destroy()
{
	vkDestroyInstance(*m_instance, nullptr);
}

void VkRenderer::Instance::printExtensions(bool printRequired, bool printAvailable)
{
	if (printRequired) {
		Logger::printSeparator();

		Logger::printOnce("Used extensions: ", MessageType::Nothing);
		for (const auto& extension : requiredExtensions) {
			Logger::printOnce(extension, MessageType::Nothing);
		}
	}
	if (printAvailable) {
		getAvailableExtensions();
		Logger::printSeparator();

		Logger::printOnce("Available extensions: ", MessageType::Nothing);
		for (const auto& extension : availibleExtensions) {
			Logger::printOnce(extension.extensionName, MessageType::Nothing);
		}
	}
}
