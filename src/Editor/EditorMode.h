#pragma once
#include "Entities/Entity.h"
#include "Utils/SingleDelegate.h"

class InputText;
class EntityPanel;

class EditorMode
{
    static Entity* selectedEntity;
    static EntityPanel* entityViewer;
    static InputText* inputText;
    
    static void CreateEntityPanel();
    static void CreateInputBox();

public:
    static bool isInputBoxOpen;
    static bool isPanelOpen;
    
    static void SelectEntity(Entity* entity);
    static void Unselect();

    static void SetEntityViewer();

    static void OpenInputBox();
    static void CloseInputBox(bool _saveValue);
    static void ProcessUserInput(int key);

    static SingleDelegate<const string&> OnEnterInput;

    static void ClearEditor();
};
