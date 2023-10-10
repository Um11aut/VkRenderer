#pragma once

#include <iostream>
#include <vector>

#include <vulkan/vulkan.h>
#include "logger.hpp"

namespace VkRenderer {
	const std::vector<const char*> validationLayers = {
			"VK_LAYER_KHRONOS_validation"
	};

	class ValidationLayer {
	private:
		bool m_validationLayerEnabled;

		uint32_t m_layerCount;
		std::vector<VkLayerProperties> m_availableLayers;

		Logger logger;
	public:
		ValidationLayer(bool ValidationLayerEnabled);

		bool isEnabled() const { return m_validationLayerEnabled; }
		bool checkValidationLayerSupport();
	};
}