#pragma once
#include <vector>
#include <limits>
#include <memory>
#include <algorithm>
#include <iostream>
#include <glfw/glfw3.h>
#include "device.hpp"
#include "extra/extra.hpp"
#include "window.hpp"
#include "logger.hpp"

namespace VkRenderer {
	struct SwapChainDetails {
		VkSurfaceCapabilitiesKHR capabilities;
		std::vector<VkSurfaceFormatKHR> formats;
		std::vector<VkPresentModeKHR> presentModes;
	};
	class SwapChain {
	private:
		Extra::VkVars* m_vars;

		VkPresentModeKHR m_presentMode{};
		VkSwapchainCreateInfoKHR createInfo{};
		uint32_t imageCount = 0;

		std::shared_ptr<VkRenderer::Window> m_window;

		SwapChainDetails getSwapChainSupportDetails();

		VkSurfaceFormatKHR getSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& formats);
		VkExtent2D getSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
		VkPresentModeKHR getSwapPresentMode(const std::vector<VkPresentModeKHR>& presentModes);
	public:
		SwapChain(Extra::VkVars* vars, std::shared_ptr<VkRenderer::Window> window);

		void setPresentMode(const Extra::PresentMode presentMode);

		void create();

		~SwapChain();
	};
}