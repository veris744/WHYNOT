// WHYNOT.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

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
		float deltaTime = static_cast<float>(glfwGetTime() - lastTime);
		lastTime = glfwGetTime();

		Helper::Draw();
		World::GetInstance()->PrepareLoad();
		
		InputManager::GetInstance()->Update(deltaTime);
		
		if (World::IsSceneLoaded())
		{
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


