#pragma once

#include <vulkan/vulkan.h>
#include "shader.hpp"
#include "swap_chain.hpp"
#include "buffers/vertex_buffer.hpp"
#include <array>
#include <vector>
#include <memory>

namespace VkRenderer {
	class GraphicsPipeline {
	private:
		std::vector<VkDynamicState> dynamicStates = {
			VK_DYNAMIC_STATE_VIEWPORT,
			VK_DYNAMIC_STATE_SCISSOR
		};

		Extra::VkVars* m_vars;

		VkPipeline m_graphicsPipeline{};
		VkPipelineLayout m_pipelineLayout{};

		VkGraphicsPipelineCreateInfo pipelineInfo{};

		std::shared_ptr<SwapChain> m_swapChain;
		std::shared_ptr<ShaderModule> m_shaderModule;
		std::shared_ptr<VertexBuffer> m_vertexBuffer;

		void createShaderStageInfo();
		VkPipelineShaderStageCreateInfo fragCreateInfo{};
		VkPipelineShaderStageCreateInfo vertCreateInfo{};
		std::array<VkPipelineShaderStageCreateInfo, 2> m_shaderStages;

		void createFixedFunctions();

		//fixed structs
		VkPipelineDynamicStateCreateInfo dynamicState{};
		VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
		VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
		VkRect2D scissor{};
		VkViewport viewport{};
		VkPipelineViewportStateCreateInfo viewportState{};
		VkPipelineRasterizationStateCreateInfo rasterizer{};
		VkPipelineMultisampleStateCreateInfo multisampling{};
		VkPipelineColorBlendAttachmentState colorBlendAttachment{};
		VkPipelineColorBlendStateCreateInfo colorBlending{};
		VkPipelineLayoutCreateInfo pipelineLayoutInfo{};

		void createPipeline();
	public:
		GraphicsPipeline(Extra::VkVars* vars, const std::shared_ptr<ShaderModule> module, std::shared_ptr<SwapChain> swapChain, std::shared_ptr<VertexBuffer> buffer);

		inline VkPipeline getPipeline() const { return m_graphicsPipeline; };

		void destroy();
	};
}