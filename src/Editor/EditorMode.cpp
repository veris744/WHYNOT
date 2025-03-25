#include "EditorMode.h"

#include "Components/Transform.h"
#include "Managers/World.h"
#include "UI/Containers/EntityPanel.h"
#include "UI/Text/InputText.h"

Entity* EditorMode::selectedEntity = nullptr;
EntityPanel* EditorMode::entityViewer = nullptr;
InputText* EditorMode::inputText = nullptr;
bool EditorMode::isInputBoxOpen = false;
bool EditorMode::isPanelOpen = false;
SingleDelegate<const string&> EditorMode::OnEnterInput;

void EditorMode::CreateEntityPanel()
{
    std::shared_ptr<EntityPanel> tempPanel = std::make_shared<EntityPanel>(vec2(0, 50), vec2(300, 0), "EntityPanel");
    tempPanel->background = {0.3f, 0.3f, 0.3f, 1.f};
    tempPanel->pixelCorrection = {150, 0};
    tempPanel->autoSizing = AutoSizing::VERTICAL;
    World::GetInstance()->AddWidget(tempPanel);

    entityViewer = tempPanel.get();
}

void EditorMode::CreateInputBox()
{
    std::shared_ptr<InputText> tempInput = std::make_shared<InputText>();
    inputText = tempInput.get();
    World::GetInstance()->AddWidget(tempInput);
}

void EditorMode::SelectEntity(Entity* entity)
{
    if (selectedEntity)
    {
        if (entityViewer)
            entityViewer->ClearContent();
        selectedEntity->debugEnabled = false;
        selectedEntity->GetComponent<Transform>()->debugEnabled = false;
    }
    selectedEntity = entity;
    selectedEntity->debugEnabled = true;
    selectedEntity->GetComponent<Transform>()->debugEnabled = true;
    SetEntityViewer();
    isPanelOpen = true;
}

void EditorMode::Unselect()
{
    if (selectedEntity)
    {
        selectedEntity->debugEnabled = false;
        selectedEntity->GetComponent<Transform>()->debugEnabled = false;
        selectedEntity = nullptr;
        if (entityViewer)
            entityViewer->ClearContent();
    }
    isPanelOpen = false;
}

void EditorMode::SetEntityViewer()
{
    if (!entityViewer)
    {
        CreateEntityPanel();
    }
    if (selectedEntity)
    {
        entityViewer->SetEntity(selectedEntity);
        entityViewer->SetContent();
    }
}

void EditorMode::OpenInputBox()
{
    isInputBoxOpen = true;
    if (!inputText)
    {
        CreateInputBox();
    }
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
        CloseInputBox(false);
    }
    else if ((key >= GLFW_KEY_A && key <= GLFW_KEY_Z) ||
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

void EditorMode::ClearEditor()
{
    selectedEntity = nullptr;
    inputText = nullptr;
    entityViewer = nullptr;
}

