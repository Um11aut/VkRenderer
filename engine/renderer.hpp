#pragma once

#include "window.hpp"
#include "instance.hpp"
#include "validation_layers.hpp"
#include "debugger.hpp"
#include "logger.hpp"
#include "device.hpp"
#include <memory>

#ifdef NDEBUG
	const bool enableValidationLayer = false;
#else
	const bool enableValidationLayer = true;
#endif

namespace VkRenderer {
	struct AppInfo {
		std::string appName;
		std::string engineName;
		int windowWidth, windowHeight;
		bool GUI;
	};

	class Renderer {
	private:
		VkInstance m_instance;
		VkPhysicalDevice m_physicalDevice;
		VkDevice m_device;

		AppInfo m_info;

		void initVk();

	private:
		std::shared_ptr<VkRenderer::Window> app_window;
		std::shared_ptr<VkRenderer::ValidationLayer> app_validation_layer;

		std::unique_ptr<VkRenderer::Instance> app_instance;
		std::unique_ptr<VkRenderer::Debugger> app_debugger;
		std::unique_ptr<VkRenderer::Device> app_device;

	public:
		Renderer(AppInfo info);

		void run();
	};
}