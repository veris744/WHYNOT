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
    static void EndUpdate();
    static void Draw();
    static void Terminate();
};
