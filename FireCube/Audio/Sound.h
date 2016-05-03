#pragma once

#include "Utils/utils.h"
#include "Core/Resource.h"

namespace FireCube
{

class FIRECUBE_API Sound : public Resource
{
	FIRECUBE_OBJECT(Sound)
public:
	Sound(Engine *engine);
	
	/**
	* Loads a WAV file
	* @param filename The path to the file
	* @returns true if the fil was loaded successfully
	*/
	virtual bool Load(const std::string &filename);
	
	/**
	* Returns the sampling frequency of the file
	*/
	unsigned int GetFrequency() const;
	
	/**
	* Returns whehter each sample is 16-bit
	*/
	bool IsSixteenBit() const;
	
	/**
	* Returns whether this is a stereo sound
	*/
	bool IsStereo() const;
	
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
private:
	std::vector<char> data;
	unsigned int frequency;		
	bool sixteenBit;	
	bool stereo;	
	char *end;
};

}