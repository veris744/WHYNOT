#include "ThrowerPlayerController.h"
#include <Managers/World.h>
#include "ThrowerManager.h"

ThrowerPlayerController::ThrowerPlayerController()
{
    throwerManager = static_cast<ThrowerManager*>(World::GetInstance()->GetGameManager());
    canJump = false;
};

void ThrowerPlayerController::ProcessInput(int key, bool press)
{
    switch (key)
    {
    case GLFW_MOUSE_BUTTON_1:
        if (press)
            throwerManager->ChargeBall();
        else
            throwerManager->ThrowBall();
        break;
    case GLFW_MOUSE_BUTTON_2:
        if (press)
            throwerManager->GrabBall();
        else
            throwerManager->ReleaseBall();
        break;
    }
}
