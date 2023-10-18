#include "vertex_buffer.hpp"

VkRenderer::VertexBuffer::VertexBuffer(Extra::VkVars* vars, VkDeviceSize size, int location)
	: Buffer(&vars->m_device, &vars->m_physicalDevice, size, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT), m_location(location), m_size(size)

{
	setLayout();
}
void VkRenderer::VertexBuffer::setLayout()
{
	bindingDescription.binding = 0;
	bindingDescription.stride = sizeof(Extra::Vertex);
	bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

	positionAttribute.binding = 0;
	positionAttribute.location = m_location;
	positionAttribute.format = VK_FORMAT_R32G32B32_SFLOAT;
	positionAttribute.offset = offsetof(Extra::Vertex, position);

	m_createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	m_createInfo.vertexAttributeDescriptionCount = 1;
	m_createInfo.vertexBindingDescriptionCount = 1;
	m_createInfo.pVertexBindingDescriptions = &bindingDescription;
	m_createInfo.pVertexAttributeDescriptions = &positionAttribute;
}
