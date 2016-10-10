#pragma once

#include "Utils/utils.h"
#include "Core/Component.h"

namespace FireCube
{

class Sound;
class SoundDecoder;

class FIRECUBE_API SoundEmitter : public Component
{
	FIRECUBE_OBJECT(SoundEmitter)
public:
	SoundEmitter(Engine *engine);
	SoundEmitter(Engine *engine, Sound *sound);
	SoundEmitter(const SoundEmitter &other);
	
	/**
	* clones the sound emitter
	*/
	virtual Component *Clone() const;
	
	/**
	* Sets the sound source for this emitter
	* @param sound The sound source
	*/	
	void SetSound(Sound *sound);
	
	/**
	* Returns the sound source of this emitter	
	*/
	Sound *GetSound() const;
	
	/**
	* Mixes the sound source to a target buffer
	* @param dest the desition buffer
	* @param samples The number of samples to mix
	* @param mixRate The sampling rate of the target buffer
	* @param stereo Specifies whether the target buffer is stereo  
	*/
	void Mix(int *dest, unsigned int samples, unsigned int mixRate, bool stereo);
	
	/**
	* Sets looping of the sound source
	* @param looped Whether to play the sound looped
	*/
	void SetLooped(bool looped);
	
	/**
	* @reutrns Whether the sound is looped
	*/
	bool GetLooped() const;
	
	/**
	* Sets the gain of the sound
	* @param gain The gain factor
	*/
	void SetGain(float gain);
	
	/**
	* Returns the gain of the sound
	*/
	float GetGain() const;
	
	/**
	* Starts playback
	*/
	void Play();
	
	/**
	* Starts playback using a given sound
	* @param sound The sound to use for playback
	*/
	void Play(Sound *sound);
	
	/**
	* Sets the panning of the sound
	* @param panning The panning value (-1 for left channel, 0 for center, 1 for right channel)
	*/
	void SetPanning(float panning);
	
	/**
	* @returns The panning of the sound
	*/
	float GetPanning() const;
private:
	void MixMonoToMono(int *dest, Sound *sound, int samples, unsigned int mixRate);
	void MixStereoToMono(int *dest, Sound *sound, int samples, unsigned int mixRate);
	void MixMonoToStereo(int *dest, Sound *sound, int samples, unsigned int mixRate);
	void MixStereoToStereo(int *dest, Sound *sound, int samples, unsigned int mixRate);
	void MixSilence(int *dest, int samples, unsigned int mixRate);
	virtual void MarkedDirty();
	virtual void NodeChanged();
	virtual void SceneChanged(Scene *oldScene);

	Sound *sound;	
	SharedPtr<SoundDecoder> soundDecoder;
	SharedPtr<Sound> decodedSoundBuffer;
	char *position;	
	int fractPosition;
	bool looped;
	float gain;
	float panning;
	unsigned int unusedDecodedBytes;
};

}