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

		static std::vector<char> readFile(const std::string& filePath);
		VkShaderModule createShaderModule(const std::vector<char>& code);
	public:
		ShaderModule(Extra::VkVars* vars);
		std::pair<VkShaderModule, VkShaderModule> load(const std::string fragmentPath, const std::string vertexPath);
	};
}