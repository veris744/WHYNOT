#include "Audio.h"

void Audio::Initialize()
{
    for (const auto& audio : audios)
    {
        AddAudioSource(audio);
    }
    audios.clear();
}

void Audio::AddAudioSource(const AudioSource& audio)
{
    ALuint buffer = AudioManager::LoadWav(audio.filename);
    if (buffer == 0) {
        Logger::Log(LogLevel::Error, "Failed to load sound: " + audio.filename);
        return;
    }
    alGenSources(1, &audio.source);
    alSourcei(audio.source, AL_BUFFER, buffer);
    
    // Set looping mode
    alSourcei(audio.source, AL_LOOPING, audio.loops ? AL_TRUE : AL_FALSE);
        
    audioSources[audio.name] = audio;
}

void Audio::Play(const string& audioName)
{
    if (!audioSources.contains(audioName))
    {
        Logger::Log<Audio>(LogLevel::Error, audioName + " not found in Audio Component");
        return;
    }
    alSourcePlay(audioSources[audioName].source);
}
