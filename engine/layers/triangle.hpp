#pragma once

#include <iostream>
#include <memory>
#include <vector>
#include "parent/layer.hpp"
#include "../extra/extra.hpp"
#include "../swap_chain.hpp"
#include "../graphics_pipeline.hpp"
#include "../shader.hpp"
#include "../render_pass.hpp"
#include "../command_buffer.hpp"
#include "../drawer.hpp"
#include "../gui/gui.hpp"

class Triangle : public Layer {
public:
    Triangle(Extra::VkVars* vars, std::shared_ptr<VkRenderer::SwapChain> swapChain);

    void onStart() override {
        std::cout << "Triangle Application" << std::endl;
    }

    void onUpdate() override {
        draw();
    }

    void onDestroy() override {
        std::cout << "Destructor Called!" << std::endl;
        syncher->destroy(variables->m_device);
    }
private:
    Extra::VkVars* variables;

    std::shared_ptr<VkRenderer::SwapChain> app_swapChain;

    std::unique_ptr <VkRenderer::Syncher> syncher;
    std::unique_ptr<VkRenderer::RenderPass> app_renderPass;
    VkRenderer::ShaderModule shaderModule{ variables };
    std::shared_ptr<VkRenderer::GraphicsPipeline> m_mainPipeline;
    std::unique_ptr<VkRenderer::CommandBuffer> app_commandBuffer;
    std::unique_ptr<VkRenderer::GUI> app_gui;


    void draw();
};