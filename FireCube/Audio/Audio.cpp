#include <SDL.h>
#include "Utils/Logger.h"
#include "Audio/Audio.h"
#include "Audio/SoundEmitter.h"

using namespace FireCube;

void SDLAudioCallback(void *userdata, Uint8 *stream, int len);

Audio::Audio() : isPlaying(false), deviceId(0)
{

}

Audio::~Audio()
{
	if (deviceId)
	{
		SDL_CloseAudioDevice(deviceId);
		deviceId = 0;
	}
}

void Audio::Init()
{
	SDL_AudioSpec desired, obtained;	

	SDL_zero(desired);
	desired.freq = 44100;
	desired.format = AUDIO_S16;
	desired.channels = 2;
	desired.samples = 4096;
	desired.callback = SDLAudioCallback;
	desired.userdata = this;

	deviceId = SDL_OpenAudioDevice(NULL, 0, &desired, &obtained, SDL_AUDIO_ALLOW_ANY_CHANGE & ~SDL_AUDIO_ALLOW_FORMAT_CHANGE);
	if (deviceId == 0) 
	{
		LOGERROR("Failed to open audio: ", SDL_GetError());
	}	

	stereo = obtained.channels == 2;
	sampleSize = stereo ? sizeof(int) : sizeof(short);
	mixRate = obtained.freq;
	mixBuffer.resize(stereo ? obtained.samples << 1 : obtained.samples);

	Play();
}

void Audio::Play()
{
	if (isPlaying)
	{
		return;
	}

	SDL_PauseAudioDevice(deviceId, 0);
	isPlaying = true;
}

void SDLAudioCallback(void *userdata, Uint8 *stream, int len)
{		
	Audio *audio = static_cast<Audio *>(userdata);
	audio->Mix(stream, len / audio->GetSampleSize());	
}

void Audio::Mix(void *dest, unsigned int samples)
{
	std::lock_guard<std::mutex> lock(mutex);	

	if (isPlaying == false)
	{
		memset(dest, 0, samples * sampleSize);
		return;
	}

	memset(mixBuffer.data(), 0, mixBuffer.size() * sizeof(int));
	
	for (auto soundEmitter : soundEmitters)
	{		
		soundEmitter->Mix(mixBuffer.data(), samples, mixRate, stereo);
	}
	
	short* destPtr = (short*)dest;
	for (unsigned int i = 0; i < mixBuffer.size(); ++i)
	{
		*destPtr++ = Clamp(mixBuffer[i], -32768, 32767);
	}
}

unsigned int Audio::GetSampleSize() const
{
	return sampleSize;
}

void Audio::AddSoundEmitter(SoundEmitter *soundEmitter)
{
	std::lock_guard<std::mutex> lock(mutex);
	soundEmitters.push_back(soundEmitter);
}

void Audio::RemoveSoundEmitter(SoundEmitter *soundEmitter)
{
	std::lock_guard<std::mutex> lock(mutex);
	soundEmitters.erase(std::remove(soundEmitters.begin(), soundEmitters.end(), soundEmitter), soundEmitters.end());
}

std::mutex &Audio::GetMutex()
{
	return mutex;
}