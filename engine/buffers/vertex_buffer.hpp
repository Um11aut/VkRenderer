#pragma once

#include <vulkan/vulkan.h>
#include "buffer.hpp"

namespace VkRenderer {
	class VertexBuffer : public Buffer {
	public:
		VertexBuffer(Extra::VkVars* vars, VkDeviceSize size, int location = 0);

		void update(const void* data, VkDeviceSize size) override {
			void* mappedData;
			vkMapMemory(*m_device, m_memory, 0, size, 0, &mappedData);
			memcpy(mappedData, data, static_cast<size_t>(size));
			vkUnmapMemory(*m_device, m_memory);
		}

		void bind(VkCommandBuffer commandBuffer) const override {
			VkBuffer vertexBuffers[] = { m_buffer };
			VkDeviceSize offsets[] = {0};
			vkCmdBindVertexBuffers(commandBuffer, 0, 1, &m_buffer, offsets);
		}

		void destroy() {
			destroyBuffer();
		}

		const VkPipelineVertexInputStateCreateInfo& getLayout() const { return m_createInfo; }
		const VkDeviceSize& getSize() const { return m_size; };
	private:
		void setLayout();

		const int m_location;
		VkDeviceSize m_size;
		VkPipelineVertexInputStateCreateInfo m_createInfo{};

		VkVertexInputBindingDescription bindingDescription{};
		VkVertexInputAttributeDescription positionAttribute{};
		VkVertexInputAttributeDescription colorAttribute{};
	};
}