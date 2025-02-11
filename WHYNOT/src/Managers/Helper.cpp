#include "Helper.h"


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
    Helper::windowHeight = height;
    Helper::windowWidth = width;
}


GLFWwindow* Helper::window = nullptr;
float Helper::windowHeight = 600;
float Helper::windowWidth = 800;

void Helper::InitializeGlfw()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GetWindow();
}

void Helper::InitializeGlad()
{
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        Logger::Log<Helper>(LogLevel::FatalError, "Failed to initialize GLAD");
    }

    glfwSetFramebufferSizeCallback(Helper::GetWindow(), framebuffer_size_callback);
}

GLFWwindow* Helper::GetWindow()
{
    if (window == nullptr)
    {
        window = glfwCreateWindow(windowWidth, windowHeight, "Window", NULL, NULL);
        if (window == nullptr)
        {
            Logger::Log<Helper>(LogLevel::FatalError, "Failed to initialize GLAD");
            glfwTerminate();
            return nullptr;
        }
        glfwMakeContextCurrent(window);
    }
    return window;
}

void Helper::EndUpdate(float deltaTime)
{
    glfwSwapBuffers(GetWindow());
    glfwPollEvents(); 
}

void Helper::Draw()
{
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Helper::Terminate()
{
    glfwTerminate();
}

void Helper::SetCursorVisible(bool _isVisible)
{
    glfwSetInputMode(window, GLFW_CURSOR, _isVisible ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);
}


