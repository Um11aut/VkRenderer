#pragma once
#include <string>
#include <optional>
#include <vulkan/vulkan.h>

namespace Extra {
	struct AppInfo {
		std::string appName = "Blank";
		std::string engineName = "Blank";
		int windowWidth = 800, windowHeight = 600;
		bool GUI = true;
	};

	struct VkVars {
		VkInstance m_instance;
		VkPhysicalDevice m_physicalDevice;
		VkDevice m_device;
		VkSurfaceKHR m_surface;
		VkQueue m_presentQueue;
		VkQueue m_graphicsQueue; 
		VkSwapchainKHR m_swapChain;
		VkRenderPass m_renderPass;
	};

	struct QueueFamilyIndices {
		std::optional<uint32_t> graphicsFamily;
		std::optional<uint32_t> presentFamily;
	};

	enum PresentMode {
		Immediate = VK_PRESENT_MODE_IMMEDIATE_KHR,
		Fifo = VK_PRESENT_MODE_FIFO_KHR,
		FifoRelaxed = VK_PRESENT_MODE_FIFO_RELAXED_KHR,
		MailBox = VK_PRESENT_MODE_MAILBOX_KHR
	};
}