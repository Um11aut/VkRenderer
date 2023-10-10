#include "instance.hpp"

#include <stdexcept>
#include <iostream>

void VkRenderer::Instance::createInstance()
{
	if (m_validationLayer->isEnabled()) {
		logger.printOnce("Validation Layers enabled", MessageType::Nothing);

		if (!m_validationLayer->checkValidationLayerSupport()) {
			logger.printOnce("Validation Layer requested, but not available!", MessageType::Error);
			std::terminate();
		}
	}

	m_appInfo->sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	m_appInfo->pApplicationName = m_appName.c_str();
	m_appInfo->applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	m_appInfo->pEngineName = m_engineName.c_str();
	m_appInfo->engineVersion = VK_MAKE_VERSION(1, 0, 0);
	m_appInfo->apiVersion = VK_API_VERSION_1_3;
}

void VkRenderer::Instance::createInfo()
{
	m_instanceInfo->sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	m_instanceInfo->pApplicationInfo = m_appInfo;
}

void VkRenderer::Instance::clean()
{
	vkDestroyInstance(*m_instance, nullptr);
}

void VkRenderer::Instance::enableExtensions()
{
	glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

	m_instanceInfo->enabledExtensionCount = glfwExtensionCount;
	m_instanceInfo->ppEnabledExtensionNames = glfwExtensions;

	if (m_validationLayer->isEnabled()) {

		m_instanceInfo->enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
		m_instanceInfo->ppEnabledLayerNames = validationLayers.data();

		Debugger::populate(debugCreateInfo);
		m_instanceInfo->pNext = (VkDebugUtilsMessengerCreateInfoEXT*) &debugCreateInfo;
	}
	else {
		m_instanceInfo->enabledLayerCount = 0;

		m_instanceInfo->pNext = nullptr;
	}
}

void VkRenderer::Instance::checkInstanceCreation()
{
	if (vkCreateInstance(m_instanceInfo, nullptr, m_instance) != VK_SUCCESS) {
		Logger::printOnce("Failed to create Vulkan Instance", MessageType::Error);
		std::terminate();
	}
	else {
		logger.print({"Created Instance! Application Name: ", m_appInfo->pApplicationName}, MessageType::Success);
	}
}

void VkRenderer::Instance::getRequiredExtensions()
{
	for (uint32_t i = 0; i < glfwExtensionCount; i++) {
		requiredExtensions.emplace_back(glfwExtensions[i]);
	}

	requiredExtensions.emplace_back(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);
	updateExtensions();
}

void VkRenderer::Instance::getAvailableExtensions()
{
	vkEnumerateInstanceExtensionProperties(nullptr, &availableExtensionsCount, nullptr);

	availibleExtensions.resize(availableExtensionsCount);

	vkEnumerateInstanceExtensionProperties(nullptr, &availableExtensionsCount, availibleExtensions.data());
}

void VkRenderer::Instance::updateExtensions()
{
	m_instanceInfo->flags |= VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;

	m_instanceInfo->enabledExtensionCount = static_cast<uint32_t>(requiredExtensions.size());
	m_instanceInfo->ppEnabledExtensionNames = requiredExtensions.data();
}

void VkRenderer::Instance::appendExtension(const char* extensionName)
{
	requiredExtensions.push_back(extensionName);
	updateExtensions();
}

void VkRenderer::Instance::create()
{
	checkInstanceCreation();
}

void VkRenderer::Instance::destroy()
{
	clean();
}

VkRenderer::Instance::Instance(std::string appName, std::string engineName,
	VkInstance* instance, VkInstanceCreateInfo* instanceInfo,
	VkApplicationInfo* appInfo, std::shared_ptr<VkRenderer::ValidationLayer> validationLayer)

	: m_appName(appName), m_engineName(engineName), m_instance(instance),
	m_instanceInfo(instanceInfo), m_appInfo(appInfo), m_validationLayer(validationLayer)
{
	createInstance();
	createInfo();
	enableExtensions();
	getRequiredExtensions();
	getAvailableExtensions();
}

VkRenderer::Instance::~Instance()
{
	clean();
}

void VkRenderer::Instance::printExtensions(bool printRequired, bool printAvailable)
{
	if (printRequired) {
		logger.printSeparator();

		logger.printOnce("Used extensions: ", MessageType::Nothing);
		for (const auto& extension : requiredExtensions) {
			logger.printOnce(extension, MessageType::Nothing);
		}
	}
	if (printAvailable) {
		logger.printSeparator();

		logger.printOnce("Available extensions: ", MessageType::Nothing);
		for (const auto& extension : availibleExtensions) {
			logger.printOnce(extension.extensionName, MessageType::Nothing);
		}
	}
}
