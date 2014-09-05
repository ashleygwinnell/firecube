#pragma  once

#include <map>
#include "Core/Resource.h"
#include "Rendering/Texture.h"
#include "Rendering/RenderingTypes.h"
#include "Math/Math.h"

class TiXmlElement;

namespace FireCube
{

enum RenderPathCommandType
{
	COMMAND_UNKNOWN, COMMAND_CLEAR, COMMAND_BASEPASS, COMMAND_LIGHTPASS, COMMAND_QUAD
};

class RenderPath;
class Shader;
class RenderSurface;

class FIRECUBE_API RenderPathCommand
{
public:
	RenderPathCommand(RenderPath *renderPath);
	bool Load(TiXmlElement *element, Engine *engine);
	static RenderPathCommandType stringToType(const std::string &type);

	RenderPath *renderPath;
	RenderPathCommandType type;
	bool useFogColor;
	vec3 clearColor;
	StringHash output;
	StringHash textures[static_cast<int>(TextureUnit::MAX_TEXTURE_UNITS)];
	StringHash pass;
	Shader *vertexShader;
	Shader *fragmentShader;	
};

class FIRECUBE_API RenderSurfaceDescriptor
{
public:
	int width, height;
	vec2 sizeMultiplier;
	bool hasSizeMultiplier;
	std::string name;
};

class FIRECUBE_API RenderPath : public Resource
{
public:
	RenderPath(Engine *engine);
	
	virtual bool Load(const std::string &filename);
	const std::vector<RenderPathCommand> &GetCommands() const;
	RenderPathCommand &GetCommand(int index);
	RenderSurface *GetRenderTarget(StringHash name);
	void AllocateRenderSurfaces();
private:
	std::map<StringHash, SharedPtr<RenderSurface>> renderTargets;
	std::vector<RenderPathCommand> commands;
	std::vector<RenderSurfaceDescriptor> renderSurfaceDescriptors;
};

}