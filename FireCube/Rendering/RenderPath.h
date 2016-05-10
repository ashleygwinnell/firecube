#pragma  once

#include <map>
#include "Core/Resource.h"
#include "Rendering/Texture.h"
#include "Rendering/RenderingTypes.h"
#include "Math/Math.h"
#include "Core/Variant.h"

class TiXmlElement;

namespace FireCube
{

enum class RenderPathCommandType
{
	UNKNOWN, CLEAR, BASEPASS, LIGHTPASS, QUAD
};

class RenderPath;
class Shader;
class RenderSurface;

/**
* This class represents a command in a render path
*/
class FIRECUBE_API RenderPathCommand
{
public:
	RenderPathCommand(RenderPath *renderPath);

	/**
	* Loads the command from an xml element
	* @param element The xml element to load
	* @param engine The engine
	*/
	bool Load(TiXmlElement *element, Engine *engine);

	/**
	* @returns Whether this command reads from the viewport
	*/
	bool HasViewportReads() const;

	/**
	* Parse the command type from string
	* @param type The command type name
	* @returns The command type
	*/
	static RenderPathCommandType stringToType(const std::string &type);

	RenderPath *renderPath;
	RenderPathCommandType type;
	bool useFogColor;
	vec3 clearColor;
	ClearBufferType clearBuffers;
	StringHash output;
	StringHash textures[static_cast<int>(TextureUnit::MAX_TEXTURE_UNITS)];
	StringHash pass;
	Shader *vertexShader;
	Shader *fragmentShader;	
	std::map<StringHash, Variant> parameters;
};

class FIRECUBE_API RenderSurfaceDescriptor
{
public:
	int width, height;
	vec2 sizeMultiplier;
	bool hasSizeMultiplier;
	std::string name;
};

/**
* A class representing a render path which is a collection of rendering commands and render surfaces
*/
class FIRECUBE_API RenderPath : public Resource
{
	FIRECUBE_OBJECT(RenderPath)
public:
	RenderPath(Engine *engine);
	
	/**
	* Loads the rendring path from an xml file
	* @param filename The path to the xml file
	*/
	virtual bool Load(const std::string &filename);

	/**
	* @returns All the commands in the render path
	*/
	const std::vector<RenderPathCommand> &GetCommands() const;

	/**
	* Returns a command
	* @param index The index of the command
	*/
	RenderPathCommand &GetCommand(int index);

	/**
	* Returns a render surface
	* @param name The has of the name of the render surface to get
	*/
	RenderSurface *GetRenderTarget(StringHash name);

	/**
	* @returns All the render surface descriptors used by the render path
	*/
	std::vector<RenderSurfaceDescriptor>  &GetRenderSurfaceDescriptors();

	/**
	* Allocates all render surfaces according to the descriptors
	*/
	void AllocateRenderSurfaces();

	/**
	* Appends a render path to this rendr path
	* @param renderPath The render path to append
	*/
	void Append(RenderPath *renderPath);

	/**
	* Clones this render path
	*/
	RenderPath *Clone() const;

private:
	std::map<StringHash, SharedPtr<RenderSurface>> renderTargets;
	std::vector<RenderPathCommand> commands;
	std::vector<RenderSurfaceDescriptor> renderSurfaceDescriptors;
};

}