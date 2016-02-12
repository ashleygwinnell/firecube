#include "RenderPath.h"
#include "tinyxml.h"
#include "Utils/Filesystem.h"
#include "Core/Variant.h"
#include "Core/Engine.h"
#include "Rendering/Renderer.h"
#include "Rendering/Material.h"
#include "Rendering/ShaderTemplate.h"
#include "Core/ResourceCache.h"
#include "Rendering/RenderSurface.h"

using namespace FireCube;

RenderPathCommand::RenderPathCommand(RenderPath *renderPath) : renderPath(renderPath), type(RenderPathCommandType::UNKNOWN), useFogColor(false)
{

}

bool RenderPathCommand::Load(TiXmlElement *element, Engine *engine)
{
	type = RenderPathCommand::stringToType(element->Attribute("type"));

	switch (type)
	{
	case RenderPathCommandType::CLEAR:
	{
		clearBuffers = ClearBufferType::NONE;
		const char *buffers = element->Attribute("buffers");
		if (buffers)
		{
			auto bufferTypes = Split(std::string(buffers), ' ');
			for (const auto &bufferType : bufferTypes)
			{
				if (bufferType == "color")
				{
					clearBuffers |= ClearBufferType::COLOR;
				}
				else if (bufferType == "depth")
				{
					clearBuffers |= ClearBufferType::DEPTH;
				}
			}
		}
		else
		{
			clearBuffers = ClearBufferType::COLOR | ClearBufferType::DEPTH;
		}

		const char *clearColor = element->Attribute("color");
		if (clearColor)
		{
			std::string clearColorStr = clearColor;
			if (clearColorStr == "fogcolor")
			{
				useFogColor = true;
			}
			else
			{
				useFogColor = false;
				Variant v = Variant::FromString(clearColorStr);
				this->clearColor = v.GetVec3();
			}
		}
		break;
	}


	case RenderPathCommandType::BASEPASS:
	{
		const char *passNameStr = element->Attribute("pass");
		if (passNameStr)
		{
			pass = passNameStr;
		}
		break;
	}


	case RenderPathCommandType::LIGHTPASS:
	{
		const char *passNameStr = element->Attribute("pass");
		if (passNameStr)
		{
			pass = passNameStr;
		}
		break;
	}


	case RenderPathCommandType::QUAD:
	{
		ShaderTemplate *vertexShaderTemplate = engine->GetResourceCache()->GetResource<ShaderTemplate>(element->Attribute("vs"));
		ShaderTemplate *fragmentShaderTemplate = engine->GetResourceCache()->GetResource<ShaderTemplate>(element->Attribute("fs"));
		std::string shaderDefines = element->Attribute("defines");
		if (vertexShaderTemplate == nullptr || fragmentShaderTemplate == nullptr)
			return false;

		vertexShader = vertexShaderTemplate->GenerateShader(shaderDefines);
		fragmentShader = fragmentShaderTemplate->GenerateShader(shaderDefines);
		break;
	}

	default:
		return false;
	}

	if (element->Attribute("output"))
	{
		output = StringHash(element->Attribute("output"));
		// TODO: Implement MRT by allowing multiple outputs and specify index for each output
	}

	for (TiXmlElement *e = element->FirstChildElement(); e != nullptr; e = e->NextSiblingElement())
	{
		if (e->ValueStr() == "texture")
		{
			std::string textureUnitName = e->Attribute("unit");
			if (textureUnitName.empty())
				continue;
			std::string textureName = e->Attribute("name");
			if (textureName.empty())
				continue;

			TextureUnit textureUnit = Material::ParseTextureUnitName(textureUnitName);
			textures[static_cast<int>(textureUnit)] = StringHash(textureName);			
		}
	}


	return true;
}

bool RenderPathCommand::HasViewportReads() const
{
	for (int i = 0; i < static_cast<int>(TextureUnit::MAX_TEXTURE_UNITS); ++i)
	{
		if (textures[i] == VIEWPORT_TARGET)
		{
			return true;
		}
	}

	return false;
}

