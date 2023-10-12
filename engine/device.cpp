#include "device.hpp"

bool VkRenderer::Device::isDeviceSuitable(VkPhysicalDevice device)
{
	VkPhysicalDeviceProperties deviceProperties;
	VkPhysicalDeviceFeatures deviceFeatures;

	vkGetPhysicalDeviceProperties(device, &deviceProperties);
	vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

	return deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU && deviceFeatures.geometryShader;
}

void VkRenderer::Device::getDeviceProperties()
{
	if (&m_vars->m_physicalDevice != nullptr) {
		vkGetPhysicalDeviceProperties(m_vars->m_physicalDevice, &m_deviceProperties);
	}
}

void VkRenderer::Device::getDeviceFeatures()
{
	if (&m_vars->m_physicalDevice != nullptr) {
		vkGetPhysicalDeviceFeatures(m_vars->m_physicalDevice, &m_deviceFeatures);
	}
}

void VkRenderer::Device::createLogicalDevice()
{
	if (&m_vars->m_physicalDevice == nullptr) {
		return;
	}

	m_physicalDeviceIndices = findSupportedQueueFamilies(m_vars->m_physicalDevice);

	uniqueQueueFamilies = { m_physicalDeviceIndices.graphicsFamily.has_value(), m_physicalDeviceIndices.presentFamily.value() };

	float m_queuePriority = 1.0f;
	for (uint32_t queueFamily : uniqueQueueFamilies) {
		VkDeviceQueueCreateInfo queueCreateInfo{};
		queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queueCreateInfo.queueFamilyIndex = queueFamily;
		queueCreateInfo.queueCount = 1;
		queueCreateInfo.pQueuePriorities = &m_queuePriority;
		m_queueCreateInfos.push_back(queueCreateInfo);
	}
	
	m_deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	m_deviceCreateInfo.queueCreateInfoCount = static_cast<uint32_t>(m_queueCreateInfos.size());
	m_deviceCreateInfo.pQueueCreateInfos = m_queueCreateInfos.data();

	getDeviceFeatures();
	m_deviceCreateInfo.pEnabledFeatures = &m_deviceFeatures;

	m_deviceCreateInfo.enabledExtensionCount = 0;

	if (m_validationLayer->isEnabled()) {
		m_deviceCreateInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
		m_deviceCreateInfo.ppEnabledLayerNames = validationLayers.data();
	}
	else {
		m_deviceCreateInfo.enabledLayerCount = 0;
	}

	if (vkCreateDevice(m_vars->m_physicalDevice, &m_deviceCreateInfo, nullptr, &m_vars->m_device) != VK_SUCCESS) {
		Logger::printOnce("failed to create logical Device", MessageType::Error);
	}
	else {
		Logger::printOnce("Created Logical Device!", MessageType::Success);
	}


	vkGetDeviceQueue(m_vars->m_device, m_physicalDeviceIndices.presentFamily.value(), 0, &m_vars->m_presentQueue);
	vkGetDeviceQueue(m_vars->m_device, m_physicalDeviceIndices.graphicsFamily.value(), 0, &m_vars->m_graphicsQueue);
}

VkRenderer::Device::Device(
	Extra::VkVars* vars, std::shared_ptr<VkRenderer::ValidationLayer> validationLayer)
	: m_vars(vars), m_validationLayer(validationLayer)
{
	uint32_t device_count;
	vkEnumeratePhysicalDevices(m_vars->m_instance, &device_count, nullptr);
	if (device_count == 0) {
		Logger::printOnce("There are no suitable physical device to load!", MessageType::Error);
		std::terminate();
	}

	m_availableDevices.resize(device_count);

	vkEnumeratePhysicalDevices(m_vars->m_instance, &device_count, m_availableDevices.data());

	pickDevice();

	createLogicalDevice();
}

Extra::QueueFamilyIndices VkRenderer::Device::findSupportedQueueFamilies(VkPhysicalDevice device)
{
	Extra::QueueFamilyIndices indicies;
	uint32_t queueFamilyCount;

	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);


	std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

	int count = 0;

	for (const auto& queueFamilie : queueFamilies) {
		if (queueFamilie.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
			indicies.graphicsFamily = count;
		}
		
		VkBool32 presentSupport = false;
		vkGetPhysicalDeviceSurfaceSupportKHR(device, count, m_vars->m_surface, &presentSupport);

		if (presentSupport) {
			indicies.presentFamily = count;
		}

		if (indicies.graphicsFamily.has_value() && indicies.presentFamily.has_value()) {
			break;
		}

		count++;
	}

	return indicies;
}

int VkRenderer::Device::rateDevice(VkPhysicalDevice device)
{
	VkPhysicalDeviceProperties deviceProperties;
	VkPhysicalDeviceFeatures deviceFeatures;
	vkGetPhysicalDeviceProperties(device, &deviceProperties);
	vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

	int score = 0;

	if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
		score += 1000;
	}

	score += deviceProperties.limits.maxImageDimension2D;
	score += deviceProperties.limits.maxImageDimension3D;

	if (!deviceFeatures.geometryShader) {
		return 0;
	}

	return score;
}

void VkRenderer::Device::pickDevice()
{
	std::multimap<int, VkPhysicalDevice> candidates;

	for (const auto& device : m_availableDevices) {
		candidates.insert(std::make_pair(rateDevice(device), device));
	}

	for (const auto& candidate : candidates) {
		if (!findSupportedQueueFamilies(candidate.second).graphicsFamily.has_value()) {
			continue;
		}
		VkPhysicalDeviceProperties deviceProperties;
		vkGetPhysicalDeviceProperties(candidate.second, &deviceProperties);

		Logger::print({ "Candidate GPU: ", deviceProperties.deviceName, " | GPU score: ", std::to_string(candidate.first)}, MessageType::Nothing);
	}

	if (candidates.rbegin()->first > 0) {
		m_vars->m_physicalDevice = candidates.rbegin()->second;
		getDeviceProperties();
		Logger::print({ "Picked GPU!: ", m_deviceProperties.deviceName}, MessageType::Success);
	}
	else {
		Logger::printOnce("Failed to pick suitable GPU", MessageType::Error);
		std::terminate();
	}
}

VkRenderer::Device::~Device()
{
	vkDestroyDevice(m_vars->m_device, nullptr);
}
