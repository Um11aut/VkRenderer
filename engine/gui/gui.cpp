#include "gui.hpp"

VkRenderer::GUI::GUI(Extra::VkVars* vars, std::shared_ptr<VkRenderer::SwapChain> swapChain)
	: m_vars(vars), m_swapChain(swapChain)
{
	m_GuiRenderer = std::make_unique<Gui::GuiRenderer>(m_vars, m_swapChain);
}

VkRenderer::GUI::~GUI()
{
}
