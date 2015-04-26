#pragma once

#include <mutex>
#include "Utils/utils.h"


namespace FireCube
{

class SoundEmitter;

class FIRECUBE_API Audio
{
public:
	Audio();
	~Audio();

	void Init();
	void Play();
	void AddSoundEmitter(SoundEmitter *soundEmitter);
	void RemoveSoundEmitter(SoundEmitter *soundEmitter);
	void Mix(void *dest, unsigned int samples);
	unsigned int GetSampleSize() const;
	std::mutex &GetMutex();
private:	
	
	unsigned int deviceId;
	bool isPlaying;
	bool stereo;
	unsigned int sampleSize;
	unsigned int mixRate;
	std::vector<SoundEmitter *> soundEmitters;
	std::vector<int> mixBuffer;
	std::mutex mutex;
};

}
