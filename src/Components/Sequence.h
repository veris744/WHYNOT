#pragma once
#include <queue>
#include <vec3.hpp>
#include <Reflection/Reflection.h>

#include "Component.h"

class Transform;

class Sequence : public Component
{
    Transform* transform;

    bool isPlaying = false;
    unsigned int currentState = 0;

    float totalTime = 0.f;

public:

    struct State
    {
        glm::vec3 position;
        glm::vec3 rotation;
        glm::vec3 scale;

        // time in seconds
        // TODO add time class
        float time;
    };

    void SetSequence(std::vector<State> _states);
    void Play();
    void Pause();
    void Reset();

    void Update(float deltaTime) override;

    std::vector<State> states;

    bool isLooping = false;
    bool playOnStart = false;

};

REGISTER_CLASS(Sequence,
    REGISTER_MEMBER(Sequence, name, MemberProperty::Serializable),
    REGISTER_MEMBER(Sequence, isLooping, MemberProperty::Viewable | MemberProperty::Editable | MemberProperty::Serializable),
    REGISTER_MEMBER(Sequence, playOnStart, MemberProperty::Viewable | MemberProperty::Editable | MemberProperty::Serializable)
);