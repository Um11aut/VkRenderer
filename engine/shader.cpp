#include "shader.hpp"

std::vector<char> VkRenderer::ShaderModule::readFile(const std::string& filePath)
{
	std::ifstream file(filePath, std::ios::ate | std::ios::binary);

	if (!file.is_open()) {
		Logger::print({ "Failed to open file! ", filePath }, MessageType::Error);
		std::terminate();
	}
	else {
		Logger::print({"Reading file: ", filePath});
	}

	size_t fileSize = (size_t)file.tellg();
	std::vector<char> buffer(fileSize);
	file.seekg(0);
	file.read(buffer.data(), fileSize);

	file.close();
	return buffer;
}

VkShaderModule VkRenderer::ShaderModule::createShaderModule(const std::vector<char>& code)
{
	VkShaderModuleCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	createInfo.codeSize = code.size();
	createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

	VkShaderModule shaderModule;
	if (vkCreateShaderModule(m_vars->m_device, &createInfo, nullptr, &shaderModule) != VK_SUCCESS) {}
	else {
		Logger::printOnce("Created Shader Module!", MessageType::Success);
	}

	return shaderModule;
}

VkRenderer::ShaderModule::ShaderModule(Extra::VkVars* vars)
	: m_vars(vars)
{
}

VkRenderer::ShaderModule::~ShaderModule()
{
	vkDestroyShaderModule(m_vars->m_device, frag, nullptr);
	vkDestroyShaderModule(m_vars->m_device, vert, nullptr);
}

std::pair<VkShaderModule, VkShaderModule> VkRenderer::ShaderModule::load(const std::string fragmentPath, const std::string vertexPath)
{
	auto fragShaderCode = readFile(fragmentPath);
	auto vertShaderCode = readFile(vertexPath);
	
	frag = createShaderModule(fragShaderCode);
	vert = createShaderModule(vertShaderCode);

	return std::pair<VkShaderModule, VkShaderModule>(frag, vert);
}
