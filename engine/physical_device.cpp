#include "physical_device.hpp"

bool VkRenderer::PhysicalDevice::isDeviceSuitable(VkPhysicalDevice device)
{
	VkPhysicalDeviceProperties deviceProperties;
	VkPhysicalDeviceFeatures deviceFeatures;

	vkGetPhysicalDeviceProperties(device, &deviceProperties);
	vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

	return deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU && deviceFeatures.geometryShader;
}

void VkRenderer::PhysicalDevice::getDeviceProperties()
{
	if (m_device != nullptr) {
		vkGetPhysicalDeviceProperties(*m_device, &m_deviceProperties);
	}
}

void VkRenderer::PhysicalDevice::getDeviceFeatures()
{
	if (m_device != nullptr) {
		vkGetPhysicalDeviceFeatures(*m_device, &m_deviceFeatures);
	}
}

VkRenderer::PhysicalDevice::PhysicalDevice(
	VkPhysicalDevice device, VkInstance* instance)
	: m_device(&device), m_instance(instance)
{
	vkEnumeratePhysicalDevices(*m_instance, &m_device_count, nullptr);
	if (m_device_count == 0) {
		Logger::printOnce("There are no suitable physical device to load!", MessageType::Error);
		std::terminate();
	}

	m_devices.resize(m_device_count);

	vkEnumeratePhysicalDevices(*m_instance, &m_device_count, m_devices.data());

	pickDevice();
}

QueueFamilyIndices VkRenderer::PhysicalDevice::findSupportedQueueFamilies(VkPhysicalDevice device)
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

int VkRenderer::PhysicalDevice::rateDevice(VkPhysicalDevice device)
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

void VkRenderer::PhysicalDevice::pickDevice()
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
		*m_device = candidates.rbegin()->second;
		getDeviceProperties();
		Logger::print({ "Successfully picked GPU: ", m_deviceProperties.deviceName}, MessageType::Success);
	}
	else {
		Logger::printOnce("Failed to pick suitable GPU", MessageType::Error);
		std::terminate();
	}
}

VkRenderer::PhysicalDevice::~PhysicalDevice()
{

}
