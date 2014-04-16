#include "RenderPath.h"
#include "tinyxml.h"
#include "Utils/Filesystem.h"
#include "Core/Variant.h"
#include "Core/Engine.h"
#include "Rendering/Renderer.h"

using namespace FireCube;

RenderPathCommand::RenderPathCommand() : type(COMMAND_UNKNOWN), useFogColor(false)
{

}

bool RenderPathCommand::Load(TiXmlElement *element)
{
	type = RenderPathCommand::stringToType(element->Attribute("type"));

	switch (type)
	{	
	case FireCube::COMMAND_CLEAR:
		{
			std::string clearStr = element->Attribute("color");
			if (clearStr == "fogcolor")
			{
				useFogColor = true;
			}
			else
			{
				useFogColor = false;
				Variant v = Variant::FromString(clearStr);
				clearColor = v.GetVec3();
			}
		}
		break;
	case FireCube::COMMAND_SCENEPASS:		
		break;
	case FireCube::COMMAND_LIGHTPASS:
		break;
	default:
		return false;		
	}

	return true;
}

RenderPathCommandType RenderPathCommand::stringToType(const std::string &type)
{
	if (type == "clear")
		return COMMAND_CLEAR;
	else if (type == "scenepass")
		return COMMAND_SCENEPASS;
	else if (type == "lightpass")
		return COMMAND_LIGHTPASS;
	else
		return COMMAND_UNKNOWN;
}

RenderPath::RenderPath(Engine *engine) : Resource(engine)
{

}
bool RenderPath::Load(const std::string &filename)
{
	TiXmlDocument xmlDocument;
	if (!xmlDocument.LoadFile(Filesystem::SearchForFileName(filename)))
		return false;

	TiXmlElement *e = xmlDocument.FirstChildElement("renderpath");
	if (e == nullptr)
		return false;
	this->filename = filename;

	for (TiXmlElement *element = e->FirstChildElement(); element != nullptr; element = element->NextSiblingElement())
	{
		if (element->ValueStr() == "rendersurface")
		{
			std::string name = element->Attribute("name");			
			Variant sizeVariant = Variant::FromString(element->Attribute("size"));
			vec2 size = sizeVariant.GetVec2();
			Texture *texture = new Texture(engine);
			engine->GetRenderer()->UseTexture(texture, 0);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, (int) size.x, (int) size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			renderTargets[StringHash(name)] = texture;
		}
		else if (element->ValueStr() == "command")
		{
			RenderPathCommand command;
			if (command.Load(element))
			{
				commands.push_back(command);
			}
		}
		/*
		else if (element->ValueStr() == "texture")
		{
			std::string textureUnitName = element->Attribute("unit");
			if (textureUnitName.empty())
				continue;
			std::string textureName = element->Attribute("name");
			if (textureName.empty())
				continue;

			TextureUnit textureUnit = ParseTextureUnitName(textureUnitName);
			textures[textureUnit] = engine->GetResourcePool()->GetResource<Texture>(textureName);
		}*/
	}
	return true;	
}

const std::vector<RenderPathCommand> &RenderPath::GetCommands() const
{
	return commands;
}

RenderPathCommand &RenderPath::GetCommand(int index)
{
	return commands[index];
}