#pragma once
#include <UI/Containers/MemberView.h>

#include "Entities/Entity.h"
#include "Utils/SingleDelegate.h"

class InputText;
class EntityPanel;
class SerializeButton;

class EditorMode
{
    static Entity* selectedEntity;
    static EntityPanel* entityViewer;
    static InputText* inputText;
    static SerializeButton* serializeButton;
    
    static void CreateEntityPanel();
    static void CreateInputBox();
    static void CreateSerializeButton();

public:
    static bool isInputBoxOpen;
    static bool isPanelOpen;

    static void EnterEditorMode();
    static void ExitEditorMode();
    
    static void SelectEntity(Entity* entity);
    static void Unselect();

    static void SetEntityViewer();

    static void OpenInputBox();
    static void CloseInputBox(bool _saveValue);
    static void ProcessUserInput(int key);

    static SingleDelegate<const string&> OnEnterInput;

    static void ClearEditor();

    static const Entity* GetSelectedEntity() { return selectedEntity; }
};
