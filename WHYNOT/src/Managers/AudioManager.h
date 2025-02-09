#pragma once
#include <memory>
#include <string>
#include <unordered_map>
#include <AL/al.h>
#include <AL/alc.h>

using namespace std;

class AudioManager
{
    static std::shared_ptr<AudioManager> instance;

    ALCdevice* device = nullptr;
    ALCcontext* context = nullptr;

    AudioManager() = default;
public:
    AudioManager(const AudioManager&);
    ~AudioManager();
    static std::shared_ptr<AudioManager> GetInstance();

    void Initialize();
    void PlaySound(const string& _filename, bool loop = false);


private:
    unordered_map<std::string, ALuint> soundBuffers;
    ALuint LoadWav(const string& _filename);
    
};
