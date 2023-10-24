#include "descriptor.hpp"

void VkRenderer::UniformBufferDescriptor::createDescriptorSetLayout()
{
	m_layoutBinding.binding = 0;
	m_layoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	m_layoutBinding.descriptorCount = 1;
	m_layoutBinding.stageFlags = m_shaderType;

	m_layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	m_layoutInfo.bindingCount = 1;
	m_layoutInfo.pBindings = &m_layoutBinding;

	if (vkCreateDescriptorSetLayout(m_vars->m_device, &m_layoutInfo, nullptr, &m_layout) != VK_SUCCESS) {}
}

void VkRenderer::UniformBufferDescriptor::createUniformBuffer()
{
	m_buffers.resize(Extra::FRAMES_IN_FLIGHT);

	for (size_t i = 0; i < m_buffers.size(); ++i) {
		m_buffers[i] = std::make_unique<UniformBuffer>(m_vars, m_size);
	}
}

VkRenderer::UniformBufferDescriptor::UniformBufferDescriptor(Extra::VkVars* vars, VkShaderStageFlagBits shaderType, VkDeviceSize size, uint32_t binding)
	: m_vars(vars), m_binding(binding), m_shaderType(shaderType), m_size(size)
{
	createDescriptorSetLayout();
	createUniformBuffer();
	createDescriptorPool();
	createDescriptorSet();
}

void VkRenderer::UniformBufferDescriptor::bindLayoutInfo(VkPipelineLayoutCreateInfo& pipelineInfo)
{
	pipelineInfo.pSetLayouts = &m_layout;
}

void VkRenderer::UniformBufferDescriptor::createDescriptorPool()
{
	m_poolSize.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	m_poolSize.descriptorCount = static_cast<uint32_t>(Extra::FRAMES_IN_FLIGHT);

	m_poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	m_poolInfo.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
	m_poolInfo.poolSizeCount = 1;
	m_poolInfo.pPoolSizes = &m_poolSize;
	m_poolInfo.maxSets = static_cast<uint32_t>(Extra::FRAMES_IN_FLIGHT) * 2;

	if (vkCreateDescriptorPool(m_vars->m_device, &m_poolInfo, nullptr, &m_vars->m_pool) != VK_SUCCESS) {}
}

void VkRenderer::UniformBufferDescriptor::createDescriptorSet()
{
	std::vector<VkDescriptorSetLayout> layouts(Extra::FRAMES_IN_FLIGHT, m_layout);

	VkDescriptorSetAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	allocInfo.descriptorPool = m_vars->m_pool;
	allocInfo.descriptorSetCount = static_cast<uint32_t>(Extra::FRAMES_IN_FLIGHT);
	allocInfo.pSetLayouts = layouts.data();

	descriptorSets.resize(Extra::FRAMES_IN_FLIGHT);
	if (vkAllocateDescriptorSets(m_vars->m_device, &allocInfo, descriptorSets.data()) != VK_SUCCESS) {}

	for (size_t i = 0; i < Extra::FRAMES_IN_FLIGHT; ++i) {
		VkDescriptorBufferInfo bufferInfo{};
		bufferInfo.buffer = m_buffers[i]->getBuffer();
		bufferInfo.offset = 0;
		bufferInfo.range = m_size;

		VkWriteDescriptorSet descriptorWrite{};
		descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrite.dstSet = descriptorSets[i];
		descriptorWrite.dstBinding = 0;
		descriptorWrite.dstArrayElement = 0;
		descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		descriptorWrite.descriptorCount = 1;
		descriptorWrite.pBufferInfo = &bufferInfo;

		vkUpdateDescriptorSets(m_vars->m_device, 1, &descriptorWrite, 0, nullptr);
	}
}

void VkRenderer::UniformBufferDescriptor::destroy()
{
	vkDestroyDescriptorPool(m_vars->m_device, m_vars->m_pool, nullptr);
	for (const auto& buffer : m_buffers) {
		buffer->destroy();
	}
	vkDestroyDescriptorSetLayout(m_vars->m_device, m_layout, nullptr);
}
