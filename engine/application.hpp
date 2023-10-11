#pragma once
#include "window.hpp"
#include "instance.hpp"
#include "validation_layers.hpp"
#include "debugger.hpp"
#include "logger.hpp"
#include "physical_device.hpp"
#include <memory>

#ifdef NDEBUG
	const bool enableValidationLayer = false;
#else
	const bool enableValidationLayer = true;
#endif

namespace VkRenderer {
	class App {
	private:
		VkInstance m_instance;
		VkInstanceCreateInfo m_instanceInfo{};
		VkApplicationInfo m_appInfo{};
		VkPhysicalDevice m_device;

		const int window_width, window_height;

		Logger logger;

		void initVk();

	private:
		std::shared_ptr<VkRenderer::Window> app_window;
		std::shared_ptr<VkRenderer::ValidationLayer> app_validation_layer;

		std::unique_ptr<VkRenderer::Instance> app_instance;
		std::unique_ptr<VkRenderer::PhysicalDevice> app_physical_device;
		std::unique_ptr<VkRenderer::Debugger> app_debugger;

	public:
		App(const int w_width, const int w_height);

		void run();
	};
}