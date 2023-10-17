#pragma once
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
		Extra::VkVars* m_vars;

		VkPhysicalDeviceProperties m_deviceProperties{};
		VkPhysicalDeviceFeatures m_deviceFeatures{};

		std::vector<VkPhysicalDevice> m_availableDevices;

		VkDeviceCreateInfo m_deviceCreateInfo{};

		Extra::QueueFamilyIndices m_physicalDeviceIndices{};

		std::shared_ptr<VkRenderer::ValidationLayer> m_validationLayer;
		std::vector<const char*> m_deviceExtensions;
		std::vector<VkDeviceQueueCreateInfo> m_queueCreateInfos;
		std::set<uint32_t> uniqueQueueFamilies;
		
		void getDeviceProperties();
		void getDeviceFeatures();
		bool isDeviceSuitable(VkPhysicalDevice device);

		int rateDevice(VkPhysicalDevice device);
		void pickDevice();
	public:
		Device(Extra::VkVars* vars, std::shared_ptr<VkRenderer::ValidationLayer> validationLayer);
		static Extra::QueueFamilyIndices findSupportedQueueFamilies(VkPhysicalDevice device, VkSurfaceKHR surface);

		void appendExtension(const char* extensionName);

		void create();
		void destoy();

	};
}