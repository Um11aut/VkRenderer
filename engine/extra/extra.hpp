#pragma once
#include <string>
#include <optional>
#include <vulkan/vulkan.h>

namespace Extra {
	struct AppInfo {
		std::string appName;
		std::string engineName;
		int windowWidth, windowHeight;
		bool GUI;
	};

	struct VkVars {
		VkInstance m_instance;
		VkPhysicalDevice m_physicalDevice;
		VkDevice m_device;
		VkSurfaceKHR m_surface;
		VkQueue m_presentQueue;
		VkQueue m_graphicsQueue;


		VkVars& operator=(const VkVars& other) {
			this->m_instance = other.m_instance;
			this->m_physicalDevice = other.m_physicalDevice;
			this->m_device = other.m_device;
			this->m_surface = other.m_surface;
			this->m_presentQueue = other.m_presentQueue;
			this->m_graphicsQueue = other.m_graphicsQueue;
		}
	};

	struct QueueFamilyIndices {
		std::optional<uint32_t> graphicsFamily;
		std::optional<uint32_t> presentFamily;
	};
}