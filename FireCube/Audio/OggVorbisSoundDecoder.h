#pragma once

#include "SoundDecoder.h"

struct stb_vorbis;

namespace FireCube
{

class OggVorbisSoundDecoder : public SoundDecoder
{
public:
	OggVorbisSoundDecoder(Sound *source);
	~OggVorbisSoundDecoder();

	virtual unsigned int GetData(char *dest, unsigned int bytes) override;

private:
	Sound *source;
	stb_vorbis *decoder;
	char *data;
};

}