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
		VkSwapchainCreateInfoKHR m_createInfo{};
		uint32_t imageCount = 0;

		std::shared_ptr<VkRenderer::Window> m_window;

		std::vector<VkImage> m_images;
		std::vector<VkImageView> m_imageViews;
		std::vector<VkFramebuffer> m_frameBuffers;

		VkFormat m_imageFormat;
		VkExtent2D m_swapChainExtent;

		SwapChainDetails getSwapChainSupportDetails();

		VkSurfaceFormatKHR getSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& formats);
		VkExtent2D getSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
		VkPresentModeKHR getSwapPresentMode(const std::vector<VkPresentModeKHR>& presentModes);

		SwapChainDetails swapChainSupport;

		void createImageViews();
	public:
		SwapChain(Extra::VkVars* vars, std::shared_ptr<VkRenderer::Window> window);

		void setPresentMode(const Extra::PresentMode presentMode);

		void create();
		void createFrameBuffers();

		inline VkExtent2D getExtent() const { return m_swapChainExtent; }
		inline VkFormat getFormat() const { return m_imageFormat; }

		inline std::vector<VkImage> getImages() const { return m_images; }

		inline std::vector<VkFramebuffer> getFrameBuffers() const { return m_frameBuffers; }
		inline SwapChainDetails getDetails() const { return swapChainSupport; }

		void recreate();
		void destroy();

	};
}