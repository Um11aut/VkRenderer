#include "vertex_buffer.hpp"

VkRenderer::VertexBuffer::VertexBuffer(Extra::VkVars* vars, VkDeviceSize size, int location)
	: Buffer(&vars->m_device, &vars->m_physicalDevice, size, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT), m_location(location), m_size(size)

{
	singleCommandBuffer = std::make_unique<VkRenderer::CommandBuffer>(vars);
	stagingBuffer = std::make_unique<VkRenderer::StagingBuffer>(vars, size);

	setLayout();
}
void VkRenderer::VertexBuffer::setLayout()
{
	bindingDescription.binding = 0;
	bindingDescription.stride = sizeof(Extra::Vertex);
	bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

	attributeDescriptions[0].binding = 0;
	attributeDescriptions[0].location = m_location;
	attributeDescriptions[0].format = VK_FORMAT_R32G32_SFLOAT;
	attributeDescriptions[0].offset = offsetof(Extra::Vertex, pos);

	attributeDescriptions[1].binding = 0;
	attributeDescriptions[1].location = m_location + 1;
	attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
	attributeDescriptions[1].offset = offsetof(Extra::Vertex, color);

	m_createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	m_createInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
	m_createInfo.vertexBindingDescriptionCount = 1;
	m_createInfo.pVertexBindingDescriptions = &bindingDescription;
	m_createInfo.pVertexAttributeDescriptions = attributeDescriptions.data();
}
