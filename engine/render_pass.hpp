#pragma once

#include "extra/extra.hpp"
#include <memory>
#include "logger.hpp"
#include "swap_chain.hpp"

namespace VkRenderer {
	class RenderPass {
	private:
		Extra::VkVars* m_vars;

		std::shared_ptr<SwapChain> m_swapChain;

		VkAttachmentDescription colorAttachment{};
		VkAttachmentReference colorAttachmentRef{};
		VkSubpassDescription subpass{};
		VkSubpassDependency dependency{};
		VkRenderPassCreateInfo renderPassInfo{};
	public:
		RenderPass(Extra::VkVars* vars, std::shared_ptr<SwapChain> swapChain);
		void destroy();
	};
}