#pragma once
#include <vulkan/vulkan.h>
#include "extra/extra.hpp"

namespace VkRenderer {
	class TextureDescriptor {
	public:
		TextureDescriptor(Extra::VkVars* vars);

	private:
		Extra::VkVars* m_vars;

		VkDescriptorSetLayout layout;
		VkDescriptorSetLayoutBinding layoutBinding;
		VkDescriptorSet descriptorSet;
	};
}