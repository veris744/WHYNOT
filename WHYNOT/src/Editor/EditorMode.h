#pragma once
#include "Entities/Entity.h"

class EntityPanel;

class EditorMode
{
    static Entity* selectedEntity;
    static EntityPanel* entityViewer;

public:
    static void SelectEntity(Entity* entity);
    static void Unselect();

    static void SetEntityViewer();
};
