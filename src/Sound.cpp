// sound.cpp
#include "Sound.h"

#include <string>

Sound::Sound() : sound(0), maxx(SoundPriority::NOPLAY) {
    SDL_AudioSpec * spec = new SDL_AudioSpec{};
    spec->freq = 22050;
    spec->format = 8;
    spec->channels = 1;
    spec->samples = 4096;
    deviceId = SDL_OpenAudioDevice(NULL, 0, spec, NULL, 0);
}

Sound::~Sound() {
    std::map<std::string, Wav*>::iterator it;
    for (it = sounds.begin(); it != sounds.end(); it++) {
        SDL_FreeWAV(it->second->buffer);
        delete it->second;
    }
    SDL_CloseAudioDevice(deviceId);
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
        SDL_ClearQueuedAudio(deviceId);
        return;
    }
    std::string file = name; 
    if(!sounds.contains(file)) {
        Wav * wav = new Wav{};
        std::string path = "assets/sounds/" + file;
        SDL_LoadWAV(path.c_str(), &wav->spec, &wav->buffer, &wav->length);
        sounds[file] = std::move(wav);
    }

    SDL_QueueAudio(deviceId, sounds[file]->buffer, sounds[file]->length);
    SDL_PauseAudioDevice(deviceId, 0);
}

