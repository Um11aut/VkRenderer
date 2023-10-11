#include "window.hpp"

bool VkRenderer::Window::shouldClose()
{
	return glfwWindowShouldClose(m_window);
}

VkRenderer::Window::Window(int width, int height, std::string app_name)
	: m_width(width), m_height(height), m_appName(app_name)
{
	glfwInit();
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
	m_window = glfwCreateWindow(m_width, m_height, m_appName.c_str(), nullptr, nullptr);
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
