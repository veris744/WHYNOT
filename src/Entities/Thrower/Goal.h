#pragma once
#include <unordered_set>
#include <Entities/Entity.h>


class Material;

class Goal : public Entity
{
    static unsigned int counter;
    std::string defaultName = "Goal";

    std::set<Entity*> entitiesEntering;
    std::set<Entity*> entitiesInside;

    Material* material;

public:
    Goal(const std::string& _name = "")
    {
        name = _name.empty() ? defaultName + std::to_string(++counter) : _name;
    }
    ~Goal() override = default;

    void Initialize() override;
    void Update(float _deltaTime) override;
    void ClearComponents() override;

    void OnCollision(Entity* _otherEntity, glm::vec3 normal) override;
};
REGISTER_CLASS(Goal, {
    REGISTER_MEMBER(Goal, name, MemberProperty::Viewable | MemberProperty::Serializable)
});

