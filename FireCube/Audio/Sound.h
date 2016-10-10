#pragma once

#include "Utils/utils.h"
#include "Core/Resource.h"

namespace FireCube
{

class SoundDecoder;

class FIRECUBE_API Sound : public Resource
{
	FIRECUBE_OBJECT(Sound)
public:
	Sound(Engine *engine);
	
	/**
	* Loads a sound file
	* @param filename The path to the file
	* @returns true if the file was loaded successfully
	*/
	virtual bool Load(const std::string &filename);
	
	/**
	* Returns the sampling frequency of the file
	*/
	unsigned int GetFrequency() const;

	void SetFrequency(unsigned int frequency);
	
	/**
	* Returns whether each sample is 16-bit
	*/
	bool IsSixteenBit() const;

	void SetSixteenBit(bool sixteenBit);
	
	/**
	* Returns whether this is a stereo sound
	*/
	bool IsStereo() const;
	
	void SetStereo(bool stereo);
	/**
	* Returns a pointer to the start of the sound buffer
	*/
	char *GetStart();
	
	/**
	* Returns a pointer to the end of the sound buffer
	*/
	char *GetEnd();
	
	/**
	* Returns the sample size in bytes
	*/
	unsigned int GetSampleSize() const;

	/**
	* Allocates data
	* @param size The number of bytes to allocate
	*/
	void SetSize(unsigned int size);

	bool NeedsDecoding() const;

	unsigned int GetSize() const;

	SharedPtr<SoundDecoder> GetSoundDecoder();
private:

	bool LoadWav(const std::string &filename);
	bool LoadOggVorbis(const std::string &filename);
	std::vector<char> data;
	unsigned int frequency;		
	bool sixteenBit;	
	bool stereo;	
	bool needsDecoding;
	char *end;
};

}