#include "Audio/SoundEmitter.h"
#include "Audio/Sound.h"
#include "Core/Engine.h"
#include "Audio/Audio.h"

using namespace FireCube;

SoundEmitter::SoundEmitter(Engine *engine) : Component(engine), sound(nullptr), looped(false), gain(1.0f), panning(0.0f)
{

}

SoundEmitter::SoundEmitter(const SoundEmitter &other) : Component(other), sound(other.sound), looped(other.looped), gain(other.gain), panning(other.panning), 
														position(other.position), fractPosition(other.fractPosition)
{

}

Component *SoundEmitter::Clone() const
{
	return new SoundEmitter(*this);
}

void SoundEmitter::MarkedDirty()
{

}

void SoundEmitter::NodeChanged()
{

}

void SoundEmitter::SceneChanged(Scene *oldScene)
{
	if (oldScene)
	{
		engine->GetAudio()->RemoveSoundEmitter(this);
	}

	if (scene)
	{
		engine->GetAudio()->AddSoundEmitter(this);
	}
}

void SoundEmitter::SetSound(Sound *sound)
{
	this->sound = sound;
	position = nullptr;	
}

Sound *SoundEmitter::GetSound() const
{
	return sound;
}

void SoundEmitter::Play()
{
	position = sound->GetStart();
	fractPosition = 0;
}

void SoundEmitter::Mix(int *dest, unsigned int samples, unsigned int mixRate, bool stereo)
{
	if (position == nullptr || !sound)
		return;
	
	if (stereo)
	{
		if (sound->IsStereo())
		{
			MixStereoToStereo(dest, samples, mixRate);
		}
		else
		{
			MixMonoToStereo(dest, samples, mixRate);
		}
	}
	else
	{
		if (sound->IsStereo())
		{
			MixStereoToMono(dest, samples, mixRate);
		}
		else
		{
			MixMonoToMono(dest, samples, mixRate);
		}
	}	
}

template <typename T>
void IncPositionMono(T *& position, int &fractPosition, int intInc, int fractInc, bool looped, T *start, T *end)
{
	position += intInc;
	fractPosition += fractInc;
	if (fractPosition > 65535)
	{
		fractPosition &= 65535;
		++position;
	}
	if (position >= end)
	{
		if (looped)
		{
			position = start;
		}
		else
		{
			position = nullptr;
		}
	}
}

template <typename T>
void IncPositionStereo(T *& position, int &fractPosition, int intInc, int fractInc, bool looped, T *start, T *end)
{
	position += (intInc << 1);
	fractPosition += fractInc;
	if (fractPosition > 65535)
	{
		fractPosition &= 65535;
		position += 2;
	}
	if (position >= end)
	{
		if (looped)
		{
			position = start;
		}
		else
		{
			position = nullptr;
		}
	}
}

void SoundEmitter::MixMonoToMono(int *dest, int samples, unsigned int mixRate)
{
	float finalGain = gain;
	int vol = (int)(256.0f * finalGain + 0.5f);	

	if (vol == 0)
	{
		MixSilence(dest, samples, mixRate);
	}

	float freqRatio = (float)sound->GetFrequency() / (float)mixRate;
	int intInc = (int)freqRatio;
	int fractInc = (int)((freqRatio - std::floor(freqRatio)) * 65536.0f);	

	if (sound->IsSixteenBit())
	{
		short *pos = (short *)position;
		short *end = (short *)sound->GetEnd();
		while (samples--)
		{
			*dest = *dest + (*pos * vol) / 256;
			++dest;			

			IncPositionMono(pos, fractPosition, intInc, fractInc, looped, (short *)sound->GetStart(), end);
			if (pos == nullptr)
				break;			
		}

		position = (char*)pos;
	}
	else
	{
		char *pos = position;
		char *end = sound->GetEnd();
		while (samples--)
		{
			*dest = *dest + *pos * vol;
			++dest;
			
			IncPositionMono(pos, fractPosition, intInc, fractInc, looped, sound->GetStart(), end);
			if (pos == nullptr)
				break;
		}

		position = (char*)pos;
	}	
}

void SoundEmitter::MixStereoToMono(int *dest, int samples, unsigned int mixRate)
{
	float finalGain = gain;
	int vol = (int)(256.0f * finalGain + 0.5f);

	if (vol == 0)
	{
		MixSilence(dest, samples, mixRate);
	}

	float freqRatio = (float)sound->GetFrequency() / (float)mixRate;
	int intInc = (int)freqRatio;
	int fractInc = (int)((freqRatio - std::floor(freqRatio)) * 65536.0f);	

	if (sound->IsSixteenBit())
	{
		short *pos = (short *)position;
		short *end = (short *)sound->GetEnd();
		while (samples--)
		{
			int m = ((int)pos[0] + (int)pos[1]) / 2;
			*dest = *dest + (m * vol) / 256;
			++dest;
			
			IncPositionStereo(pos, fractPosition, intInc, fractInc, looped, (short *) sound->GetStart(), end);
			if (pos == nullptr)
				break;			
		}

		position = (char*)pos;
	}
	else
	{
		char *pos = position;
		char *end = sound->GetEnd();
		while (samples--)
		{
			int m = ((int)pos[0] + (int)pos[1]) / 2;
			*dest = *dest + m * vol;
			++dest;
			
			IncPositionStereo(pos, fractPosition, intInc, fractInc, looped, sound->GetStart(), end);
			if (pos == nullptr)
				break;
		}

		position = (char*)pos;
	}	
}

