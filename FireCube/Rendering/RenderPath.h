#include <map>
#include "Core/Resource.h"
#include "Rendering/Texture.h"
#include "Math/Math.h"

class TiXmlElement;

namespace FireCube
{

enum RenderPathCommandType
{
	COMMAND_UNKNOWN, COMMAND_CLEAR, COMMAND_SCENEPASS, COMMAND_LIGHTPASS
};

class RenderPathCommand
{
public:
	RenderPathCommand();
	bool Load(TiXmlElement *element);	
	static RenderPathCommandType stringToType(const std::string &type);

	RenderPathCommandType type;
	bool useFogColor;
	vec3 clearColor;
};

class RenderPath : public Resource
{
public:
	RenderPath(Engine *engine);
	virtual bool Load(const std::string &filename);
	const std::vector<RenderPathCommand> &GetCommands() const;
	RenderPathCommand &GetCommand(int index);
private:
	std::map<StringHash, Texture *> renderTargets;
	std::vector<RenderPathCommand> commands;
};

}