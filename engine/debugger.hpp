#pragma once

#include <glfw/glfw3.h>
#include <vulkan/vulkan.h>

#include <iostream>
#include <vector>
#include <memory>

#include "logger.hpp"
#include "validation_layers.hpp"

namespace VkRenderer {
	class Debugger {
	private:
		static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
			VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
			VkDebugUtilsMessageTypeFlagsEXT messageType,
			const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
			void* pUserData);

		VkResult CreateDebugUtilsMessengerEXT();

		void DestroyDebugUtilsMessengerEXT();

		void setupDebugMessenger();
		
		VkDebugUtilsMessengerEXT m_debugMessenger;
		VkDebugUtilsMessengerCreateInfoEXT m_createInfo;

		VkInstance* m_instance;

		std::shared_ptr<VkRenderer::ValidationLayer> m_validationLayer;
	public:
		Debugger(
			std::shared_ptr<VkRenderer::ValidationLayer> validation_layer,
			VkInstance* instance);
		~Debugger();

		static void populate(VkDebugUtilsMessengerCreateInfoEXT& createInfo);

		void destroy();
	};
}