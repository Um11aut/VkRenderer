#pragma once

#include <glfw/glfw3.h>
#include <vulkan/vulkan.h>

#include <string>
#include <vector>
#include <memory>

#include "logger.hpp"
#include "validation_layers.hpp"
#include "debugger.hpp"

namespace VkRenderer {
	class Instance {
	private:
		VkInstance* m_instance{};

		VkInstanceCreateInfo* m_instanceInfo{};
		VkApplicationInfo* m_appInfo{};

		VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};

		std::shared_ptr<VkRenderer::ValidationLayer> m_validationLayer;

		uint32_t glfwExtensionCount = 0;
		const char** glfwExtensions;

		std::vector<const char*> requiredExtensions;

		uint32_t availableExtensionsCount = 0;
		std::vector<VkExtensionProperties> availibleExtensions;

		std::string m_appName, m_engineName;

		void createInstance();
		void createInfo();
		void enableExtensions();

		void getRequiredExtensions();
		void getAvailableExtensions();

		void useExtensions();
	public:
		Instance(std::string appName, std::string engineName,
			VkInstance* instance, VkInstanceCreateInfo* instanceInfo,
			VkApplicationInfo* appInfo, std::shared_ptr<VkRenderer::ValidationLayer> validationLayer);
		~Instance();

		void appendExtension(const char* extensionName);

		void create();

		void printExtensions(bool printRequired, bool printAvailable);
	};
}