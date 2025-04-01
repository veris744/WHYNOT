#pragma once
#include <UI/Widget.h>

class Dropbox : public Widget
{
    static unsigned int counter;

    std::vector<Widget*> DroppingWidgets;
    Widget* titleWidget = nullptr;
    bool isDropped = false;

    void Initialize() override;

public:
    Dropbox(vec2 _pos, vec2 _size, const string& _name = "");
    Dropbox() = default;
    void AddDroppingWidget(const shared_ptr<Widget>& _widget);
    void SetTitleWidget(const shared_ptr<Widget>& _widget);
    void SetAutoName() override;

    void CalculateHeight();
    void OrganizeWidgets();
    void Drop();
    void UpdateParentDropboxes() const;
    bool IsDropped() const { return isDropped; };

    SingleDelegate<> OnDroppedDelegate;
};

