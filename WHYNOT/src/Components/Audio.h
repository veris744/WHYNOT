#pragma once
#include "Component.h"
#include "includes.h"
#include "Managers/AudioManager.h"
#include "Reflection/Reflection.h"

struct AudioSource
{
    AudioSource() = default;
    AudioSource(const string& _name, const string& _filename, bool loops = false)
        : name(_name), filename(_filename), loops(loops) {};
    
    string name;
    string filename;
    bool loops;
    mutable ALuint source = 0;

    // float volume;
};
REGISTER_CLASS(AudioSource, {
    REGISTER_MEMBER(AudioSource, name),
    REGISTER_MEMBER(AudioSource, filename),
    REGISTER_MEMBER(AudioSource, loops)
});

class Audio : public Component
{
    unordered_map<string, AudioSource> audioSources;
    
public:
    Audio()
    {
        name = "AUDIO";
    }
    
    vector<AudioSource> audios;

    void Initialize() override;
    void Update(float deltaTime) override {};
    void AddAudioSource(const AudioSource& audio);
    void Play(const string& audioName);
};
REGISTER_CLASS(Audio);