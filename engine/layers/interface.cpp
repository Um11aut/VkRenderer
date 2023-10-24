#include "interface.hpp"

Interface::Interface(Extra::VkVars* vars, std::shared_ptr<VkRenderer::SwapChain> swapChain)
	: variables(vars), swapChain(swapChain)
{
}
