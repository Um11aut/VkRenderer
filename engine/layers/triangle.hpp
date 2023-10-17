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
        syncher->destroy(variables->m_device);
        commandBuffer->destroy();
        trianglePipeline->destroy();
        triangle1Pipeline->destroy();
        shaderModule->destroy();
        shaderModule1->destroy();
        renderPass->destroy();
        swapChain->destroy();
    }
private:
    Extra::VkVars* variables;


    std::unique_ptr<VkRenderer::RenderPass> renderPass;
    
    std::shared_ptr<VkRenderer::ShaderModule> shaderModule;
    std::shared_ptr<VkRenderer::ShaderModule> shaderModule1;
    std::shared_ptr<VkRenderer::GraphicsPipeline> trianglePipeline;
    std::shared_ptr<VkRenderer::GraphicsPipeline> triangle1Pipeline;

    std::shared_ptr<VkRenderer::SwapChain> swapChain;
    std::unique_ptr<VkRenderer::CommandBuffer> commandBuffer;
    std::unique_ptr <VkRenderer::Syncher> syncher;

    void draw();
};