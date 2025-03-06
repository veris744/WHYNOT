#pragma once
#include "Entities/Entity.h"

class InputText;
class EntityPanel;

class EditorMode
{
    static Entity* selectedEntity;
    static EntityPanel* entityViewer;
    static InputText* inputText;
    static string lastInputText;
    
    static void CreateEntityPanel();
    static void CreateInputBox();

public:
    static bool isInputBoxOpen;
    static bool isPanelOpen;
    
    static void SelectEntity(Entity* entity);
    static void Unselect();

    static void SetEntityViewer();

    static void OpenInputBox();
    static void AddInputChar(char c);
    static void CloseInputBox(bool _saveValue);
    static string GetLastInputText() { return lastInputText; }

    static SingleDelegate<const string&> OnEnterInput;
};
