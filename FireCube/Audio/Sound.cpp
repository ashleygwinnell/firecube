#include <SDL.h>
#include "Audio/Sound.h"
#include "Utils/Logger.h"
#include "Utils/Filesystem.h"

using namespace FireCube;

Sound::Sound(Engine *engine) : Resource(engine)
{

}

bool Sound::Load(const std::string &filename)
{	
	std::string resolvedFileName = Filesystem::FindResourceByName(filename);
	if (resolvedFileName.empty())
		return false;

	SDL_AudioSpec desired, *obtained;
	Uint32 wavLength;
	Uint8 *wavBuffer;

	SDL_zero(desired);
	desired.freq = 44100;
	desired.format = AUDIO_S16;
	desired.channels = 2;
		
	// Load the WAV
	if ((obtained = SDL_LoadWAV(resolvedFileName.c_str(), &desired, &wavBuffer, &wavLength)) == nullptr)
	{
		LOGERROR("Could not open wav file: ", SDL_GetError());
		return false;
	}
	
	frequency = obtained->freq;
	stereo = obtained->channels == 2;
	sixteenBit = (obtained->format == AUDIO_S16) || (obtained->format == AUDIO_U16);
	data.resize(wavLength);
	for (unsigned int i = 0; i < wavLength; ++i)
	{
		data[i] = wavBuffer[i];
	}
	end = data.data() + data.size();	
	SDL_FreeWAV(wavBuffer);

	return true;	
}



unsigned int Sound::GetFrequency() const
{
	return frequency;
}

bool Sound::IsSixteenBit() const
{
	return sixteenBit;
}

bool Sound::IsStereo() const
{
	return stereo;
}

char *Sound::GetStart()
{
	return data.data();
}

char *Sound::GetEnd()
{
	return end;
}

unsigned int Sound::GetSampleSize() const
{
	unsigned size = 1;
	if (sixteenBit)
		size <<= 1;
	if (stereo)
		size <<= 1;
	return size;
}
