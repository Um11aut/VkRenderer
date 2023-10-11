#include <vulkan/vulkan.h>

#include <vector>
#include <map>
#include <string>
#include <optional>

#include "logger.hpp"

namespace {
	struct QueueFamilyIndices {
		std::optional<uint32_t> graphicsFamily;
	};
}

namespace VkRenderer {
	class PhysicalDevice {
	private:
		VkPhysicalDeviceProperties m_deviceProperties;
		VkPhysicalDeviceFeatures m_deviceFeatures;

		VkPhysicalDevice* m_device = VK_NULL_HANDLE;

		void getDeviceProperties();
		void getDeviceFeatures();

		VkInstance* m_instance;

		uint32_t m_device_count;

		std::vector<VkPhysicalDevice> m_devices;

		QueueFamilyIndices findSupportedQueueFamilies(VkPhysicalDevice device);

		bool isDeviceSuitable(VkPhysicalDevice device);

		int rateDevice(VkPhysicalDevice device);
		void pickDevice();
	public:
		PhysicalDevice(VkPhysicalDevice device, VkInstance* instance);
		~PhysicalDevice();
	};
}