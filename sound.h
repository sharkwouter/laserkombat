//sound.h

enum SoundPriority { NOPLAY,TICK, DONK, SCRAPE, SPLASH, POP, COLLAPSE, NUKE_SOUND };

class Sound
{
public:
	Sound() : sound(0), maxx(NOPLAY) {}
	void Play();
	bool PlayASound(const char* name, SoundPriority priority);
private:
	SoundPriority maxx;
	const char* sound;


};
