#ifndef SOUND_H
#define SOUND_H

enum SoundPriority { NOPLAY,TICK, DONK, SCRAPE, SPLASH, POP, COLLAPSE, NUKE_SOUND };

class Sound
{
public:
	Sound() : sound(0), maxx(SoundPriority::NOPLAY) {}
	static void Play();
	static bool PlayASound(const char* name, SoundPriority priority);
private:
	SoundPriority maxx;
	const char* sound;
};

#endif // SOUND_H
