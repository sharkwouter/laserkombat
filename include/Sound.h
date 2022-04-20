#ifndef SOUND_H
#define SOUND_H

#include <SDL.h>

enum SoundPriority { NOPLAY,TICK, DONK, SCRAPE, SPLASH, POP, COLLAPSE, NUKE_SOUND };

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

	Uint8 *wavBuffer = NULL;
};

#endif // SOUND_H
