#pragma once

#include <iostream>
#include <memory>
#include <vector>
#include "parent/layer.hpp"
#include "../swap_chain.hpp"
#include "../render_pass.hpp"
#include "../gui/gui.hpp"

class Interface : public Layer {
public:
    Interface(Extra::VkVars* vars, std::shared_ptr<VkRenderer::SwapChain> swapChain);

    void onStart() override {
        std::cout << "Drawing gui" << std::endl;
        m_gui = std::make_unique<VkRenderer::GUI>(variables, swapChain);
    }

    void onUpdate() override {
        m_gui->update();
    }

    void onDestroy() override {
    }
private:
    Extra::VkVars* variables;
    std::shared_ptr<VkRenderer::SwapChain> swapChain;

    std::unique_ptr<VkRenderer::GUI> m_gui;
};