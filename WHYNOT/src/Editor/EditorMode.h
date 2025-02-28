#pragma once
#include "Entities/Entity.h"

class EditorMode
{
    static Entity* selectedEntity;

public:
    static void SelectEntity(Entity* entity);
    static void Unselect();
    
};
