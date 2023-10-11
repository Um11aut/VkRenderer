#include <vulkan/vulkan.h>

#include <vector>
#include <map>
#include <string>
#include <optional>
#include <memory>

#include "logger.hpp"
#include "validation_layers.hpp"

namespace {
	struct QueueFamilyIndices {
		std::optional<uint32_t> graphicsFamily;
	};
}

namespace VkRenderer {
	class Device {
	private:
		VkPhysicalDeviceProperties m_deviceProperties;
		VkPhysicalDeviceFeatures m_deviceFeatures;

		VkPhysicalDevice* m_physicalDevice = VK_NULL_HANDLE;

		void getDeviceProperties();
		void getDeviceFeatures();

		VkInstance* m_instance;

		uint32_t m_device_count;
		std::vector<VkPhysicalDevice> m_devices;

		QueueFamilyIndices findSupportedQueueFamilies(VkPhysicalDevice device);

		bool isDeviceSuitable(VkPhysicalDevice device);

		int rateDevice(VkPhysicalDevice device);
		void pickDevice();

		VkDevice* m_device;
		VkDeviceQueueCreateInfo m_queueCreateInfo{};
		VkDeviceCreateInfo m_deviceCreateInfo{};
		QueueFamilyIndices m_physicalDeviceIndices{};

		VkQueue m_graphicsQueue;
		float m_queuePriority = 1.0f;

		std::shared_ptr<VkRenderer::ValidationLayer> m_validationLayer;

		void createLogicalDevice();
	public:
		Device(VkPhysicalDevice* physicalDevice,
			VkDevice* device,
			VkInstance instance,
			std::shared_ptr<VkRenderer::ValidationLayer> validationLayer);
		~Device();
	};
}