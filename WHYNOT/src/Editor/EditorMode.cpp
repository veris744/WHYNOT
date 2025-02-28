#include "EditorMode.h"

#include "Components/Transform.h"

Entity* EditorMode::selectedEntity = nullptr;

void EditorMode::SelectEntity(Entity* entity)
{
    if (selectedEntity)
    {
        selectedEntity->debugEnabled = false;
        selectedEntity->GetComponent<Transform>()->debugEnabled = false;
    }
    selectedEntity = entity;
    selectedEntity->debugEnabled = true;
    selectedEntity->GetComponent<Transform>()->debugEnabled = true;
}

void EditorMode::Unselect()
{
    if (selectedEntity)
    {
        selectedEntity->debugEnabled = false;
        selectedEntity->GetComponent<Transform>()->debugEnabled = false;
        selectedEntity = nullptr;
    }
}
