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
#include "../logger.hpp"
#include "../buffers/vertex_buffer.hpp"

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
        VkRenderer::Logger::print({ "Frame count ", std::to_string(frames) });
        syncher->destroy(variables->m_device);
        vertexBuffer->destroy();
        commandBuffer->destroy();
        trianglePipeline->destroy();
        shaderModule->destroy();
        renderPass->destroy();
        swapChain->destroy();

    }
private:
    Extra::VkVars* variables;
    uint32_t currentFrame = 0;
    uint32_t frames = 0;

    std::unique_ptr<VkRenderer::RenderPass> renderPass;
    
    std::shared_ptr<VkRenderer::ShaderModule> shaderModule;
    std::shared_ptr<VkRenderer::VertexBuffer> vertexBuffer;
    std::shared_ptr<VkRenderer::GraphicsPipeline> trianglePipeline;

    const std::vector<Extra::Vertex> vertices = {
        {{-0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}},
        {{0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}},
        {{0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}},
        {{-0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}}
    };

    std::shared_ptr<VkRenderer::SwapChain> swapChain;
    std::unique_ptr<VkRenderer::CommandBuffer> commandBuffer;
    std::unique_ptr <VkRenderer::Syncher> syncher;

    void draw();
};