#pragma once

#include "Sofia/Renderer/GraphicsContext.h"

#include <glfw3.h>

namespace Sofia {

    class OpenGLContext : public GraphicsContext
    {
    public:
        SOF_CORE OpenGLContext(void* window);
        SOF_CORE ~OpenGLContext() = default;

        SOF_CORE virtual void Init() override {}
        SOF_CORE void InitWindow();
    private:
        GLFWwindow* m_Window = nullptr;
    };
}
