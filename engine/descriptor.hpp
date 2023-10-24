#pragma once

#include "extra/extra.hpp"
#include "buffers/uniform_buffer.hpp"
#include <vulkan/vulkan.hpp>
#include <vector>
#include <iostream>
#include <memory>

namespace VkRenderer {
	class UniformBufferDescriptor {
	private:
		Extra::VkVars* m_vars;
		const uint32_t m_binding;
		VkDeviceSize m_size;
		VkShaderStageFlagBits m_shaderType;

		VkDescriptorSetLayout m_layout;

		VkDescriptorSetLayoutBinding m_layoutBinding{};
		VkDescriptorSetLayoutCreateInfo m_layoutInfo{};

		std::vector<VkDescriptorSet> descriptorSets;

		VkDescriptorPoolSize m_poolSize{};
		VkDescriptorPoolCreateInfo m_poolInfo{};

		std::vector<std::unique_ptr<UniformBuffer>> m_buffers;

		void createDescriptorSetLayout();
		void createUniformBuffer();
		void createDescriptorPool();
		void createDescriptorSet();
	public:
		UniformBufferDescriptor(Extra::VkVars* vars, VkShaderStageFlagBits shaderType, VkDeviceSize size, uint32_t binding = 0);

		void bindLayoutInfo(VkPipelineLayoutCreateInfo& pipelineInfo);

		void bind(VkCommandBuffer& commandBuffer, VkPipelineLayout pipelineLayout, uint32_t currentFrame) {
			vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 0, 1, &descriptorSets[currentFrame], 0, nullptr);
		}

		void update(auto& data, VkDeviceSize size, uint32_t currentFrame)
		{
			std::memcpy(m_buffers[currentFrame]->getMappedMemory(), &data, size);
		}

		void destroy();
	};
}