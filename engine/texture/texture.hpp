#pragma once

#include <vulkan/vulkan.h>
#include "../extra/extra.hpp"
#include "../buffers/staging_buffer.hpp"
#include "../buffers/buffer.hpp"
#include "../command_buffer.hpp"
#include "texture_descriptor.hpp"
#include <stdexcept>
#include <memory>

#include "stb_image.h"


namespace VkRenderer {
	class Texture {
	public:
		Texture(Extra::VkVars* vars, const char* path);

		void destroy();
	private:
		Extra::VkVars* m_vars;

		std::unique_ptr<CommandBuffer> m_commandBuffer;
		std::unique_ptr<StagingBuffer> m_stagingBuffer;
		std::unique_ptr<TextureDescriptor> m_textureDescriptor;

		const char* m_path;
		int m_width, m_height;
		VkImage m_textureImage;
		VkDeviceMemory m_textureImageMemory;
		VkImageView m_textureImageView;
		VkSampler m_sampler;

		VkImageView createImageView(VkImage image, VkFormat format);

		void createTextureImage();
		void createTextureImageView();
		void createTextureSampler();
		void createImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory);
		void transitionImageLayout(VkImage& image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);
		void copyBufferToImage(VkBuffer buffer, VkImage& image, uint32_t width, uint32_t height);
	};
}