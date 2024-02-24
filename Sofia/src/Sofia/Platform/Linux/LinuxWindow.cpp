#include <pch.h>
#include "LinuxWindow.h"

#include "Sofia/Application.h"

#include "Sofia/Events/ApplicationEvents.h"
#include "Sofia/Events/MouseEvents.h"
#include "Sofia/Events/KeyEvents.h"

#include <glad/glad.h>
#include <stb_image.h>

namespace Sofia {

	LinuxWindow::LinuxWindow(const WindowProps& props)
	{
		Init(props);
	}
	LinuxWindow::~LinuxWindow()
	{
		Shutdown();
	}

	void LinuxWindow::OnUpdate()
	{
		Application::Get().GetGraphicsContext()->SwapBuffers(this);
	}

	std::optional<int> LinuxWindow::ProcessEvents()
	{
		glfwPollEvents();
		return {};
	}

	void LinuxWindow::BindWindow() noexcept
	{
		Application::Get().GetGraphicsContext()->BindWindow(this);
	}
	void LinuxWindow::Clear(const glm::vec4& color) noexcept
	{
		Application::Get().GetGraphicsContext()->Clear(this, color);
	}

	void LinuxWindow::SetTitle(const std::string& title)
	{
		glfwSetWindowTitle(m_Window, title.c_str());
		m_Data.title;
	}
	void LinuxWindow::SetVSync(bool vsync) noexcept
	{
		glfwSwapInterval(vsync ? 1 : 0);
		m_Data.vSync = vsync;
	}

	void LinuxWindow::Minimize() noexcept
	{
		m_Data.minimized = true;
		glfwIconifyWindow(m_Window);
	}
	void LinuxWindow::Maximize() noexcept
	{
		m_Data.maximized = true;
		glfwMaximizeWindow(m_Window);
	}
	void LinuxWindow::Restore() noexcept
	{
		m_Data.maximized = false;
		glfwRestoreWindow(m_Window);
	}

	void LinuxWindow::SetIcon(const std::filesystem::path& iconPath)
	{
		std::string path = iconPath.string();
		if (std::filesystem::exists(iconPath))
		{
			GLFWimage image;
			int channels;
			image.pixels = stbi_load(path.c_str(), &image.width, &image.height, &channels, 4);
			glfwSetWindowIcon(m_Window, 1, &image);
			stbi_image_free(image.pixels);
		}
	}