void SoundEmitter::MixMonoToStereo(int *dest, int samples, unsigned int mixRate)
{
	float finalGain = gain;
	int leftVol = (int)((-panning + 1.0f) * (256.0f * finalGain + 0.5f));
	int rightVol = (int)((panning + 1.0f) * (256.0f * finalGain + 0.5f));

	if (leftVol == 0 && rightVol == 0)
	{
		MixSilence(dest, samples, mixRate);
	}

	float freqRatio = (float)sound->GetFrequency() / (float)mixRate;
	int intInc = (int)freqRatio;
	int fractInc = (int)((freqRatio - std::floor(freqRatio)) * 65536.0f);	

	if (sound->IsSixteenBit())
	{
		short *pos = (short *)position;
		short *end = (short *)sound->GetEnd();
		while (samples--)
		{
			*dest = *dest + (*pos * leftVol) / 256;
			++dest;
			*dest = *dest + (*pos * rightVol) / 256;
			++dest;

			IncPositionMono(pos, fractPosition, intInc, fractInc, looped, (short *)sound->GetStart(), end);
			if (pos == nullptr)
				break;
		}

		position = (char*)pos;
	}
	else
	{
		char *pos = position;
		char *end = sound->GetEnd();
		while (samples--)
		{
			*dest = *dest + *pos * leftVol;
			++dest;
			*dest = *dest + *pos * rightVol;
			++dest;

			IncPositionMono(pos, fractPosition, intInc, fractInc, looped, sound->GetStart(), end);
			if (pos == nullptr)
				break;
		}

		position = (char*)pos;
	}	
}

void SoundEmitter::MixStereoToStereo(int *dest, int samples, unsigned int mixRate)
{
	float finalGain = gain;
	int leftVol = (int)((-panning + 1.0f) * (256.0f * finalGain + 0.5f));
	int rightVol = (int)((panning + 1.0f) * (256.0f * finalGain + 0.5f));

	if (leftVol == 0 && rightVol == 0)
	{
		MixSilence(dest, samples, mixRate);
	}

	float freqRatio = (float)sound->GetFrequency() / (float)mixRate;
	int intInc = (int)freqRatio;
	int fractInc = (int)((freqRatio - std::floor(freqRatio)) * 65536.0f);	

	if (sound->IsSixteenBit())
	{
		short *pos = (short *)position;
		short *end = (short *)sound->GetEnd();
		while (samples--)
		{
			*dest = *dest + (pos[0] * leftVol) / 256;
			++dest;
			*dest = *dest + (pos[1] * rightVol) / 256;
			++dest;

			IncPositionStereo(pos, fractPosition, intInc, fractInc, looped, (short *)sound->GetStart(), end);
			if (pos == nullptr)
				break;
		}

		position = (char*)pos;
	}
	else
	{
		char *pos = position;
		char *end = sound->GetEnd();
		while (samples--)
		{
			*dest = *dest + pos[0] * leftVol;
			++dest;
			*dest = *dest + pos[1] * rightVol;
			++dest;

			IncPositionStereo(pos, fractPosition, intInc, fractInc, looped, sound->GetStart(), end);
			if (pos == nullptr)
				break;
		}

		position = (char*)pos;
	}	
}

void SoundEmitter::SetLooped(bool looped)
{
	this->looped = looped;
}

bool SoundEmitter::GetLooped() const
{
	return looped;
}

void SoundEmitter::SetGain(float gain)
{
	this->gain = std::max(gain, 0.0f);
}
float SoundEmitter::GetGain() const
{
	return gain;
}

void SoundEmitter::MixSilence(int *dest, int samples, unsigned int mixRate)
{
	float freqRatio = (float) sound->GetFrequency() / (float)mixRate;
	float sampleCount = (float)samples * freqRatio;
	int intSampleCount = (int)sampleCount;
	int fractSampleCount = (int)((sampleCount - std::floor(sampleCount)) * 65536.0f);
	unsigned int sampleSize = sound->GetSampleSize();

	fractPosition += fractSampleCount;
	if (fractPosition > 65535)
	{
		fractPosition &= 65535;
		position += sampleSize;
	}
	position += intSampleCount * sampleSize;

	if (position > sound->GetEnd())
	{
		if (looped)
		{
			position = sound->GetStart();			
		}
		else
		{
			position = nullptr;
		}
	}
}

void SoundEmitter::SetPanning(float panning)
{
	this->panning = Clamp(panning, -1.0f, 1.0f);
}

float SoundEmitter::GetPanning() const
{
	return panning;
}

void SoundEmitter::Play(Sound *sound)
{
	SetSound(sound);
	Play();
}