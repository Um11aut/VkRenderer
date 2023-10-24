#pragma once

#include "window.hpp"
#include "instance.hpp"
#include "validation_layers.hpp"
#include "debugger.hpp"
#include "logger.hpp"
#include "device.hpp"
#include "surface.hpp"
#include "swap_chain.hpp"
#include "graphics_pipeline.hpp"
#include "shader.hpp"
#include "render_pass.hpp"
#include "command_buffer.hpp"
#include "drawer.hpp"
#include "gui/gui.hpp"
#include "extra/extra.hpp"
#include "layers/triangle.hpp"
#include "layers/parent/layer_factory.hpp"
#include "layers/interface.hpp"
#include <memory>

#include "imgui.h"
#include "backends/imgui_impl_vulkan.h"
#include "backends/imgui_impl_glfw.h"

#ifdef NDEBUG
	const bool enableValidationLayer = false;
#else
	const bool enableValidationLayer = true;
#endif

namespace VkRenderer {
	class Renderer {
	private:
		Extra::VkVars m_variables;

		void initVk();
		std::shared_ptr<VkRenderer::Window> app_window;

	private:

		std::unique_ptr<VkRenderer::Instance> app_instance;
		std::unique_ptr<VkRenderer::Surface> app_surface;
		std::unique_ptr<VkRenderer::Device> app_device;
		std::shared_ptr<VkRenderer::SwapChain> app_swapChain;
		std::unique_ptr<VkRenderer::RenderPass> renderPass;

		std::vector<std::shared_ptr<Layer>> layers;

		std::shared_ptr<VkRenderer::ValidationLayer> app_validation_layer;
		std::unique_ptr<VkRenderer::Debugger> app_debugger;

	public:
		Renderer(Window* window);
		~Renderer();

		void run();
		void init();
	};
}