#pragma once

#include "Utils/utils.h"
#include "Core/Resource.h"

namespace FireCube
{

class FIRECUBE_API Sound : public Resource
{
	OBJECT(Sound)
public:
	Sound(Engine *engine);
	virtual bool Load(const std::string &filename);
	unsigned int GetFrequency() const;
	bool IsSixteenBit() const;
	bool IsStereo() const;
	char *GetStart();
	char *GetEnd();
	unsigned int GetSampleSize() const;
private:
	std::vector<char> data;
	unsigned int frequency;		
	bool sixteenBit;	
	bool stereo;	
	char *end;
};

}