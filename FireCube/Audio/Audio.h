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

	/**
	* Initializes the audio sytsem
	*/
	void Init();
	
	/**
	* Starts playback
	*/
	void Play();
	
	/**
	* Adds a sound emitter
	* @param soundEmitter The sounds emitter	
	*/
	void AddSoundEmitter(SoundEmitter *soundEmitter);
	
	/**
	* Removes a sound emitter
	* @param soundEmitter The sound emitter
	*/
	void RemoveSoundEmitter(SoundEmitter *soundEmitter);
	
	/**
	* Main mixing function.
	*/
	void Mix(void *dest, unsigned int samples);
	
	/**
	* Returns the sample size in bytes (2 for mono, 4 for stereo)
	*/
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
