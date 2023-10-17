#pragma once

#include <glfw/glfw3.h>
#include <string>
#include "extra/extra.hpp"

namespace VkRenderer {
	class Window {
	private:
		GLFWwindow* m_window;
		int m_width, m_height;

		Extra::AppInfo m_info;

		Window(const Window&) = delete;
		Window operator=(const Window&) = delete;

	public:
		bool shouldClose();

		explicit Window(Extra::AppInfo& info);

		inline int GetWidth() const { return m_width; }
		inline int GetHeight() const { return m_height; }
		inline GLFWwindow* getWindow() { return m_window; }
		inline Extra::AppInfo getInfo() { return m_info; }

		void close();

		bool KeyPressed(int key);
	};
}