#pragma once

#include <vulkan/vulkan.h>
#include "buffer.hpp"
#include "../command_buffer.hpp"
#include <array>

namespace VkRenderer {
	class StagingBuffer : public Buffer {
	public:
		StagingBuffer(Extra::VkVars* vars, VkDeviceSize size) :
			Buffer(
				&vars->m_device,
				&vars->m_physicalDevice,
				size,
				VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
				VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT) {}
		void update(const void* data, VkDeviceSize size) override {
			void* mappedData;
			vkMapMemory(*m_device, m_memory, 0, size, 0, &mappedData);
			memcpy(mappedData, data, static_cast<size_t>(size));
			vkUnmapMemory(*m_device, m_memory);
		}

		void destroy() {
			destroyBuffer();
		}

		void bind(VkCommandBuffer commandBuffer) const override {

		}
	};

	class VertexBuffer : public Buffer {
	public:
		VertexBuffer(Extra::VkVars* vars, VkDeviceSize size, int location = 0);

		void update(const void* data, VkDeviceSize size) override {
			stagingBuffer->update(data, size);

			stagingBuffer->copyTo(singleCommandBuffer, this->m_buffer, size);

			stagingBuffer->destroy();
		}

		void bind(VkCommandBuffer commandBuffer) const override {
			VkBuffer vertexBuffers[] = { m_buffer };
			VkDeviceSize offsets[] = { 0 };
			vkCmdBindVertexBuffers(commandBuffer, 0, 1, &m_buffer, offsets);
		}

		void destroy() {
			destroyBuffer();
		}

		const VkPipelineVertexInputStateCreateInfo& getLayout() const { return m_createInfo; }
		const VkDeviceSize& getSize() const { return m_size; };
	private:
		void setLayout();

		std::unique_ptr<VkRenderer::StagingBuffer> stagingBuffer;
		std::unique_ptr<VkRenderer::CommandBuffer> singleCommandBuffer;

		const int m_location;
		VkDeviceSize m_size;
		VkPipelineVertexInputStateCreateInfo m_createInfo{};

		VkVertexInputBindingDescription bindingDescription{};
		std::array<VkVertexInputAttributeDescription, 2> attributeDescriptions{};
	};
}