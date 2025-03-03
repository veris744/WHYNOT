#pragma once
#include "Entities/Entity.h"

class EntityView;

class EditorMode
{
    static Entity* selectedEntity;
    static EntityView* entityViewer;

public:
    static void SelectEntity(Entity* entity);
    static void Unselect();

    static void SetEntityViewer();
};
