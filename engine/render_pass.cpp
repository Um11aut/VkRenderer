#include "render_pass.hpp"

VkRenderer::RenderPass::RenderPass(Extra::VkVars* vars, std::shared_ptr<SwapChain> swapChain)
	: m_vars(vars), m_swapChain(swapChain)
{
	colorAttachment.format = m_swapChain->getFormat();
	colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
	colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

	colorAttachmentRef.attachment = 0;
	colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	subpass.pColorAttachments = &colorAttachmentRef;

	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	renderPassInfo.attachmentCount = 1;
	renderPassInfo.pAttachments = &colorAttachment;
	renderPassInfo.subpassCount = 1;
	renderPassInfo.pSubpasses = &subpass;

	if (vkCreateRenderPass(m_vars->m_device, &renderPassInfo, nullptr, &m_vars->m_renderPass) != VK_SUCCESS) {}
	else {
		Logger::printOnce("Created Render Pass!", MessageType::Success);
	}
}

VkRenderer::RenderPass::~RenderPass()
{
	vkDestroyRenderPass(m_vars->m_device, m_vars->m_renderPass, nullptr);
}
