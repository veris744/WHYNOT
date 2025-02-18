#include "Helper.h"

#include "Input/InputManager.h"


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
    Helper::windowHeight = height;
    Helper::windowWidth = width;
}


GLFWwindow* Helper::window = nullptr;
float Helper::windowHeight = 600;
float Helper::windowWidth = 800;

vec3 Helper::sceneDimensions = vec3(30, 30, 30);
vec3 Helper::sceneCenter = vec3(0, 0, 0);
vec2 Helper::XBounds = vec2(sceneCenter.x - sceneDimensions.x * 0.5f, sceneCenter.x + sceneDimensions.x * 0.5f);
vec2 Helper::YBounds = vec2(sceneCenter.y - sceneDimensions.y * 0.5f, sceneCenter.y + sceneDimensions.y * 0.5f);
vec2 Helper::ZBounds = vec2(sceneCenter.z - sceneDimensions.z * 0.5f, sceneCenter.z + sceneDimensions.z * 0.5f);

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


void Helper::generateSphere(std::vector<float>& vertices, std::vector<unsigned int>& indices, float radius, int sectors,
                            int stacks)
{
    const float PI = 3.14159265359f;

    // Vertices
    for (int stack = 0; stack <= stacks; ++stack) {
        float stackAngle = PI / 2 - stack * PI / stacks; // From PI/2 to -PI/2
        float xy = radius * cosf(stackAngle);           // r * cos(phi)
        float z = radius * sinf(stackAngle);            // r * sin(phi)

        for (int sector = 0; sector <= sectors; ++sector) {
            float sectorAngle = 2 * PI * sector / sectors; // From 0 to 2PI

            // Vertex position (x, y, z)
            float x = xy * cosf(sectorAngle); // r * cos(phi) * cos(theta)
            float y = xy * sinf(sectorAngle); // r * cos(phi) * sin(theta)
            vertices.push_back(x);
            vertices.push_back(y);
            vertices.push_back(z);

            // Normalized normals (x, y, z)
            vertices.push_back(x / radius);
            vertices.push_back(y / radius);
            vertices.push_back(z / radius);

            // Texture coordinates (u, v)
            float u = (float)sector / sectors;
            float v = (float)stack / stacks;
            vertices.push_back(u);
            vertices.push_back(v);
        }
    }

    // Indices
    for (int stack = 0; stack < stacks; ++stack) {
        for (int sector = 0; sector < sectors; ++sector) {
            int first = stack * (sectors + 1) + sector;
            int second = first + sectors + 1;

            // Triangle 1
            indices.push_back(first);
            indices.push_back(second);
            indices.push_back(first + 1);

            // Triangle 2
            indices.push_back(second);
            indices.push_back(second + 1);
            indices.push_back(first + 1);
        }
    }
}



