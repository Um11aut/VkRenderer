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

void VkRenderer::SwapChain::createImageViews()
{
	m_imageViews.resize(m_images.size());

	for (size_t i = 0; i < m_imageViews.size(); i++) {
		VkImageViewCreateInfo imageViewCreateInfo{};
		imageViewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		imageViewCreateInfo.image = m_images[i];
		imageViewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
		imageViewCreateInfo.format = m_imageFormat;

		imageViewCreateInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
		imageViewCreateInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
		imageViewCreateInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
		imageViewCreateInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

		imageViewCreateInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		imageViewCreateInfo.subresourceRange.baseMipLevel = 0;
		imageViewCreateInfo.subresourceRange.levelCount = 1;
		imageViewCreateInfo.subresourceRange.baseArrayLayer = 0;
		imageViewCreateInfo.subresourceRange.layerCount = 1;


		if (vkCreateImageView(m_vars->m_device, &imageViewCreateInfo, nullptr, &m_imageViews[i]) != VK_SUCCESS) {
			throw std::runtime_error("Failed to create an Image View!");
		}
	}

	if (!m_imageViews.empty()) {
		Logger::printOnce("Created SwapChain Image Views!", MessageType::Success);
	}
}

void VkRenderer::SwapChain::createFrameBuffers()
{
	m_frameBuffers.resize(m_imageViews.size());
	
	for (size_t i = 0; i < m_imageViews.size(); i++) {
		VkImageView attachments[] = {
			m_imageViews[i]
		};

		VkFramebufferCreateInfo frameInfo{};
		frameInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		frameInfo.renderPass = m_vars->m_renderPass;
		frameInfo.attachmentCount = 1;
		frameInfo.pAttachments = attachments;
		frameInfo.width = m_swapChainExtent.width;
		frameInfo.height = m_swapChainExtent.height;
		frameInfo.layers = 1;

		if (vkCreateFramebuffer(m_vars->m_device, &frameInfo, nullptr, &m_frameBuffers[i]) != VK_SUCCESS) {}
	}
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
	swapChainSupport = getSwapChainSupportDetails();

	VkSurfaceFormatKHR surfaceFormat = getSwapSurfaceFormat(swapChainSupport.formats);
	VkPresentModeKHR presentMode = getSwapPresentMode(swapChainSupport.presentModes);
	m_swapChainExtent = getSwapExtent(swapChainSupport.capabilities);

	imageCount = swapChainSupport.capabilities.minImageCount + 1;

	Logger::print({ "Minimum available SwapChain image count: ", std::to_string(swapChainSupport.capabilities.minImageCount) });
	Logger::print({ "Maximum available SwapChain image count: ", std::to_string(swapChainSupport.capabilities.maxImageCount) });

	if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount) {
		imageCount = swapChainSupport.capabilities.maxImageCount;
	}

	m_createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	m_createInfo.surface = m_vars->m_surface;
	m_createInfo.minImageCount = imageCount;
	m_createInfo.imageFormat = surfaceFormat.format;
	m_createInfo.imageColorSpace = surfaceFormat.colorSpace;
	m_createInfo.imageExtent = m_swapChainExtent;
	m_createInfo.imageArrayLayers = 1;
	m_createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

	Extra::QueueFamilyIndices indices = Device::findSupportedQueueFamilies(m_vars->m_physicalDevice, m_vars->m_surface);

	uint32_t queueFamilyIndices[] = { indices.graphicsFamily.value(), indices.presentFamily.value() };

	if (indices.graphicsFamily != indices.presentFamily) {
		m_createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
		m_createInfo.queueFamilyIndexCount = 2;
		m_createInfo.pQueueFamilyIndices = queueFamilyIndices;
	}
	else {
		m_createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
		m_createInfo.queueFamilyIndexCount = 0; // Optional
		m_createInfo.pQueueFamilyIndices = nullptr; // Optional
	}

	m_createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
	m_createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	m_createInfo.presentMode = presentMode;
	m_createInfo.clipped = VK_TRUE;
	m_createInfo.oldSwapchain = VK_NULL_HANDLE;

	if (vkCreateSwapchainKHR(m_vars->m_device, &m_createInfo, nullptr, &m_vars->m_swapChain) != VK_SUCCESS) {}
	else {
		Logger::printOnce("Created SwapChain!", MessageType::Success);
	}

	vkGetSwapchainImagesKHR(m_vars->m_device, m_vars->m_swapChain, &imageCount, nullptr);
	m_images.resize(imageCount);
	vkGetSwapchainImagesKHR(m_vars->m_device, m_vars->m_swapChain, &imageCount, m_images.data());

	m_imageFormat = surfaceFormat.format;

	createImageViews();
}

void VkRenderer::SwapChain::recreate()
{
	vkDeviceWaitIdle(m_vars->m_device);

	destroy();

	create();
	createImageViews();
	createFrameBuffers();
}

void VkRenderer::SwapChain::destroy()
{
	Logger::printOnce("Destroyed SwapChain!");

	for (const auto& frameBuffer : m_frameBuffers) {
		vkDestroyFramebuffer(m_vars->m_device, frameBuffer, nullptr);
	}

	for (const auto& imageView : m_imageViews) {
		vkDestroyImageView(m_vars->m_device, imageView, nullptr);
	}

	vkDestroySwapchainKHR(m_vars->m_device, m_vars->m_swapChain, nullptr);
}