#pragma once

#include <vulkan/vulkan.h>
#include "shader.hpp"
#include "swap_chain.hpp"
#include <array>
#include <vector>

namespace VkRenderer {
	class GraphicsPipeline {
	private:
		std::vector<VkDynamicState> dynamicStates = {
			VK_DYNAMIC_STATE_VIEWPORT,
			VK_DYNAMIC_STATE_SCISSOR
		};

		Extra::VkVars* m_vars;

		VkPipelineLayout m_pipelineLayout;

		SwapChain* m_swapChain;
		std::unique_ptr<ShaderModule> m_shaderModule;

		void createShaderStageInfo();
		std::array<VkPipelineShaderStageCreateInfo, 2> m_shaderStages;

		void createFixedFunctions();
	public:
		GraphicsPipeline(Extra::VkVars* vars, const ShaderModule& module, SwapChain& swapChain);
		~GraphicsPipeline();
	};
}