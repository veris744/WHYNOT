#pragma once
#include <string>
#include <unordered_map>
#include <AL/al.h>
#include <AL/alc.h>


class AudioManager
{
    static std::unordered_map<std::string, ALuint> loadedSounds;

    static ALCdevice* device;
    static ALCcontext* context;

public:
    static void Initialize();
    static void ShutDown();
    static ALuint LoadWav(const std::string& _filename);

};
