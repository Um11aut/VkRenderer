#pragma once

#include <vulkan/vulkan.h>
#include "../command_buffer.hpp"
#include "buffer.hpp"
#include "staging_buffer.hpp"
#include <array>

namespace VkRenderer {
	class IndexBuffer : public Buffer {
	public:
		IndexBuffer(Extra::VkVars* vars, VkDeviceSize size, VkDeviceSize vectorSize) :
			Buffer(&vars->m_device,
				&vars->m_physicalDevice,
				size,
				VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
				VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT), m_vectorSize(vectorSize) {
			stagingBuffer = std::make_unique<VkRenderer::StagingBuffer>(vars, size);
			singleCommandBuffer = std::make_unique<VkRenderer::CommandBuffer>(vars);
		}

		void update(const void* data, VkDeviceSize size) override {
			stagingBuffer->update(data, size);

			stagingBuffer->copyTo(singleCommandBuffer, this->m_buffer, size);

			stagingBuffer->destroy();
		}

		void bind(VkCommandBuffer commandBuffer) const override {
			vkCmdBindIndexBuffer(commandBuffer, m_buffer, 0, VK_INDEX_TYPE_UINT16);
		}

		void destroy() {
			destroyAsync();
		}

		const VkDeviceSize& getSize() const { return m_vectorSize; };
	private:
		uint32_t m_vectorSize = 0;

		std::unique_ptr<VkRenderer::StagingBuffer> stagingBuffer;
		std::unique_ptr<VkRenderer::CommandBuffer> singleCommandBuffer;
	};
}