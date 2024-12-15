// WHYNOT.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "Managers/Helper.h"
#include "Input/InputManager.h"
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
	
	World::GetInstance()->Initialize();
	Renderer::GetInstance()->Initialize();
	Renderer2D::GetInstance()->Initialize();
	InputManager::GetInstance()->Initialize();
	
	double lastTime = glfwGetTime();
	while (!glfwWindowShouldClose(Helper::GetWindow()))
	{
		float deltaTime = static_cast<float>(glfwGetTime() - lastTime);
		lastTime = glfwGetTime();
		
		InputManager::GetInstance()->Update(deltaTime);
		
		World::GetInstance()->Update();
		
		Helper::Draw();
		
		Renderer2D::GetInstance()->Render();
		Renderer2D::GetInstance()->Clear();
		
		Renderer::GetInstance()->Render();
		Renderer::GetInstance()->Clear();
		
		Helper::EndUpdate();
	}

	Helper::Terminate();
	return 0;
}


