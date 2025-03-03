#include "EditorMode.h"

#include "Components/Transform.h"
#include "Managers/World.h"
#include "UI/Containers/EntityView.h"

Entity* EditorMode::selectedEntity = nullptr;
EntityView* EditorMode::entityViewer = nullptr;

void EditorMode::SelectEntity(Entity* entity)
{
    if (selectedEntity)
    {
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
        entityViewer->ClearContent();
    }
}

void EditorMode::SetEntityViewer()
{
    if (!entityViewer)
    {
        entityViewer = dynamic_pointer_cast<EntityView>(World::GetInstance()->GetWidget("EditorEntityView")).get();
        if  (!entityViewer)
        {
            Logger::Log<EditorMode>(LogLevel::Warning, "Could not find entity viewer");
            return;
        }
    }
    if (selectedEntity)
    {
        entityViewer->SetEntity(selectedEntity);
        entityViewer->SetContent();
    }
}
