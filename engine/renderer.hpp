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
		Extra::VkVars m_variables;

		void initVk();
		void draw();

	private:
		std::shared_ptr<VkRenderer::Window> app_window;
		std::shared_ptr<VkRenderer::ValidationLayer> app_validation_layer;
		
		std::unique_ptr<VkRenderer::Instance> app_instance;
		std::unique_ptr<VkRenderer::Debugger> app_debugger;
		std::unique_ptr<VkRenderer::Surface> app_surface;
		std::unique_ptr<VkRenderer::Device> app_device;

		std::unique_ptr<VkRenderer::RenderPass> app_renderPass;
		std::shared_ptr<VkRenderer::GraphicsPipeline> m_mainPipeline;
		std::shared_ptr<VkRenderer::SwapChain> app_swapChain;

		std::shared_ptr<VkRenderer::CommandBuffer> app_commandBuffer;

	public:
		Renderer(Window* window);

		void run();
		void init();
	};
}