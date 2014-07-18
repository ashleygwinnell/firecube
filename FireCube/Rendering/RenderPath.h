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
	COMMAND_UNKNOWN, COMMAND_CLEAR, COMMAND_SCENEPASS, COMMAND_LIGHTPASS, COMMAND_QUAD
};

class RenderPath;
class Shader;

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
	StringHash textures[MAX_TEXTURE_UNITS];
	Shader *vertexShader;
	Shader *fragmentShader;
};

class FIRECUBE_API RenderPath : public Resource
{
public:
	RenderPath(Engine *engine);
	~RenderPath();
	virtual bool Load(const std::string &filename);
	const std::vector<RenderPathCommand> &GetCommands() const;
	RenderPathCommand &GetCommand(int index);
	Texture *GetRenderTarget(StringHash name);
private:
	std::map<StringHash, Texture *> renderTargets;
	std::vector<RenderPathCommand> commands;
};

}