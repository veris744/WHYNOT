#include "EditorMode.h"

#include "Components/Transform.h"
#include "Managers/World.h"
#include "UI/Containers/EntityPanel.h"
#include "UI/Text/InputText.h"

Entity* EditorMode::selectedEntity = nullptr;
EntityPanel* EditorMode::entityViewer = nullptr;
InputText* EditorMode::inputText = nullptr;
string EditorMode::lastInputText;
bool EditorMode::isInputBoxOpen = false;
bool EditorMode::isPanelOpen = false;
SingleDelegate<const string&> EditorMode::OnEnterInput;

void EditorMode::CreateEntityPanel()
{
    std::shared_ptr<EntityPanel> tempPanel = std::make_shared<EntityPanel>(vec2(0, 50), vec2(400, 0), "EntityPanel");
    tempPanel->background = {0.3f, 0.3f, 0.3f, 1.f};
    tempPanel->pixelCorrection = {200, 0};
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
    lastInputText = "";
    if (!inputText)
    {
        CreateInputBox();
    }
    inputText->isActive = true;
}

void EditorMode::AddInputChar(char c)
{
    inputText->UpdateText(c);
}

void EditorMode::CloseInputBox(bool _saveValue)
{
    isInputBoxOpen = false;
    if (inputText)
    {
        if (_saveValue)
        {
            lastInputText = inputText->ReturnText();
            OnEnterInput.Execute(lastInputText);
        }
        inputText->isActive = false;
        inputText->ClearText();
    }
    OnEnterInput.Clear();
}

