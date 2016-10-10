#pragma once

#include "Core/Memory.h"
#include "Sound.h"

namespace FireCube
{

class SoundDecoder : public RefCounted
{
public:
	SoundDecoder() {}
	virtual ~SoundDecoder() {}

	virtual unsigned int GetData(char *dest, unsigned int bytes) = 0;
};

}