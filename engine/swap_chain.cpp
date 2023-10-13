#include "swap_chain.hpp"

VkSurfaceFormatKHR VkRenderer::SwapChain::getSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& formats)
{
	for (const auto& format : formats) {
		if (format.format == VK_FORMAT_B8G8R8A8_SRGB && format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
			return format;
		}
	}

	return formats[0];
}

VkExtent2D VkRenderer::SwapChain::getSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities)
{
	if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
		return capabilities.currentExtent;
	}
	else {
		int w, h;
		glfwGetFramebufferSize(m_window->getWindow(), &w, &h);

		VkExtent2D actualExtent = {
			static_cast<uint32_t>(w),
			static_cast<uint32_t>(h)
		};
		actualExtent.width = std::clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
		actualExtent.height = std::clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

		return actualExtent;
	}
}

VkPresentModeKHR VkRenderer::SwapChain::getSwapPresentMode(const std::vector<VkPresentModeKHR>& presentModes)
{
	return m_presentMode;
}

VkRenderer::SwapChainDetails VkRenderer::SwapChain::getSwapChainSupportDetails()
{
	SwapChainDetails details;

	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(m_vars->m_physicalDevice, m_vars->m_surface, &details.capabilities);

	uint32_t formatCount;
	vkGetPhysicalDeviceSurfaceFormatsKHR(m_vars->m_physicalDevice, m_vars->m_surface, &formatCount, nullptr);

	if (formatCount != 0) {
		details.formats.resize(formatCount);

		vkGetPhysicalDeviceSurfaceFormatsKHR(m_vars->m_physicalDevice, m_vars->m_surface, &formatCount, details.formats.data());
	}

	uint32_t presentModeCount;
	vkGetPhysicalDeviceSurfacePresentModesKHR(m_vars->m_physicalDevice, m_vars->m_surface, &presentModeCount, nullptr);

	if (presentModeCount != 0) {
		details.presentModes.resize(presentModeCount);
		vkGetPhysicalDeviceSurfacePresentModesKHR(m_vars->m_physicalDevice, m_vars->m_surface, &presentModeCount, details.presentModes.data());
	}

	return details;
}

VkRenderer::SwapChain::SwapChain(Extra::VkVars* vars, std::shared_ptr<VkRenderer::Window> window)
	: m_vars(vars), m_window(window)
{
}

void VkRenderer::SwapChain::setPresentMode(Extra::PresentMode presentMode)
{
	switch (presentMode) {
	case Extra::Fifo:
		Logger::printOnce("SwapChain Present Mode: Fifo");
		break;
	case Extra::FifoRelaxed:
		Logger::printOnce("SwapChain Present Mode: Fifo Relaxed");
		break;
	case Extra::MailBox:
		Logger::printOnce("SwapChain Present Mode: Mailbox");
		break;
	case Extra::Immediate:
		Logger::printOnce("SwapChain Present Mode: Immediate");
		break;
	}
	m_presentMode = static_cast<VkPresentModeKHR>(presentMode);
}

void VkRenderer::SwapChain::create()
{
	SwapChainDetails swapChainSupport = getSwapChainSupportDetails();

	VkSurfaceFormatKHR surfaceFormat = getSwapSurfaceFormat(swapChainSupport.formats);
	VkPresentModeKHR presentMode = getSwapPresentMode(swapChainSupport.presentModes);
	VkExtent2D extent = getSwapExtent(swapChainSupport.capabilities);

	imageCount = swapChainSupport.capabilities.minImageCount + 1;

	Logger::print({ "Minimum available SwapChain image count: ", std::to_string(swapChainSupport.capabilities.minImageCount) });
	Logger::print({ "Maximum available SwapChain image count: ", std::to_string(swapChainSupport.capabilities.maxImageCount) });

	if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount) {
		imageCount = swapChainSupport.capabilities.maxImageCount;
	}

	createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	createInfo.surface = m_vars->m_surface;
	createInfo.minImageCount = imageCount;
	createInfo.imageFormat = surfaceFormat.format;
	createInfo.imageColorSpace = surfaceFormat.colorSpace;
	createInfo.imageExtent = extent;
	createInfo.imageArrayLayers = 1;
	createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

	Extra::QueueFamilyIndices indices = Device::findSupportedQueueFamilies(m_vars->m_physicalDevice, m_vars->m_surface);

	uint32_t queueFamilyIndices[] = { indices.graphicsFamily.value(), indices.presentFamily.value() };

	if (indices.graphicsFamily != indices.presentFamily) {
		createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
		createInfo.queueFamilyIndexCount = 2;
		createInfo.pQueueFamilyIndices = queueFamilyIndices;
	}
	else {
		createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
		createInfo.queueFamilyIndexCount = 0; // Optional
		createInfo.pQueueFamilyIndices = nullptr; // Optional
	}

	createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
	createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	createInfo.presentMode = presentMode;
	createInfo.clipped = VK_TRUE;
	createInfo.oldSwapchain = VK_NULL_HANDLE;

	if (vkCreateSwapchainKHR(m_vars->m_device, &createInfo, nullptr, &m_vars->m_swapChain) != VK_SUCCESS) {}
	else {
		Logger::printOnce("Created SwapChain!", MessageType::Success);
	}
}

VkRenderer::SwapChain::~SwapChain()
{
	vkDestroySwapchainKHR(m_vars->m_device, m_vars->m_swapChain, nullptr);
}
