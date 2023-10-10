#include "validation_layers.hpp"

VkRenderer::ValidationLayer::ValidationLayer(bool ValidationLayerEnabled)
	: m_validationLayerEnabled(ValidationLayerEnabled)
{
	vkEnumerateInstanceLayerProperties(&m_layerCount, nullptr);

	m_availableLayers.resize(m_layerCount);
	vkEnumerateInstanceLayerProperties(&m_layerCount, m_availableLayers.data());
}

bool VkRenderer::ValidationLayer::checkValidationLayerSupport()
{
	for (const auto& layerName : validationLayers) {
		bool layerFound = false;

		for (const auto& layerProperties : m_availableLayers) {
			if (strcmp(layerName, layerProperties.layerName) == 0) {
				layerFound = true;
				break;
			}
		}

		if (!layerFound) {
			return false;
		}
	}

	return true;
}
