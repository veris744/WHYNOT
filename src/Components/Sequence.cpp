#include "Sequence.h"

#include <Entities/Entity.h>

#include "Transform.h"

void Sequence::SetSequence(std::vector<State> _states)
{
    states = _states;
    for (const auto& state : states) {
        totalTime += state.time;
    }
}

void Sequence::Play()
{
    isPlaying = true;
}

void Sequence::Pause()
{
    isPlaying = false;
}

void Sequence::Reset()
{
    isPlaying = false;
    currentState = 0;
}

void Sequence::Update(float deltaTime)
{
    if (states.empty())
    {
        return;
    }

    if (!transform)
    {
        transform = parent->GetComponent<Transform>();
        if (!transform)
        {
            Logger::Log(LogLevel::Error, "Sequence Comp cannot find parent's transform");
        }
    }

    if (isPlaying)
    {
        static float accumulatedTime = 0.0f;
        accumulatedTime += deltaTime;

        // Find the current state and next state based on accumulated time
        unsigned int prevState = 0;

        // Handle looping
        if (isLooping && accumulatedTime >= totalTime) {
            accumulatedTime = fmod(accumulatedTime, totalTime);
        }

        // If we've reached the end and not looping, stop playing
        if (!isLooping && accumulatedTime >= totalTime) {
            isPlaying = false;
            // Snap to final state
            const State& finalState = states.back();
            transform->position = finalState.position;
            transform->SetRotation(finalState.rotation);
            transform->scale = finalState.scale;
            return;
        }

        // Find the current state segment
        float currentSegmentTime = 0.0f;
        unsigned int nextState = 0;

        for (size_t i = 0; i < states.size() - 1; ++i) {
            if (accumulatedTime >= currentSegmentTime &&
                accumulatedTime < currentSegmentTime + states[i].time) {
                prevState = i;
                nextState = i + 1;
                break;
            }
            currentSegmentTime += states[i].time;
        }

        // Calculate interpolation factor (0 to 1)
        float segmentStartTime = currentSegmentTime;
        float segmentEndTime = currentSegmentTime + states[prevState].time;
        float t = (accumulatedTime - segmentStartTime) / (segmentEndTime - segmentStartTime);
        t = glm::clamp(t, 0.0f, 1.0f);

        // Interpolate between states
        const State& stateA = states[prevState];
        const State& stateB = states[nextState];

        glm::vec3 interpolatedPos = glm::mix(stateA.position, stateB.position, t);
        glm::vec3 interpolatedRot = glm::mix(stateA.rotation, stateB.rotation, t);
        glm::vec3 interpolatedScale = glm::mix(stateA.scale, stateB.scale, t);

        // Apply to transform
        transform->position = interpolatedPos;
        transform->SetRotation(interpolatedRot);
        transform->scale = interpolatedScale;

        // Update current state index (optional, for tracking purposes)
        currentState = nextState;
    }
}
