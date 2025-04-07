#include "EditorMode.h"

#include <Components/PlayerController.h>
#include <Managers/ConfigurationValues.h>

#include "Components/Transform.h"
#include "Managers/World.h"
#include "UI/Containers/EntityPanel.h"
#include "UI/Text/InputText.h"
#include "UI/Buttons/SerializeButton.h"

Entity* EditorMode::selectedEntity = nullptr;
EntityPanel* EditorMode::entityViewer = nullptr;
InputText* EditorMode::inputText = nullptr;
SerializeButton* EditorMode::serializeButton = nullptr;
bool EditorMode::isInputBoxOpen = false;
bool EditorMode::isPanelOpen = false;
SingleDelegate<const string&> EditorMode::OnEnterInput;

void EditorMode::CreateEntityPanel()
{
    std::shared_ptr<EntityPanel> tempPanel = std::make_shared<EntityPanel>(vec2(-50, 0), vec2(350, 0), "EntityPanel");
    tempPanel->background = {0.3f, 0.3f, 0.3f, 1.f};
    tempPanel->pixelCorrection = {175, 0};
    tempPanel->autoSizing = AutoSizing::VERTICAL;
    World::GetInstance()->AddWidget(tempPanel);

    entityViewer = tempPanel.get();
    entityViewer->SetContent();
    entityViewer->isActive = true;
    isPanelOpen = true;
}

void EditorMode::CreateInputBox()
{
    std::shared_ptr<InputText> tempInput = std::make_shared<InputText>();
    inputText = tempInput.get();
    World::GetInstance()->AddWidget(tempInput);
    inputText->isActive = false;
}

void EditorMode::CreateSerializeButton()
{
    std::shared_ptr<SerializeButton> tempButton = std::make_shared<SerializeButton>(vec2{40,-40}, vec2{150, 50}, "SerializeButton");
    serializeButton = tempButton.get();
    World::GetInstance()->AddWidget(tempButton);
}

void EditorMode::EnterEditorMode()
{
    ConfigurationValues::IsEditorOpen = true;
    ConfigurationValues::ArePhysicsActive = false;
    ConfigurationValues::CanPlayerLook = false;
    ConfigurationValues::CanPlayerMove = true;
    ConfigurationValues::IsUIActive = true;

    World::GetInstance()->GetEditorPlayer()->GetComponent<Transform>()->position = World::GetInstance()->GetPlayer()->GetComponent<Transform>()->position;
    World::GetInstance()->GetEditorPlayer()->GetComponent<Transform>()->SetRotation(World::GetInstance()->GetPlayer()->GetComponent<Transform>()->rotation.vector());
    World::GetInstance()->GetEditorPlayer()->isActive = true;
    World::GetInstance()->SetCurrentCamera("EditorPlayer");


    Helper::SetCursorVisible(true);

    if (World::GetGameManager())
    {
        World::GetGameManager()->EndGame();
    }
    for (const auto& widget : World::GetInstance()->GetWidgets())
    {
        if (widget->type == WidgetType::Game)
        {
            widget->SetActiveWithChildren(false);
        }
    }
    World::GetInstance()->Pause();

    if (!entityViewer)
    {
        CreateEntityPanel();
    }
    if (!serializeButton)
    {
        CreateSerializeButton();
    }
    if (!inputText)
    {
        CreateInputBox();
    }
}

void EditorMode::ExitEditorMode()
{
    ClearEditor();

    ConfigurationValues::IsEditorOpen = false;

    World::GetInstance()->GetEditorPlayer()->isActive = false;
    World::GetInstance()->SetCurrentCamera("Player");

    if (World::GetGameManager())
    {
        World::GetGameManager()->PrepareGame();
        World::GetGameManager()->StartGame();
    }

    for (const auto& widget : World::GetInstance()->GetWidgets())
    {
        if (widget->type == WidgetType::Game)
        {
            widget->SetActiveWithChildren(true);
        }
    }
    World::GetInstance()->Resume();
}

void EditorMode::SelectEntity(Entity* entity)
{
    if (selectedEntity && selectedEntity == entity)
    {
        return;
    }

    entityViewer->ShowEntities(false);

    if (selectedEntity)
    {
        entityViewer->ClearPropertiesContent();
        entityViewer->Reorganize();
        if (Transform* tempTransform = selectedEntity->GetComponent<Transform>())
        {
            tempTransform->debugEnabled = false;
        }
    }
    selectedEntity = entity;
    selectedEntity->debugEnabled = true;
    if (Transform* tempTransform = selectedEntity->GetComponent<Transform>())
    {
        tempTransform->debugEnabled = true;
    }
    SetEntityViewer();
}

void EditorMode::Unselect()
{
    if (selectedEntity)
    {
        if (Transform* tempTransform = selectedEntity->GetComponent<Transform>())
        {
            tempTransform->debugEnabled = false;
        }
        selectedEntity = nullptr;
        entityViewer->ClearPropertiesContent();
        entityViewer->ShowEntities(true);
    }
}

void EditorMode::SetEntityViewer()
{
    if (selectedEntity)
    {
        entityViewer->SetContent(selectedEntity);
    }
}

void EditorMode::OpenInputBox()
{
    isInputBoxOpen = true;
    inputText->isActive = true;
}

void EditorMode::CloseInputBox(bool _saveValue)
{
    isInputBoxOpen = false;
    if (inputText)
    {
        if (_saveValue)
        {
            OnEnterInput.Execute(inputText->ReturnText());
        }
        inputText->isActive = false;
        inputText->ClearText();
    }
    OnEnterInput.Clear();
}

void EditorMode::ProcessUserInput(int key)
{
    if (key == GLFW_KEY_ESCAPE)
    {
        if (isInputBoxOpen)
        {
            CloseInputBox(false);
            return;
        }
    }
    if (isInputBoxOpen)
    {
        if ((key >= GLFW_KEY_A && key <= GLFW_KEY_Z) ||
            (key >= GLFW_KEY_0 && key <= GLFW_KEY_9) ||
            key == GLFW_KEY_COMMA || key == GLFW_KEY_PERIOD)
        {
            inputText->UpdateText(static_cast<char>(key));
        }
        else if(key == GLFW_KEY_SLASH || key == GLFW_KEY_KP_SUBTRACT)
        {
            inputText->UpdateText('-');
        }
        else if (key == GLFW_KEY_BACKSPACE)
        {
            inputText->RemoveLastCharacter();
        }
        else if (key == GLFW_KEY_ENTER)
        {
            CloseInputBox(true);
        }
    }
}

void EditorMode::ClearEditor()
{
    selectedEntity = nullptr;
    if (inputText)
    {
        inputText->Destroy();
        inputText = nullptr;
    }
    if (entityViewer)
    {
        entityViewer->ClearContent();
        entityViewer->Destroy();
        entityViewer = nullptr;
    }
    if (serializeButton)
    {
        serializeButton->Destroy();
        serializeButton = nullptr;
    }
}

