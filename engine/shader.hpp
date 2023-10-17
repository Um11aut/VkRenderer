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

		VkShaderModule frag = VK_NULL_HANDLE;
		VkShaderModule vert = VK_NULL_HANDLE;

		static std::vector<char> readFile(const std::string& filePath);
		VkShaderModule createShaderModule(const std::vector<char>& code);
	public:
		ShaderModule(Extra::VkVars* vars);
		void destroy();
		std::pair<VkShaderModule, VkShaderModule> load(const std::string fragmentPath, const std::string vertexPath);
	};
}