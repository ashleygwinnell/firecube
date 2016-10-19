#include <fstream>
#include <SDL.h>
#include "Audio/Sound.h"
#include "Utils/Logger.h"
#include "Utils/Filesystem.h"
#include "OggVorbisSoundDecoder.h"
#define STB_VORBIS_HEADER_ONLY
#include "ThirdParty/STB/stb_vorbis.c"

using namespace FireCube;

Sound::Sound(Engine *engine) : Resource(engine)
{

}

bool Sound::Load(const std::string &filename)
{	
	std::string resolvedFileName = Filesystem::FindResourceByName(filename);
	if (resolvedFileName.empty())
		return false;

	std::string ext = Filesystem::GetFileExtension(filename);
	if (ext == "wav")
	{
		return LoadWav(resolvedFileName);
	}
	else if (ext == "ogg")
	{
		return LoadOggVorbis(resolvedFileName);
	}

	return false;
}

bool Sound::LoadWav(const std::string &filename)
{
	needsDecoding = false;
	SDL_AudioSpec desired, *obtained;
	Uint32 wavLength;
	Uint8 *wavBuffer;

	SDL_zero(desired);
	desired.freq = 44100;
	desired.format = AUDIO_S16;
	desired.channels = 2;

	// Load the WAV
	if ((obtained = SDL_LoadWAV(filename.c_str(), &desired, &wavBuffer, &wavLength)) == nullptr)
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

bool Sound::LoadOggVorbis(const std::string &filename)
{
	std::ifstream file(filename, std::ios::binary);	

	file.seekg(0, std::ios_base::end);
	std::streampos fileSize = file.tellg();
	data.resize((unsigned int) fileSize);

	file.seekg(0, std::ios_base::beg);
	file.read(data.data(), fileSize);

	int error;
	stb_vorbis *decoder = stb_vorbis_open_memory((unsigned char*)data.data(), data.size(), &error, nullptr);
	if (!decoder)
	{
		return false;
	}

	stb_vorbis_info info = stb_vorbis_get_info(decoder);
	frequency = info.sample_rate;
	stereo = info.channels > 1;
	stb_vorbis_close(decoder);
	sixteenBit = true;
	needsDecoding = true;

	return true;
}

unsigned int Sound::GetFrequency() const
{
	return frequency;
}

void Sound::SetFrequency(unsigned int frequency)
{
	this->frequency = frequency;
}

bool Sound::IsSixteenBit() const
{
	return sixteenBit;
}

void Sound::SetSixteenBit(bool sixteenBit)
{
	this->sixteenBit = sixteenBit;
}

bool Sound::IsStereo() const
{
	return stereo;
}

void Sound::SetStereo(bool stereo)
{
	this->stereo = stereo;
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

void Sound::SetSize(unsigned int size)
{
	data.resize(size);
	end = data.data() + size;
}

bool Sound::NeedsDecoding() const
{
	return needsDecoding;
}

unsigned int Sound::GetSize() const
{
	return data.size();
}

SharedPtr<SoundDecoder> Sound::GetSoundDecoder()
{
	if (needsDecoding)
	{
		return new OggVorbisSoundDecoder(this);
	}
	else
	{
		return SharedPtr<SoundDecoder>();
	}
}