	static void GLFWErrorCallback(int error, const char* message)
	{
		SOF_CORE_ERROR("GLFWError {0}: {1}", error, message);
	}
	void LinuxWindow::Init(const WindowProps& props)
	{
		m_Data.title = props.Title;
		m_Data.width = props.Width;
		m_Data.height = props.Height;
		m_Data.eventCallback = SOF_BIND_EVENT_FN(LinuxWindow::DefaultEventCallback);

		int success = glfwInit();
		SOF_CORE_ASSERT(success, "Failed to initialize GLFW");
		glfwSetErrorCallback(GLFWErrorCallback);

		glfwWindowHint(GLFW_RESIZABLE, props.Resizable);
		glfwWindowHint(GLFW_DECORATED, props.HasTitleBar);
		glfwWindowHint(GLFW_MAXIMIZED, props.Maximized);

		m_Window = glfwCreateWindow((int)m_Data.width, (int)m_Data.height, m_Data.title.c_str(), nullptr, nullptr);
		SOF_CORE_ASSERT(m_Window, "Could not create GLFW window");
		SOF_CORE_INFO("Creating window {0}, ({1}, {2})", m_Data.title, m_Data.width, m_Data.height);

		glfwMakeContextCurrent(m_Window);
		glfwSetWindowUserPointer(m_Window, &m_Data);
		glfwSwapInterval(m_Data.vSync ? 1 : 0);

		Application::Get().GetGraphicsContext()->InitForWindow(this);

		glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window)
		{
			auto& data = *(WindowData*)glfwGetWindowUserPointer(window);
			WindowCloseEvent e;
			data.eventCallback(e);
		});
		glfwSetWindowPosCallback(m_Window, [](GLFWwindow* window, int x, int y)
		{
			auto& data = *(WindowData*)glfwGetWindowUserPointer(window);
			data.pos = { x, y };

			WindowMovedEvent e(x, y);
			data.eventCallback(e);
		});
		glfwSetWindowFocusCallback(m_Window, [](GLFWwindow* window, int focused)
		{
			auto& data = *(WindowData*)glfwGetWindowUserPointer(window);
			switch (focused)
			{
				case GLFW_TRUE:
				{
					WindowFocusEvent e;
					data.eventCallback(e);
					break;
				}
				case GLFW_FALSE:
				{
					WindowLostFocusEvent e;
					data.eventCallback(e);
					break;
				}
			}
		});
		glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height)
		{
			auto& data = *(WindowData*)glfwGetWindowUserPointer(window);
			data.width = width;
			data.height = height;

			WindowResizeEvent e(width, height);
			data.eventCallback(e);
		});
		glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
		{
			auto& data = *(WindowData*)glfwGetWindowUserPointer(window);
			KeyCode code = GetKeyCodeFromGLFW(key);
			switch (action)
			{
				case GLFW_PRESS:
				{
					data.keyboard.OnKeyPressed(static_cast<uint8_t>(code));
					KeyPressedEvent e(code, 0);
					data.eventCallback(e);
					break;
				}
				case GLFW_REPEAT:
				{
					data.keyboard.OnKeyPressed(static_cast<uint8_t>(code));
					KeyPressedEvent e(static_cast<KeyCode>(key), 1);
					data.eventCallback(e);
					break;
				}
				case GLFW_RELEASE:
				{
					data.keyboard.OnKeyReleased(static_cast<uint8_t>(code));
					KeyReleasedEvent e(static_cast<KeyCode>(key));
					data.eventCallback(e);
					break;
				}
			}
		});
		glfwSetCharCallback(m_Window, [](GLFWwindow* window, uint32_t code)
		{
			auto& data = *(WindowData*)glfwGetWindowUserPointer(window);
			data.keyboard.OnChar(static_cast<char>(code));
			KeyTypedEvent e(code);
			data.eventCallback(e);
		});
		glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods)
		{
			auto& data = *(WindowData*)glfwGetWindowUserPointer(window);
			switch (action)
			{
				case GLFW_PRESS:
				{
					if (button == GLFW_MOUSE_BUTTON_LEFT)
						data.mouse.OnLeftPressed(0, 0);
					else if (button == GLFW_MOUSE_BUTTON_RIGHT)
						data.mouse.OnRightPressed(0, 0);
					MouseButtonPressedEvent e(static_cast<MouseCode>(button));
					data.eventCallback(e);
					break;
				}
				case GLFW_RELEASE:
				{
					if (button == GLFW_MOUSE_BUTTON_LEFT)
						data.mouse.OnLeftReleased(0, 0);
					else if (button == GLFW_MOUSE_BUTTON_RIGHT)
						data.mouse.OnRightReleased(0, 0);

					if (!data.mouse.IsInWindow())
					{
						MouseLeftEvent e;
						data.eventCallback(e);
					}

					MouseButtonReleasedEvent e(static_cast<MouseCode>(button));
					data.eventCallback(e);
					break;
				}
			}
		});
		glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xpos, double ypos)
		{
			auto& data = *(WindowData*)glfwGetWindowUserPointer(window);
			data.mouse.OnMouseMove((int)xpos, (int)ypos);
			MouseMovedEvent e((float)xpos, (float)ypos);
			data.eventCallback(e);
		});
		glfwSetCursorEnterCallback(m_Window, [](GLFWwindow* window, int entered)
		{
			auto& data = *(WindowData*)glfwGetWindowUserPointer(window);
			if (entered)
			{
				data.mouse.OnMouseEnter();
				MouseEnteredEvent e;
				data.eventCallback(e);
			}
			else
			{
				data.mouse.OnMouseLeave();
				if (!(data.mouse.IsLeftButtonPressed() || data.mouse.IsRightButtonPressed()))
				{
					MouseLeftEvent e;
					data.eventCallback(e);
				}
			}
		});
		glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xoffset, double yoffset)
		{
			auto& data = *(WindowData*)glfwGetWindowUserPointer(window);
			data.mouse.OnWheelDelta(0, 0, (int)yoffset);
			MouseScrolledEvent e((float)xoffset, (float)yoffset);
			data.eventCallback(e);
		});

		int width, height;
		glfwGetWindowSize(m_Window, &width, &height);
		m_Data.width = width;
		m_Data.height = height;
		int xpos, ypos;
		glfwGetWindowPos(m_Window, &xpos, &ypos);
		m_Data.pos = { xpos, ypos };
	}
	void LinuxWindow::Shutdown()
	{
		glfwDestroyWindow(m_Window);
	}
}

