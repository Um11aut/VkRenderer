#pragma once

#include "../extra/extra.hpp"
#include <memory>
#include <vulkan/vulkan.h>
#include "../logger.hpp"
#include <vector>
#include "../swap_chain.hpp"
#include "../command_buffer.hpp"

#include "imgui.h"
#include "backends/imgui_impl_vulkan.h"
#include "backends/imgui_impl_glfw.h"

using namespace VkRenderer;

namespace Gui {
	class RenderPass {
	private:
		Extra::VkVars* m_vars;

		std::unique_ptr<CommandBuffer> commandBuffer;

		ImGui_ImplVulkan_InitInfo initInfo{};

		std::shared_ptr<SwapChain> m_swapChain;
	public:
		RenderPass(Extra::VkVars* vars, std::shared_ptr<SwapChain> swapChain) 
			: m_swapChain(swapChain), m_vars(vars)
		{
			auto indices = Device::findSupportedQueueFamilies(m_vars->m_physicalDevice, m_vars->m_surface);

			VkDescriptorPoolSize pool_sizes[] =
			{
				{ VK_DESCRIPTOR_TYPE_SAMPLER, 1000 },
				{ VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1000 },
				{ VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1000 },
				{ VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1000 },
				{ VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 1000 },
				{ VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 1000 },
				{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1000 },
				{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1000 },
				{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1000 },
				{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 1000 },
				{ VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 1000 }
			};

			VkDescriptorPoolCreateInfo pool_info = {};
			pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
			pool_info.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
			pool_info.maxSets = 1000;
			pool_info.poolSizeCount = std::size(pool_sizes);
			pool_info.pPoolSizes = pool_sizes;

			VkDescriptorPool imguiPool;
			vkCreateDescriptorPool(m_vars->m_device, &pool_info, nullptr, &imguiPool);

			initInfo.Instance = m_vars->m_instance;
			initInfo.PhysicalDevice = m_vars->m_physicalDevice;
			initInfo.Device = m_vars->m_device;
			initInfo.Queue = m_vars->m_graphicsQueue;
			initInfo.DescriptorPool = imguiPool;
			initInfo.MinImageCount = 3;
			initInfo.ImageCount = 3;
			initInfo.MSAASamples = VK_SAMPLE_COUNT_1_BIT;

			commandBuffer = std::make_unique<CommandBuffer>(m_vars);
			
			ImGui_ImplVulkan_Init(&initInfo, m_vars->m_renderPass);

			commandBuffer->startSingleUse();
				ImGui_ImplVulkan_CreateFontsTexture(commandBuffer->get());
			commandBuffer->endSingleUse();

			ImGui_ImplVulkan_DestroyFontUploadObjects();
		}

		void update() {
			commandBuffer->startSingleUse();
				VkRenderPassBeginInfo renderPassInfo{};
				renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
				renderPassInfo.renderPass = m_vars->m_renderPass;
				renderPassInfo.framebuffer = m_swapChain->getFrameBuffers()[0];
				renderPassInfo.renderArea.offset = { 0,0 };
				renderPassInfo.renderArea.extent = m_swapChain->getExtent();

				VkClearValue clearColor = { {{0.0f, 0.0f, 0.0f, 1.0f}} };
				renderPassInfo.clearValueCount = 1;
				renderPassInfo.pClearValues = &clearColor;

				vkCmdBeginRenderPass(commandBuffer->get(), &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
					ImGui_ImplVulkan_NewFrame();
					ImGui_ImplGlfw_NewFrame();
					ImGui::NewFrame();

					ImGui::ShowDemoWindow();

					ImGui::Render();
					ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), commandBuffer->get());
				vkCmdEndRenderPass(commandBuffer->get());

			commandBuffer->endSingleUse();
		}


		~RenderPass() {
		}
	};
}