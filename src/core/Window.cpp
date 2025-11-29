#include "Window.h"
#include "Logger.h"
#include <glad/glad.h>

Window::Window(int width, int height, const std::string& title) {
    m_Data.Width = width;
    m_Data.Height = height;
    m_Data.Title = title;

    if (!glfwInit()) {
        LOG_ERROR("Failed to initialize GLFW");
        return;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    m_Window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
    if (!m_Window) {
        LOG_ERROR("Failed to create GLFW window");
        glfwTerminate();
        return;
    }

    glfwMakeContextCurrent(m_Window);
    glfwSwapInterval(1);
    
    // Set user pointer for callbacks
    glfwSetWindowUserPointer(m_Window, this);
    
    // Set framebuffer resize callback
    glfwSetFramebufferSizeCallback(m_Window, FramebufferSizeCallback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        LOG_ERROR("Failed to initialize GLAD");
    }
}

void Window::FramebufferSizeCallback(GLFWwindow* window, int width, int height) {
    Window* win = static_cast<Window*>(glfwGetWindowUserPointer(window));
    if (win) {
        win->m_Data.Width = width;
        win->m_Data.Height = height;
    }
}

Window::~Window() {
    glfwDestroyWindow(m_Window);
    glfwTerminate();
}

void Window::OnUpdate() {
    glfwPollEvents();
}

void Window::SwapBuffers() {
    glfwSwapBuffers(m_Window);
}

