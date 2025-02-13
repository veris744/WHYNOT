#pragma once
#include <includes.h>


class Helper
{
    static GLFWwindow* window;
    
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

    // Generate sphere vertices
    static void generateSphere(std::vector<float>& vertices, std::vector<unsigned int>& indices, float radius, int sectors, int stacks);

};
