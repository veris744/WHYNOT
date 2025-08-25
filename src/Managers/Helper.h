#pragma once

#include <glad/include/glad/glad.h>
#include <glfw/glfw3.h>
#include <vector>
#include "glm/glm.hpp"
#include "Utils/MultiDelegate.h"

class Helper
{
    static GLFWwindow* window;
    static glm::vec3 sceneDimensions;
    static glm::vec3 sceneCenter;

    static glm::vec2 XBounds;
    static glm::vec2 YBounds;
    static glm::vec2 ZBounds;
    
public:
    static float windowWidth;
    static float windowHeight;
    
    static void InitializeGlfw();
    static void InitializeGlad();
    static GLFWwindow* GetWindow();
    static void EndUpdate(float deltaTime);
    static void Draw();
    static void Terminate();
    static void SetCursorVisible(bool _isVisible);
    static void framebuffer_size_callback(GLFWwindow* window, int width, int height);

    static glm::vec3 GetSceneDimensions() { return sceneDimensions; }
    static glm::vec3 GetSceneCenter() { return sceneCenter; }
    static glm::vec2 GetXBounds() { return XBounds; }
    static glm::vec2 GetYBounds() { return YBounds; }
    static glm::vec2 GetZBounds() { return ZBounds; }

    // Generate sphere vertices
    static void generateSphere(std::vector<float>& vertices, std::vector<unsigned int>& indices, float radius, int sectors, int stacks);

    
    static MultiDelegate<> OnWindowResizeDelegate;

};
