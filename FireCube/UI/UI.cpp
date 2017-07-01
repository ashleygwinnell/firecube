#include "UI/UI.h"
#include "UI/UIElement.h"
#include "Rendering/VertexBuffer.h"
#include "Core/Engine.h"
#include "Rendering/Renderer.h"
#include "Rendering/RenderingTypes.h"
#include "Rendering/Shader.h"
#include "Rendering/ShaderTemplate.h"
#include "Core/ResourceCache.h"
#include "Rendering/Texture2D.h"

using namespace FireCube;

UI::UI(Engine *engine) : Object(engine), root(new UIElement(engine, nullptr))
{
	vertexBuffer = new VertexBuffer(engine->GetRenderer());
}

UI::~UI()
{
	delete vertexBuffer;
	delete root;
}

void UI::Render(Renderer *renderer)
{
	std::vector<UIPart> parts;
	std::vector<UIVertex> vertexData;
	GetParts(parts, vertexData, root);
	const unsigned int vertexSize = sizeof(float) * (3 + 2 + 3);
	vertexBuffer->LoadData(vertexData.data(), vertexData.size(), VertexAttributeType::POSITION | VertexAttributeType::TEXCOORD0 | VertexAttributeType::COLOR, BufferType::STREAM);
	renderer->SetBuffer(vertexBuffer);
	renderer->RestoreFrameBuffer();
	renderer->SetDepthTest(DepthTest::ALWAYS);
	renderer->SetBlendMode(BlendMode::ALPHA);
	for (const auto &part : parts)
	{
		if (part.texture->GetFormat() == TextureFormat::R)
		{
			Shader *textVertexShader = engine->GetResourceCache()->GetResource<ShaderTemplate>("Shaders/font.vert")->GenerateShader("");
			Shader *textFragmentShader = engine->GetResourceCache()->GetResource<ShaderTemplate>("Shaders/font.frag")->GenerateShader("");
			Program *textProgram = renderer->SetShaders(textVertexShader, textFragmentShader);
			mat4 ortho;
			ortho.GenerateOrthographic(0, (float)renderer->GetWidth(), (float)renderer->GetHeight(), 0, 0, 1);
			if (textProgram->IsValid())
			{
				textProgram->SetUniform("tex0", 0);
				textProgram->SetUniform("projectionMatrix", ortho);
			}

			renderer->UseTexture(0, part.texture);
			renderer->RenderStream(PrimitiveType::TRIANGLES, part.count, part.offset);
		}
		else if (part.texture->GetFormat() == TextureFormat::RGB || part.texture->GetFormat() == TextureFormat::RGBA)
		{
			std::string fragmentShaderDefines = part.texture->GetFormat() == TextureFormat::RGBA ? "ALPHA" : "";
			Shader *vertexShader = engine->GetResourceCache()->GetResource<ShaderTemplate>("Shaders/font.vert")->GenerateShader("");
			Shader *fragmentShader = engine->GetResourceCache()->GetResource<ShaderTemplate>("Shaders/image.frag")->GenerateShader(fragmentShaderDefines);
			Program *program = renderer->SetShaders(vertexShader, fragmentShader);
			mat4 ortho;
			ortho.GenerateOrthographic(0, (float)renderer->GetWidth(), (float)renderer->GetHeight(), 0, 0, 1);
			if (program->IsValid())
			{
				program->SetUniform("tex0", 0);
				program->SetUniform("projectionMatrix", ortho);
			}

			renderer->UseTexture(0, part.texture);
			renderer->RenderStream(PrimitiveType::TRIANGLES, part.count, part.offset);
		}
	}
}

void UI::GetParts(std::vector<UIPart> &parts, std::vector<UIVertex> &vertexData, UIElement *element)
{
	element->GetParts(parts, vertexData);
	auto &children = element->GetChildren();
	for (auto child : children)
	{
		GetParts(parts, vertexData, child);
	}
}

UIElement *UI::GetRoot()
{
	return root;
}