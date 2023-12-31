#pragma once

#include <vulkan/vulkan.h>

#include <fstream>
#include <vector>
#include <string>
#include "logger.hpp"
#include "extra/extra.hpp"

namespace VkRenderer {
	class ShaderModule {
	private:
		Extra::VkVars* m_vars;

		std::string m_fragmentPath, m_vertexPath;

		VkShaderModule frag = VK_NULL_HANDLE;
		VkShaderModule vert = VK_NULL_HANDLE;

		static std::vector<char> readFile(const std::string& filePath);
		VkShaderModule createShaderModule(const std::vector<char>& code);
	public:
		ShaderModule(Extra::VkVars* vars, std::string fragmentPath, std::string vertexPath);
		void destroy();
		std::pair<VkShaderModule, VkShaderModule> get();
	};
}