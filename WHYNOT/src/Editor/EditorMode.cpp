#include "EditorMode.h"

#include "Components/Transform.h"
#include "Managers/World.h"
#include "UI/Containers/EntityPanel.h"

Entity* EditorMode::selectedEntity = nullptr;
EntityPanel* EditorMode::entityViewer = nullptr;

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
}

void EditorMode::SetEntityViewer()
{
    if (!entityViewer)
    {
        entityViewer = dynamic_pointer_cast<EntityPanel>(World::GetInstance()->GetWidget("EntityPanel")).get();
        if  (!entityViewer)
        {
            Logger::Log<EditorMode>(LogLevel::Warning, "Could not find entity panel");
            return;
        }
    }
    if (selectedEntity)
    {
        entityViewer->SetEntity(selectedEntity);
        entityViewer->SetContent();
    }
}
