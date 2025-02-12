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

	AudioManager::GetInstance()->Initialize();
	World::GetInstance()->Initialize();
	Renderer::GetInstance()->Initialize();
	Renderer2D::GetInstance()->Initialize();
	TextRenderer::GetInstance()->InitFreeType();
	TextRenderer::GetInstance()->LoadFont();
	
	double lastTime = glfwGetTime();
	while (!glfwWindowShouldClose(Helper::GetWindow()))
	{
		float deltaTime = static_cast<float>(glfwGetTime() - lastTime);
		lastTime = glfwGetTime();

		Helper::Draw();
		World::GetInstance()->Prepare();
		
		InputManager::GetInstance()->Update(deltaTime);
		World::GetInstance()->Update(deltaTime);

		if (World::IsSceneLoaded())
		{
			Renderer::GetInstance()->Render();
			Renderer::GetInstance()->Clear();
		
			Renderer2D::GetInstance()->Render();
			Renderer2D::GetInstance()->Clear();
		}
		
		World::GetInstance()->Clean();
		Helper::EndUpdate(deltaTime);
	}

	Helper::Terminate();
	return 0;
}


