#include "command_buffer.hpp"

VkRenderer::CommandBuffer::CommandBuffer(Extra::VkVars* vars, std::shared_ptr<SwapChain> swapChain, std::shared_ptr<GraphicsPipeline> graphicsPipeline)
	: m_vars(vars), m_swapChain(swapChain), m_graphicsPipeline(graphicsPipeline)
{
	m_queueFamilyIndices = Device::findSupportedQueueFamilies(m_vars->m_physicalDevice, m_vars->m_surface);

	Logger::printOnce(std::to_string(m_queueFamilyIndices.graphicsFamily.value()));

	m_poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	m_poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
	m_poolInfo.queueFamilyIndex = 0;

	if (vkCreateCommandPool(m_vars->m_device, &m_poolInfo, nullptr, &m_commandPool) != VK_SUCCESS) {}
	else {
		Logger::printOnce("Created Command Pool", MessageType::Success);
	}

	m_commandBufferAllocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	m_commandBufferAllocateInfo.commandPool = m_commandPool;
	m_commandBufferAllocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	m_commandBufferAllocateInfo.commandBufferCount = 1;

	if (vkAllocateCommandBuffers(m_vars->m_device, &m_commandBufferAllocateInfo, &m_vars->m_commandBuffer) != VK_SUCCESS) {}
	else {
		Logger::printOnce("Created Command Buffer!", MessageType::Success);
	}
}

void VkRenderer::CommandBuffer::record(uint32_t imageIndex)
{
	VkCommandBufferBeginInfo beginInfo{};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

	if (vkBeginCommandBuffer(m_vars->m_commandBuffer, &beginInfo) != VK_SUCCESS) {}

	VkRenderPassBeginInfo renderPassInfo{};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	renderPassInfo.renderPass = m_vars->m_renderPass;
	renderPassInfo.framebuffer = m_swapChain->getFrameBuffers()[imageIndex];
	renderPassInfo.renderArea.offset = { 0,0 };
	renderPassInfo.renderArea.extent = m_swapChain->getExtent();
	
	VkClearValue clearColor = {{{0.0f, 1.0f, 0.0f, 1.0f}}};
	renderPassInfo.clearValueCount = 1;
	renderPassInfo.pClearValues = &clearColor;

	vkCmdBeginRenderPass(m_vars->m_commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

		vkCmdBindPipeline(m_vars->m_commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_graphicsPipeline->getPipeline());

		VkViewport viewPort{};
		viewPort.x = 0.0f;
		viewPort.y = 0.0f;
		viewPort.width = static_cast<float>(m_swapChain->getExtent().width);
		viewPort.height = static_cast<float>(m_swapChain->getExtent().height);
		viewPort.minDepth = 0.0f;
		viewPort.maxDepth = 1.0f;
		vkCmdSetViewport(m_vars->m_commandBuffer, 0, 1, &viewPort);

		VkRect2D scissor{};
		scissor.offset = { 0,0 };
		scissor.extent = m_swapChain->getExtent();
		vkCmdSetScissor(m_vars->m_commandBuffer, 0, 1, &scissor);

		vkCmdDraw(m_vars->m_commandBuffer, 3, 1, 0, 0);

	vkCmdEndRenderPass(m_vars->m_commandBuffer);

	if (vkEndCommandBuffer(m_vars->m_commandBuffer) != VK_SUCCESS) {}
}

VkRenderer::CommandBuffer::~CommandBuffer()
{
	vkDestroyCommandPool(m_vars->m_device, m_commandPool, nullptr);
}
