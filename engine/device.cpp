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
	if (m_physicalDevice != nullptr) {
		vkGetPhysicalDeviceProperties(*m_physicalDevice, &m_deviceProperties);
	}
}

void VkRenderer::Device::getDeviceFeatures()
{
	if (m_physicalDevice != nullptr) {
		vkGetPhysicalDeviceFeatures(*m_physicalDevice, &m_deviceFeatures);
	}
}

void VkRenderer::Device::createLogicalDevice()
{
	if (m_physicalDevice == nullptr) {
		return;
	}

	m_physicalDeviceIndices = findSupportedQueueFamilies(*m_physicalDevice);

	m_queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
	m_queueCreateInfo.queueFamilyIndex = m_physicalDeviceIndices.graphicsFamily.value();
	m_queueCreateInfo.queueCount = 1;
	m_queueCreateInfo.pQueuePriorities = &m_queuePriority;
	
	m_deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	m_deviceCreateInfo.pQueueCreateInfos = &m_queueCreateInfo;
	m_deviceCreateInfo.queueCreateInfoCount = 1;

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

	if (vkCreateDevice(*m_physicalDevice, &m_deviceCreateInfo, nullptr, m_device) != VK_SUCCESS) {
		Logger::printOnce("failed to create logical Device", MessageType::Error);
	}
	else {
		Logger::printOnce("Created Logical Device!", MessageType::Success);
	}
}

VkRenderer::Device::Device(
	VkPhysicalDevice* physicalDevice, VkDevice* device, VkInstance instance, std::shared_ptr<VkRenderer::ValidationLayer> validationLayer)
	: m_physicalDevice(physicalDevice), m_device(device), m_instance(&instance), m_validationLayer(validationLayer)
{
	vkEnumeratePhysicalDevices(*m_instance, &m_device_count, nullptr);
	if (m_device_count == 0) {
		Logger::printOnce("There are no suitable physical device to load!", MessageType::Error);
		std::terminate();
	}

	m_devices.resize(m_device_count);

	vkEnumeratePhysicalDevices(*m_instance, &m_device_count, m_devices.data());

	pickDevice();

	createLogicalDevice();
}

QueueFamilyIndices VkRenderer::Device::findSupportedQueueFamilies(VkPhysicalDevice device)
{
	QueueFamilyIndices indicies;
	uint32_t queueFamilyCount;

	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);


	std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

	int count = 0;

	for (const auto& queueFamilie : queueFamilies) {
		if (queueFamilie.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
			indicies.graphicsFamily = count;
		}

		if (indicies.graphicsFamily.has_value()) {
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

	if (!deviceFeatures.geometryShader) {
		return 0;
	}

	return score;
}

void VkRenderer::Device::pickDevice()
{
	std::multimap<int, VkPhysicalDevice> candidates;

	for (const auto& device : m_devices) {
		candidates.insert(std::make_pair(rateDevice(device), device));
	}

	Logger::printSeparator();
	for (const auto& candidate : candidates) {
		if (!findSupportedQueueFamilies(candidate.second).graphicsFamily.has_value()) {
			continue;
		}
		VkPhysicalDeviceProperties deviceProperties;
		vkGetPhysicalDeviceProperties(candidate.second, &deviceProperties);

		Logger::print({ "Candidate GPU: ", deviceProperties.deviceName, " | GPU score: ", std::to_string(candidate.first)}, MessageType::Nothing);
	}

	if (candidates.rbegin()->first > 0) {
		*m_physicalDevice = candidates.rbegin()->second;
		getDeviceProperties();
		Logger::print({ "picked GPU: ", m_deviceProperties.deviceName}, MessageType::Success);
	}
	else {
		Logger::printOnce("Failed to pick suitable GPU", MessageType::Error);
		std::terminate();
	}
}

VkRenderer::Device::~Device()
{
	vkDestroyDevice(*m_device, nullptr);
}
