#ifndef TEXTURE_MANAGER_H
#define TEXTURE_MANAGER_H

#pragma warning(push)
#pragma warning(disable:4251)

class FIRECUBE_API TextureResource
{
public:
	TextureResource();
	~TextureResource();

	bool Load(const string &filename);
	bool IsValid();	
	GLuint id;
};	

typedef ResourceManager<TextureResource> TextureManager;
typedef boost::shared_ptr<TextureResource> Texture;

#pragma warning(pop)
#endif