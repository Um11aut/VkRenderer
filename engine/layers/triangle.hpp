#pragma once

#include <iostream>
#include <memory>
#include <vector>
#include "parent/layer.hpp"
#include "../extra/extra.hpp"
#include "../swap_chain.hpp"
#include "../graphics_pipeline.hpp"
#include "../shader.hpp"
#include "../ubo_descriptor.hpp"
#include "../render_pass.hpp"
#include "../draw_command_buffer.hpp"
#include "../gui/gui.hpp"
#include "../logger.hpp"
#include "../command_pool.hpp"
#include "../buffers/vertex_buffer.hpp"
#include "../buffers/index_buffer.hpp"
#include "../texture/texture.hpp"

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <chrono>

class Triangle : public Layer {
public:
    Triangle(Extra::VkVars * vars, std::shared_ptr<VkRenderer::SwapChain> swapChain);

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
        texture->destroy();
        indexBuffer->destroy();
        commandPool->destroy();
        UBOdescriptor->destroy();
        trianglePipeline->destroy();
        shaderModule->destroy();
        swapChain->destroy();
    }
private:
    Extra::VkVars* variables;
    uint32_t currentFrame = 0;
    uint32_t frames = 0;

    std::shared_ptr<VkRenderer::ShaderModule> shaderModule;
    std::unique_ptr<VkRenderer::CommandPool> commandPool;

    std::shared_ptr<VkRenderer::VertexBuffer> vertexBuffer;
    std::shared_ptr<VkRenderer::IndexBuffer> indexBuffer;
    std::shared_ptr<VkRenderer::UniformBufferDescriptor> UBOdescriptor;

    std::shared_ptr<VkRenderer::GraphicsPipeline> trianglePipeline;

    const std::vector<Extra::Vertex> vertices = {
        {{-0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}},
        {{0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}},
        {{0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}},
        {{-0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}}
    };

    const std::vector<uint16_t> indices = {
        0, 1, 2, 2, 3, 0
    };

    std::shared_ptr<VkRenderer::SwapChain> swapChain;

    std::unique_ptr<VkRenderer::Texture> texture;

    std::unique_ptr<VkRenderer::DrawCommandBuffer> commandBuffer;
    std::unique_ptr <VkRenderer::Syncher> syncher;

    void draw();
};