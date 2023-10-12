#include <vulkan/vulkan.h>

#include <vector>
#include <map>
#include <string>
#include <optional>
#include <memory>
#include <set>

#include "logger.hpp"
#include "validation_layers.hpp"
#include "extra/extra.hpp"

namespace VkRenderer {
	class Device {
	private:
		VkPhysicalDevice* m_physicalDevice = VK_NULL_HANDLE;

		VkPhysicalDeviceProperties m_deviceProperties;
		VkPhysicalDeviceFeatures m_deviceFeatures;

		VkInstance* m_instance;

		std::vector<VkPhysicalDevice> m_availableDevices;

		VkDevice* m_device;
		VkDeviceCreateInfo m_deviceCreateInfo{};

		Extra::QueueFamilyIndices m_physicalDeviceIndices{};


		std::shared_ptr<VkRenderer::ValidationLayer> m_validationLayer;

		VkQueue* m_presentQueue;
		VkQueue* m_graphicsQueue;
		VkSurfaceKHR* m_surface;
		
		std::vector<VkDeviceQueueCreateInfo> m_queueCreateInfos;
		std::set<uint32_t> uniqueQueueFamilies;


		void createLogicalDevice();
		void getDeviceProperties();
		void getDeviceFeatures();
		Extra::QueueFamilyIndices findSupportedQueueFamilies(VkPhysicalDevice device);
		bool isDeviceSuitable(VkPhysicalDevice device);

		int rateDevice(VkPhysicalDevice device);
		void pickDevice();
	public:
		Device(VkPhysicalDevice* physicalDevice,
			VkDevice* device,
			VkQueue* presentQueue,
			VkQueue* graphicsQueue,
			VkSurfaceKHR* surface,
			VkInstance instance,
			std::shared_ptr<VkRenderer::ValidationLayer> validationLayer);
		~Device();
	};
}