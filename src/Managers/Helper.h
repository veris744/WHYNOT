#pragma once

#include <glad/include/glad/glad.h>
#include <glfw/glfw3.h>
#include <vector>
#include "glm/glm.hpp"
#include "Utils/MultiDelegate.h"

using namespace glm;

class Helper
{
    static GLFWwindow* window;
    static vec3 sceneDimensions;
    static vec3 sceneCenter;

    static vec2 XBounds;
    static vec2 YBounds;
    static vec2 ZBounds;
    
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

    static vec3 GetSceneDimensions() { return sceneDimensions; }
    static vec3 GetSceneCenter() { return sceneCenter; }
    static vec2 GetXBounds() { return XBounds; }
    static vec2 GetYBounds() { return YBounds; }
    static vec2 GetZBounds() { return ZBounds; }

    // Generate sphere vertices
    static void generateSphere(std::vector<float>& vertices, std::vector<unsigned int>& indices, float radius, int sectors, int stacks);

    
    static MultiDelegate<> OnWindowResizeDelegate;

};
