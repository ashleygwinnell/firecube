#include "OggVorbisSoundDecoder.h"
#include "ThirdParty/STB/stb_vorbis.c"

using namespace FireCube;

OggVorbisSoundDecoder::OggVorbisSoundDecoder(Sound *source) : source(source)
{
	data = source->GetStart();
	int error;
	decoder = stb_vorbis_open_memory((unsigned char *)data, source->GetSize(), &error, nullptr);
}

OggVorbisSoundDecoder::~OggVorbisSoundDecoder()
{
	if (decoder)
	{
		stb_vorbis_close(decoder);
		decoder = nullptr;
	}
}

unsigned int OggVorbisSoundDecoder::GetData(char *dest, unsigned int bytes)
{
	unsigned int channels = source->IsStereo() ? 2 : 1;
	unsigned int writtenSamples = (unsigned int) stb_vorbis_get_samples_short_interleaved(decoder, channels, (short*)dest, bytes >> 1);
	unsigned int writtenBytes = (writtenSamples * channels) << 1;

	if (writtenBytes < bytes)
	{
		bytes -= writtenBytes;
		stb_vorbis_seek_start(decoder);
		writtenSamples = (unsigned int) stb_vorbis_get_samples_short_interleaved(decoder, channels, (short*)(dest + writtenBytes), bytes >> 1);
		writtenBytes += (writtenSamples * channels) << 1;
	}

	return writtenBytes;
}

