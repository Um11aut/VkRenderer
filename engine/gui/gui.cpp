#include "gui.hpp"

VkRenderer::GUI::GUI(Extra::VkVars* vars, std::shared_ptr<VkRenderer::SwapChain> swapChain)
	: m_vars(vars), m_swapChain(swapChain)
{
	auto indices = Device::findSupportedQueueFamilies(m_vars->m_physicalDevice, m_vars->m_surface);

	initInfo.Instance = m_vars->m_instance;
	initInfo.PhysicalDevice = m_vars->m_physicalDevice;
	initInfo.Device = m_vars->m_device;
	initInfo.QueueFamily = indices.graphicsFamily.value();
	initInfo.Queue = m_vars->m_graphicsQueue;
	initInfo.PipelineCache = VK_NULL_HANDLE;
	initInfo.Allocator = nullptr;
	initInfo.MinImageCount = 2;
	initInfo.CheckVkResultFn = nullptr;
	initInfo.ImageCount = static_cast<uint32_t>(m_swapChain->getDetails().capabilities.maxImageCount);
}

VkRenderer::GUI::~GUI()
{
}
