#ifndef SOUND_H
#define SOUND_H

#include <SDL.h>

#include <map>
#include <string>

enum SoundPriority { NOPLAY,TICK, DONK, SCRAPE, SPLASH, POP, COLLAPSE, NUKE_SOUND };

struct Wav {
	SDL_AudioSpec spec;
    Uint32 length;
	Uint8 *buffer;
};

class Sound
{
public:
	Sound();
	~Sound();
	void Play();
	bool PlayASound(const char* name, SoundPriority priority);
private:
	void PlaySound(const char* name);
	SDL_AudioDeviceID deviceId;

	SoundPriority maxx;
	const char* sound;

    std::map<std::string,  Wav*> sounds;
};

#endif // SOUND_H
