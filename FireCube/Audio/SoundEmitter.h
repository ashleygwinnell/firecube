#pragma once

#include "Utils/utils.h"
#include "Core/Component.h"

namespace FireCube
{

class Sound;

class FIRECUBE_API SoundEmitter : public Component
{
	FIRECUBE_OBJECT(SoundEmitter)
public:
	SoundEmitter(Engine *engine);
	SoundEmitter(Engine *engine, Sound *sound);
	SoundEmitter(const SoundEmitter &other);
	virtual Component *Clone() const;	
	void SetSound(Sound *sound);
	Sound *GetSound() const;
	void Mix(int *dest, unsigned int samples, unsigned int mixRate, bool stereo);
	void SetLooped(bool looped);
	bool GetLooped() const;
	void SetGain(float gain);
	float GetGain() const;
	void Play();
	void Play(Sound *sound);
	void SetPanning(float panning);
	float GetPanning() const;
private:
	void MixMonoToMono(int *dest, int samples, unsigned int mixRate);
	void MixStereoToMono(int *dest, int samples, unsigned int mixRate);
	void MixMonoToStereo(int *dest, int samples, unsigned int mixRate);
	void MixStereoToStereo(int *dest, int samples, unsigned int mixRate);
	void MixSilence(int *dest, int samples, unsigned int mixRate);
	virtual void MarkedDirty();
	virtual void NodeChanged();
	virtual void SceneChanged(Scene *oldScene);

	Sound *sound;
	char *position;	
	int fractPosition;
	bool looped;
	float gain;
	float panning;
};

}