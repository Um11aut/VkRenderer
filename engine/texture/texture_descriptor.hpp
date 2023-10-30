#pragma once
#include <vulkan/vulkan.h>
#include "../extra/extra.hpp"

namespace VkRenderer {
	class TextureDescriptor {
	public:
		TextureDescriptor(Extra::VkVars* vars);

		void bindDescriptorImageInfo(VkImageView& imageView, VkSampler& sampler);

		void create();

	private:
		Extra::VkVars* m_vars;

		VkDescriptorImageInfo imageInfo{};

		VkDescriptorSetLayout layout;
		VkDescriptorPool pool;
		VkDescriptorSet descriptorSet;
	};
}