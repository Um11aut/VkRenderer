#include "physical_device.hpp"

bool VkRenderer::PhysicalDevise::isDeviceSuitable(VkPhysicalDevice device)
{
	VkPhysicalDeviceProperties deviceProperties;
	VkPhysicalDeviceFeatures deviceFeatures;

	vkGetPhysicalDeviceProperties(device, &deviceProperties);
	vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

	return deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU && deviceFeatures.geometryShader;
}

VkRenderer::PhysicalDevise::PhysicalDevise(
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

	for (const auto& device : m_devices) {
		if (isDeviceSuitable(device)) {
			*m_device = device;
		}
	}

	if (*m_device == VK_NULL_HANDLE) {
		Logger::printOnce("Failed to find GPU!", MessageType::Error);
		std::terminate();
	}
}

VkRenderer::PhysicalDevise::~PhysicalDevise()
{

}
