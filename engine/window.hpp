#pragma once

#include <glfw/glfw3.h>
#include <string>

namespace VkRenderer {
	class Window {
	private:
		GLFWwindow* m_window;
		int m_width, m_height;

		const std::string m_appName;

		Window(const Window&) = delete;
		Window operator=(const Window&) = delete;

	public:
		bool shouldClose();

		explicit Window(int width, int height, std::string appName);
		~Window();

		inline int GetWidth() const { return m_width; }
		inline int GetHeight() const { return m_height; }

		bool KeyPressed(int key);
	};
}