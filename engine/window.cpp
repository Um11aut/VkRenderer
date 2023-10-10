#include "window.hpp"

bool VkRenderer::Window::shouldClose()
{
	return glfwWindowShouldClose(m_window);
}

VkRenderer::Window::Window(int width, int height)
	: m_width(width), m_height(height)
{
	glfwInit();
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	m_window = glfwCreateWindow(m_width, m_height, "Renderer", nullptr, nullptr);
}

VkRenderer::Window::~Window()
{
	glfwDestroyWindow(m_window);
}

bool VkRenderer::Window::KeyPressed(int key)
{
	if (glfwGetKey(m_window, key) == GLFW_PRESS) {
		return true;
	}
	return false;
}
