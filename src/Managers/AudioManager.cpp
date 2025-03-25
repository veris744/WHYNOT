#include "AudioManager.h"

#include <fstream>

#include "Utils/Logger.h"

unordered_map<string, ALuint> AudioManager::loadedSounds;
ALCdevice* AudioManager::device = nullptr;
ALCcontext* AudioManager::context = nullptr;

void AudioManager::Initialize()
{
    device = alcOpenDevice(nullptr);
    if (!device) {
        Logger::Log(LogLevel::Error, "Failed to open OpenAL device");
        return;
    }

    context = alcCreateContext(device, nullptr);
    if (!context) {
        Logger::Log(LogLevel::Error, "Failed to create OpenAL context");
        alcCloseDevice(device);
        device = nullptr;
        return;
    }

    alcMakeContextCurrent(context);
}

void AudioManager::ShutDown()
{
    for (const auto& [name, source] : loadedSounds)
    {
        alDeleteSources(1, &source);
    }
}

ALuint AudioManager::LoadWav(const string& _filename)
{
    if (loadedSounds.find(_filename) != loadedSounds.end()) {
        return loadedSounds[_filename];
    }

    ifstream file(_filename, std::ios::binary);
    if (!file) {
        Logger::Log(LogLevel::Error, "Failed to load wav file");
        return 0;
    }

    char header[44];
    file.read(header, 44); // Read WAV header

    // Extract WAV format details (assuming PCM format)
    int dataSize = *reinterpret_cast<int*>(&header[40]);
    int sampleRate = *reinterpret_cast<int*>(&header[24]);
    short channels = *reinterpret_cast<short*>(&header[22]);
    short bitsPerSample = *reinterpret_cast<short*>(&header[34]);

    ALenum format;
    if (channels == 1 && bitsPerSample == 8) format = AL_FORMAT_MONO8;
    else if (channels == 1 && bitsPerSample == 16) format = AL_FORMAT_MONO16;
    else if (channels == 2 && bitsPerSample == 8) format = AL_FORMAT_STEREO8;
    else if (channels == 2 && bitsPerSample == 16) format = AL_FORMAT_STEREO16;
    else {
        Logger::Log(LogLevel::Error, "Unsupported WAV format");
        return 0;
    }

    // Read audio data
    vector<char> data(dataSize);
    file.read(data.data(), dataSize);

    ALuint buffer;
    alGenBuffers(1, &buffer);
    alBufferData(buffer, format, data.data(), dataSize, sampleRate);

    loadedSounds[_filename] = buffer;
    return buffer;
}