RenderPathCommandType RenderPathCommand::stringToType(const std::string &type)
{
	if (type == "clear")
		return RenderPathCommandType::CLEAR;
	else if (type == "basepass")
		return RenderPathCommandType::BASEPASS;
	else if (type == "lightpass")
		return RenderPathCommandType::LIGHTPASS;
	else if (type == "quad")
		return RenderPathCommandType::QUAD;
	else
		return RenderPathCommandType::UNKNOWN;
}

RenderPath::RenderPath(Engine *engine) : Resource(engine)
{

}

bool RenderPath::Load(const std::string &filename)
{
	std::string resolvedFileName = Filesystem::FindResourceByName(filename);
	if (resolvedFileName.empty())
		return false;

	TiXmlDocument xmlDocument;
	if (!xmlDocument.LoadFile(resolvedFileName))
		return false;

	TiXmlElement *e = xmlDocument.FirstChildElement("renderpath");
	if (e == nullptr)
		return false;
	this->filename = filename;

	for (TiXmlElement *element = e->FirstChildElement(); element != nullptr; element = element->NextSiblingElement())
	{
		if (element->ValueStr() == "rendersurface")
		{
			RenderSurfaceDescriptor desc;
			desc.name = element->Attribute("name");			
			if (element->Attribute("size"))
			{
				Variant sizeVariant = Variant::FromString(element->Attribute("size"));
				vec2 size = sizeVariant.GetVec2();
				desc.width = (int) size.x;
				desc.height = (int) size.y;
				desc.hasSizeMultiplier = false;
			}
			else if (element->Attribute("size_multiplier"))
			{
				Variant sizeVariant = Variant::FromString(element->Attribute("size_multiplier"));
				 desc.sizeMultiplier = sizeVariant.GetVec2();
				 desc.hasSizeMultiplier = true;
			}
			
			renderSurfaceDescriptors.push_back(desc);			
		}
		else if (element->ValueStr() == "command")
		{
			RenderPathCommand command(this);
			if (command.Load(element, engine))
			{
				commands.push_back(command);
			}
		}		
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

RenderSurface *RenderPath::GetRenderTarget(StringHash name)
{
	auto i = renderTargets.find(name);
	if (i != renderTargets.end())
		return i->second;

	return nullptr;
}

void RenderPath::AllocateRenderSurfaces()
{
	bool hasViewportRead = false;
	for (auto &command : commands)
	{
		if (command.HasViewportReads())
		{
			hasViewportRead = true;
			break;
		}		
	}

	if (hasViewportRead)
	{
		SharedPtr<RenderSurface> renderSurface = engine->GetRenderer()->GetRenderSurface(engine->GetRenderer()->GetWidth(), engine->GetRenderer()->GetHeight(), RenderSurfaceType::COLOR);
		renderTargets[StringHash("viewport")] = renderSurface;
	}

	for (const auto &desc : renderSurfaceDescriptors)
	{
		int width, height;
		if (desc.hasSizeMultiplier)
		{
			width = (int) (desc.sizeMultiplier.x * (float) engine->GetRenderer()->GetWidth());
			height = (int)(desc.sizeMultiplier.y * (float)engine->GetRenderer()->GetHeight());
		}
		else
		{
			width = desc.width;
			height = desc.height;
		}
		SharedPtr<RenderSurface> renderSurface = engine->GetRenderer()->GetRenderSurface(width, height, RenderSurfaceType::COLOR);
		renderTargets[StringHash(desc.name)] = renderSurface;
	}
	
}

void RenderPath::Append(RenderPath *renderPath)
{
	renderSurfaceDescriptors.insert(renderSurfaceDescriptors.end(), renderPath->renderSurfaceDescriptors.begin(), renderPath->renderSurfaceDescriptors.end());
	
	unsigned int newCommandsIndex = commands.size();
	commands.insert(commands.end(), renderPath->commands.begin(), renderPath->commands.end());
	
	for (auto i = commands.begin() + newCommandsIndex; i != commands.end(); ++i)
	{
		i->renderPath = this;
	}
}

RenderPath *RenderPath::Clone() const
{
	RenderPath *clone = new RenderPath(engine);
	clone->renderSurfaceDescriptors = renderSurfaceDescriptors;
	clone->commands = commands;
	return clone;
}
