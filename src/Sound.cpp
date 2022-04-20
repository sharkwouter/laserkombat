// sound.cpp
#include "Sound.h"

#include <string>

Sound::Sound() : sound(0), maxx(SoundPriority::NOPLAY) {
}

Sound::~Sound() {
    SDL_CloseAudioDevice(deviceId);
    SDL_FreeWAV(wavBuffer);
}

void Sound::Play()
{
	if (maxx==NOPLAY) return;
	PlaySound(NULL);
	if (sound) PlaySound(sound);
	maxx=NOPLAY;
	sound=NULL;
}

bool Sound::PlayASound(const char* name, SoundPriority priority)
{
	if (priority>maxx) {
		maxx=priority;
		sound=name;
		return true;
	}
	return false;
	return true;
}

void Sound::PlaySound(const char* name) {
    if(!name) {
        SDL_PauseAudioDevice(deviceId, 1);
        return;
    }
    std::string path = "assets/sounds/" + std::string(name);
    SDL_AudioSpec wavSpec;
    Uint32 wavLength;
    
    SDL_LoadWAV(path.c_str(), &wavSpec, &wavBuffer, &wavLength);
    deviceId = SDL_OpenAudioDevice(NULL, 0, &wavSpec, NULL, 0);
    SDL_QueueAudio(deviceId, wavBuffer, wavLength);
    SDL_PauseAudioDevice(deviceId, 0);
}

