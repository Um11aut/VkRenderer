#pragma once

#include "window.hpp"
#include "instance.hpp"
#include "validation_layers.hpp"
#include "debugger.hpp"
#include "logger.hpp"
#include "device.hpp"
#include "surface.hpp"
#include "extra/extra.hpp"
#include <memory>

#ifdef NDEBUG
	const bool enableValidationLayer = false;
#else
	const bool enableValidationLayer = true;
#endif

namespace VkRenderer {
	class Renderer {
	private:
		VkInstance m_instance;
		VkPhysicalDevice m_physicalDevice;
		VkDevice m_device;
		VkSurfaceKHR m_surface;

		Extra::AppInfo m_info;

		void initVk();

	private:
		std::shared_ptr<VkRenderer::Window> app_window;
		std::shared_ptr<VkRenderer::ValidationLayer> app_validation_layer;

		std::unique_ptr<VkRenderer::Instance> app_instance;
		std::unique_ptr<VkRenderer::Debugger> app_debugger;
		std::unique_ptr<VkRenderer::Device> app_device;
		std::unique_ptr<VkRenderer::Surface> app_surface;

	public:
		Renderer(Extra::AppInfo info);

		void run();
	};
}