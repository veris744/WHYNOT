#include "AudioManager.h"

#include <fstream>
#include <thread>

#include "Utils/Logger.h"

std::shared_ptr<AudioManager> AudioManager::instance = nullptr;

AudioManager::AudioManager(const AudioManager&)
{
    
}

AudioManager::~AudioManager()
{
    for (const auto& buffer : soundBuffers) {
        alDeleteBuffers(1, &buffer.second);
    }
    if (context) {
        alcMakeContextCurrent(nullptr);
        alcDestroyContext(context);
        context = nullptr;
    }
    if (device) {
        alcCloseDevice(device);
        device = nullptr;
    }
}

std::shared_ptr<AudioManager> AudioManager::GetInstance()
{
    if (!instance)
    {
        instance = std::make_shared<AudioManager>(AudioManager());
    }
    return instance;
}

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

void AudioManager::PlaySound(const string& _filename, bool loop)
{
    ALuint buffer = LoadWav(_filename);
    if (buffer == 0) {
        Logger::Log(LogLevel::Error, "Failed to load sound: " + _filename);
        return;
    }

    ALuint source;
    alGenSources(1, &source);
    alSourcei(source, AL_BUFFER, buffer);
    
    // Set looping mode
    alSourcei(source, AL_LOOPING, loop ? AL_TRUE : AL_FALSE);

    alSourcePlay(source);

    // If the sound is not looping, delete the source after it finishes playing
    if (!loop) {
        thread([source]() {
            ALint state;
            do {
                alGetSourcei(source, AL_SOURCE_STATE, &state);
            } while (state == AL_PLAYING);

            alDeleteSources(1, &source);
        }).detach();
    }
}


ALuint AudioManager::LoadWav(const string& _filename)
{
    if (soundBuffers.find(_filename) != soundBuffers.end()) {
        return soundBuffers[_filename];
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

    soundBuffers[_filename] = buffer;
    return buffer;
}
