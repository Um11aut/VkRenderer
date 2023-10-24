#include "draw_command_buffer.hpp"

VkRenderer::DrawCommandBuffer::DrawCommandBuffer(Extra::VkVars* vars, std::shared_ptr<SwapChain> swapChain, std::shared_ptr<GraphicsPipeline> graphicsPipeline, std::shared_ptr<VertexBuffer> vertexBuffer, std::shared_ptr<IndexBuffer> indexBuffer, std::shared_ptr<UniformBufferDescriptor> descriptorBuffer)
	: m_vars(vars), m_swapChain(swapChain), m_graphicsPipeline(graphicsPipeline), m_vertexBuffer(vertexBuffer), m_indexBuffer(indexBuffer), m_descriptorBuffer(descriptorBuffer)
{
	m_commandBuffers.resize(Extra::FRAMES_IN_FLIGHT);
	m_commandBufferAllocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	m_commandBufferAllocateInfo.commandPool = m_vars->m_commandPool;
	m_commandBufferAllocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	m_commandBufferAllocateInfo.commandBufferCount = static_cast<uint32_t>(m_commandBuffers.size());

	if (vkAllocateCommandBuffers(m_vars->m_device, &m_commandBufferAllocateInfo, m_commandBuffers.data()) != VK_SUCCESS) {}
	else {
		Logger::printOnce("Created Command Buffer!", MessageType::Success);
	}
}

void VkRenderer::DrawCommandBuffer::submit(const uint32_t currentFrame, const Syncher& syncher)
{
	VkSubmitInfo submitInfo{};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

	VkSemaphore waitSemaphores[] = { syncher.m_imageAvailableSemaphore[currentFrame] };
	VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
	submitInfo.waitSemaphoreCount = 1;
	submitInfo.pWaitSemaphores = waitSemaphores;
	submitInfo.pWaitDstStageMask = waitStages;

	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &m_commandBuffers[currentFrame];

	VkSemaphore signalSemaphores[] = { syncher.m_renderFinishedSemaphore[currentFrame] };
	submitInfo.signalSemaphoreCount = 1;
	submitInfo.pSignalSemaphores = signalSemaphores;

	if (vkQueueSubmit(m_vars->m_graphicsQueue, 1, &submitInfo, syncher.m_inFlightFence[currentFrame]) != VK_SUCCESS) {}
}

void VkRenderer::DrawCommandBuffer::record(const uint32_t currentFrame, uint32_t imageIndex)
{
	VkCommandBufferBeginInfo beginInfo{};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

	if (vkBeginCommandBuffer(m_commandBuffers[currentFrame], &beginInfo) != VK_SUCCESS) {}

	VkRenderPassBeginInfo renderPassInfo{};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	renderPassInfo.renderPass = m_vars->m_renderPass;
	renderPassInfo.framebuffer = m_swapChain->getFrameBuffers()[imageIndex];
	renderPassInfo.renderArea.offset = { 0,0 };
	renderPassInfo.renderArea.extent = m_swapChain->getExtent();

	VkClearValue clearColor = { {{0.0f, 0.0f, 0.0f, 1.0f}} };
	renderPassInfo.clearValueCount = 1;
	renderPassInfo.pClearValues = &clearColor;

	vkCmdBeginRenderPass(m_commandBuffers[currentFrame], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

		vkCmdBindPipeline(m_commandBuffers[currentFrame], VK_PIPELINE_BIND_POINT_GRAPHICS, m_graphicsPipeline->getPipeline());

		VkViewport viewPort{};
		viewPort.x = 0.0f;
		viewPort.y = 0.0f;
		viewPort.width = static_cast<float>(m_swapChain->getExtent().width);
		viewPort.height = static_cast<float>(m_swapChain->getExtent().height);
		viewPort.minDepth = 0.0f;
		viewPort.maxDepth = 1.0f;
		vkCmdSetViewport(m_commandBuffers[currentFrame], 0, 1, &viewPort);

		VkRect2D scissor{};
		scissor.offset = { 0, 0 };
		scissor.extent = m_swapChain->getExtent();
		vkCmdSetScissor(m_commandBuffers[currentFrame], 0, 1, &scissor);

		m_vertexBuffer->bind(m_commandBuffers[currentFrame]);

		m_indexBuffer->bind(m_commandBuffers[currentFrame]);

		m_descriptorBuffer->bind(m_commandBuffers[currentFrame], m_graphicsPipeline->getLayout(), currentFrame);

		vkCmdDrawIndexed(m_commandBuffers[currentFrame], static_cast<uint32_t>(m_indexBuffer->getSize()), 1, 0, 0, 0);

		ImGui_ImplVulkan_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		ImGui::ShowDemoWindow();

		ImGui::Render();
		ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), m_commandBuffers[currentFrame]);

	vkCmdEndRenderPass(m_commandBuffers[currentFrame]);

	if (vkEndCommandBuffer(m_commandBuffers[currentFrame]) != VK_SUCCESS) {}
}