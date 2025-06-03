// WHYNOT.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <Physics/CollisionManager.h>

#include "Managers/TextRenderer.h"
#include "Managers/Helper.h"
#include "Input/InputManager.h"
#include "Managers/AudioManager.h"
#include "Managers/World.h"
#include "Managers/Renderer.h"
#include "Managers/Renderer2D.h"

int main()
{
    Helper::InitializeGlfw();
    if (Helper::GetWindow() == nullptr)
    {
        return -1;
    }

    Helper::InitializeGlad();
    AudioManager::Initialize();
    World::GetInstance()->Initialize();
    Renderer::instance().Initialize();
    Renderer2D::instance().Initialize();
    TextRenderer::InitFreeType();
    TextRenderer::instance().LoadFont();

    double lastTime = glfwGetTime();
    while (!glfwWindowShouldClose(Helper::GetWindow()))
    {
        double currentTime = glfwGetTime();
        float deltaTime = static_cast<float>(currentTime - lastTime);
        lastTime = currentTime;
        if (deltaTime > 0.1f)
            deltaTime = 0.016f;

        Helper::Draw();
        World::GetInstance()->PrepareLoad();

        if (World::IsSceneLoaded())
        {
            CollisionManager::CheckCollisions();
            InputManager::GetInstance()->Update(deltaTime);
            World::GetInstance()->Update(deltaTime);

            Renderer::instance().Render();
            Renderer::instance().Clear();

            Renderer2D::instance().Render();
            Renderer2D::instance().Clear();
        }

        World::GetInstance()->Clean();
        Helper::EndUpdate(deltaTime);
    }

    AudioManager::ShutDown();
    Helper::Terminate();
    return 0;
}

