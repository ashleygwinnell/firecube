#include "Utils/Logger.h"
#include "Rendering/Texture.h"
#include "Geometry/Geometry.h"
#include "Geometry/Material.h"
#include "ModelLoaders/ColladaLoader.h"
#include "Math/MathUtils.h"
#include "Rendering/IndexBuffer.h"
#include "Core/Engine.h"
#include "Core/ResourceCache.h"
#include "Rendering/RenderingTypes.h"
#include "Rendering/Technique.h"
#include <algorithm>

using namespace FireCube;

// The ColladaLoader class loads a Collada file by reading all the libraries and storing the scene graph
// and geometries in an internal representation. Then GenerateGeometries is used to convert this internal
// representation to the engine's classes. A Collada geometry consists of vertex attributes and a list
// of faces with materials. Since the engine's Geometry class has only one matrial associated with it
// every Collada geometry is converted to several engine geometries (each with a difference material).

ColladaLoader::ColladaLoader(Engine *engine) : ModelLoader(engine)
{

}

ColladaLoader::~ColladaLoader()
{
	std::map<std::string, Node *>::iterator i = nodeLibrary.begin();
	for (; i != nodeLibrary.end(); i++)
		DeleteNodes(i->second);
}

bool ColladaLoader::Load(const std::string &filename, ModelLoadingOptions options)
{
	if (xmlDocument.LoadFile(filename) == false)
		return false;
	// Get the root element
	TiXmlElement *e = GetChildElement(&xmlDocument, "COLLADA");
	if (e == nullptr)
		return false;

	// Only Collada version 1.4 is supported
	std::string version = e->Attribute("version");
	if (version.substr(0, 3) == "1.4")
		ReadLibraries(e);

	this->options = options;	
	return true;
}

void ColladaLoader::GenerateScene(Renderer *renderer, FireCube::Node *root)
{
	// TODO: Implement
}
Node *ColladaLoader::GetGeneratedScene()
{
	// TODO: Implement
	return nullptr;
}

const std::vector<Material *> &ColladaLoader::GetGeneratedMaterials()
{
	return generatedMaterials;
}

void ColladaLoader::ReadLibraries(TiXmlNode *parent)
{
	// Parse the libraries in this Collada file
	TiXmlNode *node;
	TiXmlElement *element;
	for (node = parent->FirstChild(); node != nullptr; node = node->NextSibling())
	{
		if (node->Type() == TiXmlNode::ELEMENT)
		{
			element = node->ToElement();
			if (element->ValueStr() == "asset")
				ReadAsset(element);
			else if (element->ValueStr() == "library_images")
				ReadImageLibrary(element);
			else if (element->ValueStr() == "library_materials")
				ReadMaterialLibrary(element);
			else if (element->ValueStr() == "library_effects")
				ReadEffectLibrary(element);
			else if (element->ValueStr() == "library_geometries")
				ReadGeometryLibrary(element);
			else if (element->ValueStr() == "library_visual_scenes")
				ReadSceneLibrary(element);
			else if (element->ValueStr() == "library_nodes")
				ReadSceneNode(element, nullptr);
			else if (element->ValueStr() == "scene")
				ReadScene(element);
		}
	}
}

void ColladaLoader::ReadAsset(TiXmlNode *parent)
{
	TiXmlNode *node;
	TiXmlElement *element;
	for (node = parent->FirstChild(); node != nullptr; node = node->NextSibling())
	{
		if (node->Type() == TiXmlNode::ELEMENT)
		{
			element = node->ToElement();
			if (element->ValueStr() == "unit")
			{
				if (element->QueryFloatAttribute("meter", &unit) != TIXML_SUCCESS)
					unit = 1.0f;
			}
			else if (element->ValueStr() == "up_axis")
			{
				std::string v = element->FirstChild()->ToText()->ValueStr();
				if (v == "X_UP")
					upDirection = X_UP;
				else if (v == "Y_UP")
					upDirection = Y_UP;
				else if (v == "Z_UP")
					upDirection = Z_UP;
			}
		}
	}
}

void ColladaLoader::ReadImageLibrary(TiXmlNode *parent)
{
	TiXmlNode *node;
	TiXmlElement *element;
	for (node = parent->FirstChild(); node != nullptr; node = node->NextSibling())
	{
		if (node->Type() == TiXmlNode::ELEMENT)
		{
			element = node->ToElement();
			// Parse child elements of type "image"
			if (element->ValueStr() == "image")
			{
				// Read it's id
				std::string id = element->Attribute("id");
				imageLibrary[id] = Image();
				// Parse this image element
				ReadImage(element, imageLibrary[id]);
			}
		}
	}
}

void ColladaLoader::ReadImage(TiXmlNode *parent, Image &img)
{
	TiXmlNode *node;
	TiXmlElement *element;
	for (node = parent->FirstChild(); node != nullptr; node = node->NextSibling())
	{
		if (node->Type() == TiXmlNode::ELEMENT)
		{
			element = node->ToElement();
			// Read the image's file name
			if (element->ValueStr() == "init_from")
			{
				img.initFrom = element->GetText();
			}
		}
	}
}

void ColladaLoader::ReadMaterialLibrary(TiXmlNode *parent)
{
	TiXmlNode *node;
	TiXmlElement *element;
	for (node = parent->FirstChild(); node != nullptr; node = node->NextSibling())
	{
		if (node->Type() == TiXmlNode::ELEMENT)
		{
			// Parse child elements of type "material"
			element = node->ToElement();
			if (element->ValueStr() == "material")
			{
				// Read it's id
				std::string id = element->Attribute("id");
				materialLibrary[id] = Material();
				// Read the child elements describing this material
				ReadMaterial(element, materialLibrary[id]);
			}
		}
	}
}

void ColladaLoader::ReadMaterial(TiXmlNode *parent, Material &mat)
{
	TiXmlNode *node;
	TiXmlElement *element;
	for (node = parent->FirstChild(); node != nullptr; node = node->NextSibling())
	{
		if (node->Type() == TiXmlNode::ELEMENT)
		{
			// Read the effect associated with this material
			element = node->ToElement();
			if (element->ValueStr() == "instance_effect")
			{
				std::string url = element->Attribute("url");
				mat.effect = url.substr(1);
			}
		}
	}
}

void ColladaLoader::ReadEffectLibrary(TiXmlNode *parent)
{
	TiXmlNode *node;
	TiXmlElement *element;
	for (node = parent->FirstChild(); node != nullptr; node = node->NextSibling())
	{
		if (node->Type() == TiXmlNode::ELEMENT)
		{
			// Parse child elements of type "effect"
			element = node->ToElement();
			if (element->ValueStr() == "effect")
			{
				// Read it's id
				std::string id = element->Attribute("id");
				effectLibrary[id] = Effect();
				// Parse this effect element
				ReadEffect(element, effectLibrary[id]);
			}
		}
	}
}

void ColladaLoader::ReadEffect(TiXmlNode *parent, Effect &effect)
{
	// For now, only the profile common is supported in effect elements
	TiXmlNode *node;
	TiXmlElement *element;
	for (node = parent->FirstChild(); node != nullptr; node = node->NextSibling())
	{
		if (node->Type() == TiXmlNode::ELEMENT)
		{
			element = node->ToElement();			
			if (element->ValueStr() == "profile_COMMON")
				ReadEffectProfileCommon(element, effect);
		}
	}
}

void ColladaLoader::ReadEffectProfileCommon(TiXmlNode *parent, Effect &effect)
{
	TiXmlNode *node;
	TiXmlElement *element;
	for (node = parent->FirstChild(); node != nullptr; node = node->NextSibling())
	{
		if (node->Type() == TiXmlNode::ELEMENT)
		{
			element = node->ToElement();
			if (element->ValueStr() == "technique")
				ReadEffectProfileCommon(element, effect); // Just descend into this element
			else if (element->ValueStr() == "newparam")
			{
				// Read a param of this effect and associate it with it's sid
				std::string sid = element->Attribute("sid");
				effect.effectParams[sid] = EffectParam();
				ReadEffectParam(element, effect.effectParams[sid]);
			}
			// Read the shading type of this effect and descend into this element to get the shading parameters (ambient, diffuse, etc...)
			else if (element->ValueStr() == "phong")
			{				
				effect.shadingType = SHADING_PHONG;
				ReadEffectProfileCommon(element, effect);
			}
			else if (element->ValueStr() == "lambert")
			{
				effect.shadingType = SHADING_LAMBERT;
				ReadEffectProfileCommon(element, effect);
			}
			else if (element->ValueStr() == "blinn")
			{
				effect.shadingType = SHADING_BLINN;
				ReadEffectProfileCommon(element, effect);
			}
			else if (element->ValueStr() == "constant")
			{
				effect.shadingType = SHADING_CONSTANT;
				ReadEffectProfileCommon(element, effect);
			}
			else if (element->ValueStr() == "ambient")
				ReadEffectColor(element, effect.ambientColor, effect.ambientSampler); // Read the ambient color (and/or texture)
			else if (element->ValueStr() == "diffuse")
				ReadEffectColor(element, effect.diffuseColor, effect.diffuseSampler); // Read the diffuse color (and/or texture)
			else if (element->ValueStr() == "specular")
				ReadEffectColor(element, effect.specularColor, effect.specularSampler); // Read the specular color (and/or texture)
			else if (element->ValueStr() == "shininess") // Read the shininess value of this effect
				ReadEffectFloat(element, effect.shininess);
		}
	}
}

void ColladaLoader::ReadEffectParam(TiXmlNode *parent, EffectParam &effectParam)
{
	TiXmlNode *node;
	TiXmlElement *element;
	for (node = parent->FirstChild(); node != nullptr; node = node->NextSibling())
	{
		if (node->Type() == TiXmlNode::ELEMENT)
		{			
			element = node->ToElement();
			if (element->ValueStr() == "surface") // Read a "surface" param
			{
				TiXmlElement *e = element->FirstChildElement();
				if (e->ValueStr() == "init_from")
				{
					effectParam.type = PARAM_SURFACE;
					effectParam.reference = e->GetText();
				}
			}
			else if (element->ValueStr() == "sampler2D") // Read a "sampler2D" param
			{
				TiXmlElement *e = element->FirstChildElement();
				if (e->ValueStr() == "source")
				{
					effectParam.type = PARAM_SAMPLER_2D;
					effectParam.reference = e->GetText();
				}
			}
		}
	}
}

void ColladaLoader::ReadEffectColor(TiXmlNode *parent, vec4 &color, Sampler &sampler)
{
	// Reads a color of some lighting term (ambient, diffuse, ...)
	// The color can be an actual color and/or a texture
	TiXmlNode *node;
	TiXmlElement *element;
	for (node = parent->FirstChild(); node != nullptr; node = node->NextSibling())
	{
		if (node->Type() == TiXmlNode::ELEMENT)
		{
			element = node->ToElement();
			// Parse child elements of type "color" which contains RGBA values
			if (element->ValueStr() == "color")
			{
				std::string data = element->GetText();
				std::istringstream iss(data);
				iss >> color.x >> color.y >> color.z >> color.w;
			}
			// Parse child elements of type "texture"
			else if (element->ValueStr() == "texture")
			{
				// Read the sampler's name and the texture coordinate set		
				sampler.name = element->Attribute("texture");
				sampler.uvCoords = element->Attribute("texcoord");
			}
		}
	}
}

void ColladaLoader::ReadEffectFloat(TiXmlNode *parent, float &value)
{
	TiXmlNode *node;
	TiXmlElement *element;
	for (node = parent->FirstChild(); node != nullptr; node = node->NextSibling())
	{
		if (node->Type() == TiXmlNode::ELEMENT)
		{
			element = node->ToElement();
			if (element->ValueStr() == "float")
			{
				std::string data = element->GetText();
				std::istringstream iss(data);
				iss >> value;
			}
		}
	}
}

void ColladaLoader::ReadGeometryLibrary(TiXmlNode *parent)
{
	TiXmlNode *node;
	TiXmlElement *element;
	for (node = parent->FirstChild(); node != nullptr; node = node->NextSibling())
	{
		if (node->Type() == TiXmlNode::ELEMENT)
		{
			element = node->ToElement();
			// Read all geometries
			if (element->ValueStr() == "geometry")
			{
				std::string id = element->Attribute("id");
				std::string name = element->Attribute("name");
				geometryLibrary[id] = Geometry();
				geometryLibrary[id].name = name;
				ReadGeometry(element, geometryLibrary[id]);
			}
		}
	}
}

void ColladaLoader::ReadGeometry(TiXmlNode *parent, Geometry &geometry)
{
	TiXmlNode *node;
	TiXmlElement *element;

	tempMap.clear();
	for (node = parent->FirstChild(); node != nullptr; node = node->NextSibling())
	{
		if (node->Type() == TiXmlNode::ELEMENT)
		{
			element = node->ToElement();
			// Read the mesh of this geometry
			if (element->ValueStr() == "mesh")
				ReadMesh(element, geometry.mesh);
		}
	}
}

void ColladaLoader::ReadMesh(TiXmlNode *parent, Mesh &mesh)
{
	TiXmlNode *node;
	TiXmlElement *element;

	for (node = parent->FirstChild(); node != nullptr; node = node->NextSibling())
	{
		if (node->Type() == TiXmlNode::ELEMENT)
		{
			element = node->ToElement();
			// Read a source element which specifies a piece of data
			if (element->ValueStr() == "source")
			{
				std::string id = element->Attribute("id");
				sources[id] = Source();
				ReadSource(element, sources[id]);
			}
			// Read the vertices of this mesh
			else if (element->ValueStr() == "vertices")
			{
				ReadVertexData(element, mesh);
			}
			// Read a triangles list
			else if (element->ValueStr() == "triangles")
			{
				ReadTriangles(element, mesh);
			}
			// Read a polygon list
			else if (element->ValueStr() == "polylist")
			{
				ReadPolylist(element, mesh);
			}
		}
	}
}

void ColladaLoader::ReadSource(TiXmlNode *parent, Source &source)
{
	TiXmlNode *node;
	TiXmlElement *element;
	for (node = parent->FirstChild(); node != nullptr; node = node->NextSibling())
	{
		if (node->Type() == TiXmlNode::ELEMENT)
		{
			element = node->ToElement();
			// Read a float array
			if (element->ValueStr() == "float_array")
			{
				ReadDataArray(element, source.dataArray);
			}
			else if (element->ValueStr() == "technique_common")
				ReadSource(element, source);
			else if (element->ValueStr() == "accessor")
			{
				// Read the accessor of this source which specifies an access pattern to this source
				ReadAccessor(element, source.accessor);
			}
		}
	}
}

void ColladaLoader::ReadDataArray(TiXmlNode *parent, DataArray &dataArray)
{
	TiXmlElement *element = parent->ToElement();
	int count;
	if (element->QueryIntAttribute("count", &count) != TIXML_SUCCESS)
		return;

	std::istringstream iss(element->GetText());
	dataArray.floatData.reserve(count);
	for (int i = 0; i < count; i++)
	{
		float v;
		iss >> v;
		dataArray.floatData.push_back(v);
	}
}

void ColladaLoader::ReadAccessor(TiXmlNode *parent, Accessor &accesor)
{
	TiXmlNode *node;
	TiXmlElement *element = parent->ToElement();	
	// count specifies the number of "data units"
	// stride specifies the number of elements regarded as one "data unit". stride should be >= number of params in the accessor
	// offset specifies an offset into the data array
	if (element->QueryIntAttribute("count", &accesor.count) != TIXML_SUCCESS)
		return;
	
	if (element->QueryIntAttribute("stride", &accesor.stride) != TIXML_SUCCESS)
		accesor.stride = 1;
	if (element->QueryIntAttribute("offset", &accesor.offset) != TIXML_SUCCESS)
		accesor.offset = 0;
	
	accesor.componentsOffset[0] = accesor.componentsOffset[1] = accesor.componentsOffset[2] = accesor.componentsOffset[3] = -1;
	int currentOffset = 0;
	int currentComponent = 0;
	for (node = parent->FirstChild(); node != nullptr; node = node->NextSibling())
	{
		if (node->Type() == TiXmlNode::ELEMENT)
		{
			element = node->ToElement();
			if (element->ValueStr() == "param")
			{								
				const char *name = element->Attribute("name");				 
				if (name == nullptr || *name == '\0') // If param has no "name" attribute or is empty, just skip this offset
				{
					currentOffset++;
				}
				else
				{
					accesor.componentsOffset[currentComponent++] = currentOffset++;
				}													
			}
		}
	}
}

void ColladaLoader::ReadVertexData(TiXmlNode *parent, Mesh &mesh)
{
	TiXmlNode *node;
	TiXmlElement *element = parent->ToElement();
	mesh.vertexId = element->Attribute("id");
	// This element should have an "input" element with a "POSITION" semantic
	for (node = parent->FirstChild(); node != nullptr; node = node->NextSibling())
	{
		if (node->Type() == TiXmlNode::ELEMENT)
		{
			element = node->ToElement();
			if (element->ValueStr() == "input")
			{
				ReadInputChannel(element, mesh.inputChannels);
			}
		}
	}
}

void ColladaLoader::ReadInputChannel(TiXmlNode *parent, std::vector<InputChannel> &inputChannels)
{
	// An input channel maps streams of data to their logical meaning (normals, uv, ...)
	TiXmlNode *node;
	TiXmlElement *element = parent->ToElement();
	inputChannels.push_back(InputChannel());
	InputChannel &ic = inputChannels.back();

	std::string semantic = element->Attribute("semantic");
	ic.type = SemanticToInputType(semantic);
	std::string source = element->Attribute("source");
	ic.sourceName = source.substr(1);
	if (element->QueryIntAttribute("offset", &ic.offset) != TIXML_SUCCESS)
		ic.offset = 0;

	if ((ic.type == INPUT_TEXCOORD) || (ic.type == INPUT_COLOR))
	{
		if (element->QueryIntAttribute("set", &ic.index) != TIXML_SUCCESS)
			ic.index = 0;
	}
	for (node = parent->FirstChild(); node != nullptr; node = node->NextSibling())
	{
		if (node->Type() == TiXmlNode::ELEMENT)
		{
			element = node->ToElement();
		}
	}
}

void ColladaLoader::ReadTriangles(TiXmlNode *parent, Mesh &mesh)
{
	// Each triangle element is a list of triangles sharing a single material
	TiXmlNode *node;
	TiXmlElement *element = parent->ToElement();

	std::vector<InputChannel> primInputChannels;
	int count;
	mesh.subMeshes.push_back(SubMesh());
	SubMesh &subMesh = mesh.subMeshes[mesh.subMeshes.size() - 1];
	element->QueryIntAttribute("count", &count);
	subMesh.material = element->Attribute("material");
	subMesh.numPrimtives = count;
	subMesh.primitiveType = PRIMITIVE_TRIANGLES;

	// Read all the input channels for this sub mesh and then it's triangles indices
	for (node = parent->FirstChild(); node != nullptr; node = node->NextSibling())
	{
		if (node->Type() == TiXmlNode::ELEMENT)
		{
			element = node->ToElement();
			if (element->ValueStr() == "input")
				ReadInputChannel(element, primInputChannels);
			else if (element->ValueStr() == "p")
				ReadPrimitives(element, mesh, subMesh, primInputChannels, count);
		}
	}
}

void ColladaLoader::ReadPolylist(TiXmlNode *parent, Mesh &mesh)
{
	// Each poly list element is a list of polygons sharing a single material
	TiXmlNode *node;
	TiXmlElement *element = parent->ToElement();

	std::vector<InputChannel> primInputChannels;
	int count;
	mesh.subMeshes.push_back(SubMesh());
	SubMesh &subMesh = mesh.subMeshes[mesh.subMeshes.size() - 1];
	element->QueryIntAttribute("count", &count);
	subMesh.material = element->Attribute("material");
	subMesh.numPrimtives = count;
	subMesh.primitiveType = PRIMITIVE_POLYLIST;

	// Read all the input channels for this sub mesh, the vertices count for each polygon and it's indices
	for (node = parent->FirstChild(); node != nullptr; node = node->NextSibling())
	{
		if (node->Type() == TiXmlNode::ELEMENT)
		{
			element = node->ToElement();
			if (element->ValueStr() == "input")
				ReadInputChannel(element, primInputChannels);
			else if (element->ValueStr() == "vcount")
				ReadVCount(element, subMesh);
			else if (element->ValueStr() == "p")
				ReadPrimitives(element, mesh, subMesh, primInputChannels, count);
		}
	}
}

void ColladaLoader::ReadVCount(TiXmlNode *parent, SubMesh &subMesh)
{
	TiXmlElement *element = parent->ToElement();
	subMesh.vcount.reserve(subMesh.numPrimtives);

	std::istringstream iss(element->GetText());
	while (!iss.eof())
	{
		int idx;
		iss >> idx;
		subMesh.vcount.push_back(idx);
	}
}

void ColladaLoader::ReadPrimitives(TiXmlNode *parent, Mesh &mesh, SubMesh &subMesh, std::vector<InputChannel> primInputChannels, int count)
{
	TiXmlElement *element = parent->ToElement();
	std::vector<int> indices;
	int offset = 1;
	int vertexOffset = 0;
	// Determine the number of indices which belong to a single vertex
	for (unsigned int i = 0; i < primInputChannels.size(); i++)
	{
		offset = std::max(offset, primInputChannels[i].offset + 1);
		if (primInputChannels[i].type == INPUT_VERTEX)
			vertexOffset = primInputChannels[i].offset;
	}
	// Allocate space for the indices
	int expectedIndicesCount = 0;
	if (subMesh.primitiveType == PRIMITIVE_TRIANGLES)
		expectedIndicesCount = count * 3;
	else if (subMesh.primitiveType == PRIMITIVE_POLYLIST)
	{
		for (unsigned int i = 0; i < subMesh.vcount.size(); i++)
			expectedIndicesCount += subMesh.vcount[i];
	}


	indices.reserve(expectedIndicesCount * offset);

	// Read all the indices
	std::istringstream iss(element->GetText());
	while (!iss.eof())
	{
		int idx;
		iss >> idx;
		indices.push_back(idx);
	}
	// Resolve all the source for each input channel
	for (unsigned int i = 0; i < mesh.inputChannels.size(); i++)
	{
		if (mesh.inputChannels[i].source == nullptr)
		{
			mesh.inputChannels[i].source = &ResolveLibraryReference(sources, mesh.inputChannels[i].sourceName);
		}
	}
	for (unsigned int i = 0; i < primInputChannels.size(); i++)
	{
		if (primInputChannels[i].source == nullptr)
		{
			if (primInputChannels[i].type != INPUT_VERTEX)
				primInputChannels[i].source = &ResolveLibraryReference(sources, primInputChannels[i].sourceName);
		}
	}
	std::vector<int>::iterator idx = indices.begin();
	// Iterate over all primitives
	for (int i = 0; i < count; i++)
	{
		int pointCount = 0;
		if (subMesh.primitiveType == PRIMITIVE_TRIANGLES)
			pointCount = 3;
		else if (subMesh.primitiveType == PRIMITIVE_POLYLIST)
			pointCount = subMesh.vcount[i];
		if (pointCount != 3)
		{
			LOGERROR( "Error in Collada loader, polygons with more than 3 vertices are not implemented yet");
			continue;
		}
		// Iterate over each vertex in the primitive
		for (int j = 0; j < pointCount; j++)
		{
			// Create the indices array belonging to this vertex
			int ind[15];
			for (int o = 0; o < offset; o++)
			{
				ind[o] = *idx;
				idx++;
			}
			
			// Hash the vertex
			unsigned int h = MathUtils::MurmurHash2(ind, offset * 4, 0x1234);
			std::map<unsigned int, unsigned int>::iterator iter = tempMap.find(h);
			if (iter == tempMap.end())
			{
				for (unsigned int k = 0; k < mesh.inputChannels.size(); k++)
				{
					// Read all the vertex data from the sources and store them in their appropriate arrays in the mesh
					GetDataFromChannel(mesh.inputChannels[k], ind[vertexOffset], mesh);
				}
				for (unsigned int k = 0; k < primInputChannels.size(); k++)
				{
					// Read all the vertex data from the sources and store them in their appropriate arrays in the mesh
					GetDataFromChannel(primInputChannels[k], ind[primInputChannels[k].offset], mesh);
				}
				tempMap[h] = mesh.vertices.size() - 1;
				subMesh.indices.push_back(mesh.vertices.size() - 1);
			}
			else
				subMesh.indices.push_back(iter->second);
		}
	}

}

void ColladaLoader::GetDataFromChannel(InputChannel &ic, int index, Mesh &mesh)
{
	// This functions is responsible to read data from a source and apply the input semantic on it, that is
	// storing it in the appropriate array

	// TODO: handle input of type "vertex" correctly (they point to the vertices element)
	if (ic.type == INPUT_VERTEX)
		return;

	float *data = &ic.source->dataArray.floatData[0] + ic.source->accessor.offset + index * ic.source->accessor.stride;
	float values[4];
	for (int i = 0; i < 4; i++)
	{
		if (ic.source->accessor.componentsOffset[i] != -1)
			values[i] = data[ic.source->accessor.componentsOffset[i]];
	}

	if (ic.type == INPUT_POSITION)
		mesh.vertices.push_back(vec3(values[0], values[1], values[2]));
	else if (ic.type == INPUT_NORMAL)
		mesh.normals.push_back(vec3(values[0], values[1], values[2]));
	else if (ic.type == INPUT_TEXCOORD)
	{
		if (ic.index < 4)
			mesh.texcoords[ic.index].push_back(vec2(values[0], 1.0f - values[1]));
	}

}

void ColladaLoader::ReadSceneLibrary(TiXmlNode *parent)
{
	// Read the scene graph
	TiXmlNode *node;
	TiXmlElement *element;
	for (node = parent->FirstChild(); node != nullptr; node = node->NextSibling())
	{
		if (node->Type() == TiXmlNode::ELEMENT)
		{
			element = node->ToElement();
			if (element->ValueStr() == "visual_scene")
			{
				Node *n = new Node();
				n->parent = nullptr;
				n->id = element->Attribute("id");
				n->name = element->Attribute("name");
				nodeLibrary[n->id] = n;
				ReadSceneNode(element, n);
			}
		}
	}
}

void ColladaLoader::ReadSceneNode(TiXmlNode *parent, Node *node)
{	
	TiXmlNode *tnode;
	TiXmlElement *element;
	for (tnode = parent->FirstChild(); tnode != nullptr; tnode = tnode->NextSibling())
	{
		if (tnode->Type() == TiXmlNode::ELEMENT)
		{
			element = tnode->ToElement();
			if (element->ValueStr() == "node")
			{
				// Create a new node
				Node *n = new Node();
				n->id = element->Attribute("id");
				n->name = element->Attribute("name");
				n->parent = nullptr;
				if (node)
				{
					node->children.push_back(n);
					n->parent = node;
				}
				else
					nodeLibrary[n->id] = n;

				ReadSceneNode(element, n);
				continue;
			}

			if (element->ValueStr() == "instance_node")
			{				
				node->nodeInstances.push_back(NodeInstance());
				std::string url = element->Attribute("url"); // Read the id of the node this node instance is referring to
				node->nodeInstances.back().url = url.substr(1);
			}
			else if (element->ValueStr() == "instance_geometry")
			{
				std::string url = element->Attribute("url"); // Read the id of the geometry this geometry instance is referring to
				GeometryInstance gi;
				gi.url = url.substr(1);
				ReadNodeGeometryInstance(element, gi);
				node->geometryInstances.push_back(gi);
			}
			// Read the transformations of this node
			else if (element->ValueStr() == "lookat")
				ReadTransformation(element, node);
			else if (element->ValueStr() == "rotate")
				ReadTransformation(element, node);
			else if (element->ValueStr() == "translate")
				ReadTransformation(element, node);
			else if (element->ValueStr() == "scale")
				ReadTransformation(element, node);
			else if (element->ValueStr() == "matrix")
				ReadTransformation(element, node);
		}
	}
}

void ColladaLoader::ReadNodeGeometryInstance(TiXmlNode *parent, GeometryInstance &gi)
{
	TiXmlNode *tnode;
	TiXmlElement *element;


	for (tnode = parent->FirstChild(); tnode != nullptr; tnode = tnode->NextSibling())
	{
		if (tnode->Type() == TiXmlNode::ELEMENT)
		{
			element = tnode->ToElement();
			if (element->ValueStr() == "bind_material")
				ReadNodeGeometryInstance(element, gi);
			else if (element->ValueStr() == "technique_common")
				ReadNodeGeometryInstance(element, gi);
			else if (element->ValueStr() == "instance_material")
			{
				std::string symbol = element->Attribute("symbol"); // The symbolic name of the material as referenced in the geometry element
				std::string target = element->Attribute("target"); // The actual name of the material (as it appears in the material library)
				gi.materialInstance[symbol] = MaterialInstance();
				MaterialInstance &materialInstance = gi.materialInstance[symbol];
				materialInstance.materialName = target.substr(1);

				ReadMaterialVertexInputBinding(element, materialInstance);
			}
		}
	}
}

void ColladaLoader::ReadMaterialVertexInputBinding(TiXmlNode *parent, MaterialInstance &materialInstance)
{
	TiXmlNode *tnode;
	TiXmlElement *element;

	for (tnode = parent->FirstChild(); tnode != nullptr; tnode = tnode->NextSibling())
	{
		if (tnode->Type() == TiXmlNode::ELEMENT)
		{
			element = tnode->ToElement();
			if (element->ValueStr() == "bind_vertex_input")
			{
				// This element connects between an effect vertex input and a geometry vertex input
				// For example the effect might have input for normal texture coordinates which it calls "TEXCOORD_NORMAL"
				// and the geometry might define it's normal texture coordinates in the TEXCOORD semantic with "set" equals to 2
				InputSemanticEntry is;
				std::string s = element->Attribute("semantic"); // Read the effect parameter to connect
				std::string n = element->Attribute("input_semantic"); // Read the vertex input semantic
				is.type = SemanticToInputType(n);
				if (element->QueryIntAttribute("input_set", &is.set) != TIXML_SUCCESS) // Read the vertex input semantic set
					is.set = -1;
				materialInstance.inputMap[s] = is;
			}
		}
	}
}

void ColladaLoader::ReadScene(TiXmlNode *parent)
{
	TiXmlNode *tnode;
	TiXmlElement *element;


	for (tnode = parent->FirstChild(); tnode != nullptr; tnode = tnode->NextSibling())
	{
		if (tnode->Type() == TiXmlNode::ELEMENT)
		{
			element = tnode->ToElement();
			// Read the root node of the scene graph
			if (element->ValueStr() == "instance_visual_scene")
			{
				std::string url = element->Attribute("url");
				url = url.substr(1);
				std::map<std::string, Node*>::iterator i = nodeLibrary.find(url);
				if (i != nodeLibrary.end())
					root = i->second;
			}
		}
	}
}

void ColladaLoader::ReadTransformation(TiXmlNode *parent, Node *node)
{
	// Reads a single transformation
	TiXmlElement *element = parent->ToElement();
	Transform t;
	if (element->ValueStr() == "lookat")
		t.type = TRANSFORM_LOOKAT;
	else if (element->ValueStr() == "rotate")
		t.type = TRANSFORM_ROTATE;
	else if (element->ValueStr() == "translate")
		t.type = TRANSFORM_TRANSLATE;
	else if (element->ValueStr() == "scale")
		t.type = TRANSFORM_SCALE;
	else if (element->ValueStr() == "matrix")
		t.type = TRANSFORM_MATRIX;
	std::istringstream iss(parent->ToElement()->GetText());
	if (t.type == TRANSFORM_LOOKAT)
	{
		for (int i = 0; i < 9; i++)
			iss >> t.v[i];
	}
	else if (t.type == TRANSFORM_ROTATE)
	{
		for (int i = 0; i < 4; i++)
			iss >> t.v[i];
	}
	else if (t.type == TRANSFORM_TRANSLATE)
	{
		for (int i = 0; i < 3; i++)
			iss >> t.v[i];
	}
	else if (t.type == TRANSFORM_SCALE)
	{
		for (int i = 0; i < 3; i++)
			iss >> t.v[i];
	}
	else if (t.type == TRANSFORM_MATRIX)
	{
		for (int i = 0; i < 16; i++)
			iss >> t.v[i];
	}
	node->transformations.push_back(t);
}

void ColladaLoader::DeleteNodes(Node *node)
{
	for (unsigned int i = 0; i < node->children.size(); i++)
		DeleteNodes(node->children[i]);
	delete node;
}

mat4 ColladaLoader::CalculateTranformation(std::vector<Transform> &transformations)
{
	mat4 ret = mat4::IDENTITY;
	for (unsigned int i = 0; i < transformations.size(); i++)
	{
		Transform &t = transformations[i];
		mat4 temp = mat4::IDENTITY;
		if (t.type == TRANSFORM_TRANSLATE)
		{
			temp.Translate(t.v[0], t.v[1], t.v[2]);
			ret *= temp;
		}
		else if (t.type == TRANSFORM_SCALE)
		{
			temp.Scale(t.v[0], t.v[1], t.v[2]);
			ret *= temp;
		}
		else if (t.type == TRANSFORM_ROTATE)
		{
			float angle = (float)(-t.v[3] / 180.0f * PI);

			temp.Rotate(vec3(t.v[0], t.v[1], t.v[2]), angle);
			ret *= temp;
		}
		else if (t.type == TRANSFORM_MATRIX)
		{
			for (unsigned int j = 0; j < 16; j++)
				temp.m[j] = t.v[j];
			ret *= temp;
		}
		else if (t.type == TRANSFORM_LOOKAT)
		{
			vec3 pos(t.v[0], t.v[1], t.v[2]);
			vec3 dst(t.v[3], t.v[4], t.v[5]);
			vec3 up(t.v[6], t.v[7], t.v[8]);
			up.Normalize();
			temp.LookAt(pos, dst, up);
			ret *= temp;
		}
	}
	return ret;
}

vec3 ColladaLoader::GetTranslation(std::vector<Transform> &transformations)
{
	vec3 ret(0, 0, 0);
	for (unsigned int i = 0; i < transformations.size(); i++)
	{
		Transform &t = transformations[i];
		if (t.type == TRANSFORM_TRANSLATE)
		{
			ret += vec3(t.v[0], t.v[1], t.v[2]);
		}
	}
	return ret;
}

mat4 ColladaLoader::GetRotation(std::vector<Transform> &transformations)
{
	mat4 ret = mat4::IDENTITY;

	for (unsigned int i = 0; i < transformations.size(); i++)
	{
		Transform &t = transformations[i];
		if (t.type == TRANSFORM_ROTATE)
		{			
			float angle = (float)(-t.v[3] / 180.0f * PI);			
			ret.Rotate(vec3(t.v[0], t.v[1], t.v[2]), angle);			

		}
	}
	return ret;
}

vec3 ColladaLoader::GetScale(std::vector<Transform> &transformations)
{
	vec3 ret(1, 1, 1);
	for (unsigned int i = 0; i < transformations.size(); i++)
	{
		Transform &t = transformations[i];
		if (t.type == TRANSFORM_SCALE)
		{
			ret.x *= t.v[0];
			ret.y *= t.v[1];
			ret.z *= t.v[2];
		}
	}
	return ret;
}

mat4 ColladaLoader::GetTransformMatrix(std::vector<Transform> &transformations)
{
	mat4 ret = mat4::IDENTITY;
	for (unsigned int i = 0; i < transformations.size(); i++)
	{
		Transform &t = transformations[i];
		mat4 temp = mat4::IDENTITY;
		if (t.type == TRANSFORM_MATRIX)
		{
			for (unsigned int j = 0; j < 16; j++)
				temp.m[j] = t.v[j];
			ret *= temp;
		}
		else if (t.type == TRANSFORM_LOOKAT)
		{
			vec3 pos(t.v[0], t.v[1], t.v[2]);
			vec3 dst(t.v[3], t.v[4], t.v[5]);
			vec3 up(t.v[6], t.v[7], t.v[8]);
			up.Normalize();
			temp.LookAt(pos, dst, up);
			ret *= temp;
		}
	}
	return ret;
}

void ColladaLoader::ApplyMaterialInstanceSemanticMapping(Sampler &sampler, MaterialInstance &materialInstance)
{
	// Applies the semantic mapping between texture coordinates in an effect
	// and texture coordinates defined in a geometry
	std::map<std::string, InputSemanticEntry>::iterator iter = materialInstance.inputMap.find(sampler.uvCoords);
	sampler.uvId = -1;
	if (iter != materialInstance.inputMap.end())
	{
		if (iter->second.type == INPUT_TEXCOORD)
			sampler.uvId = iter->second.set;
	}
}

std::string ColladaLoader::GetTextureFileNameFromSampler(Effect &effect, Sampler &sampler)
{
	// A sampler points to a surface param in an effect which points to an image in the image library
	std::string cur = sampler.name;
	std::map<std::string, EffectParam>::iterator i;
	while ((i = effect.effectParams.find(cur)) != effect.effectParams.end())
	{
		cur = i->second.reference;
	}
	std::map<std::string, ColladaLoader::Image>::iterator imgi = imageLibrary.find(cur);
	if (imgi == imageLibrary.end())
		return "";
	else
	{
		return imgi->second.initFrom;
	}
}

std::string ColladaLoader::FixFileName(std::string &filename)
{
	// Removes "file://" and replaces "%20" with spaces
	std::string::size_type i = 0;
	std::string file = filename;
	while ((file[i] == '.') || (file[i] == '/'))
		i++;
	file = file.substr(i);
	i = file.find("file://");
	if (i != std::string::npos)
	{
		file.erase(i, i + 7);
	}
	i = file.find("%20");
	while (i != std::string::npos)
	{
		file.replace(i, 3, " ");
		i = file.find("%20");
	}
	return file;
}

/*
FireCube::NodePtr ColladaLoader::GenerateSceneGraph(Node *node)
{	
	// This function converts a given Collada node to the engine's Node class

	// Allocate a new node.
	FireCube::NodePtr ret(new FireCube::Node(node->name));

	// Calculate the node's transformation.
	mat4 transformation = GetTransformMatrix(node->transformations);
	vec3 translation;
	mat3 rotation;
	vec3 scale;
	transformation.Decompose(scale, translation, rotation);
	ret->SetTranslation(translation);
	ret->SetRotation(rotation.ToMat4());
	ret->SetScale(scale);	

	// Iterate over all geometry instances in this node
	// In Collada terms, each mesh can have a group of triangles with different materials.
	// We translate it to the engine's world by creating a node for the geometry and then creating a child node
	// for each group of triangles with a given material and make it point to the generated engine's Geometry class.
	for (unsigned int i = 0; i < node->geometryInstances.size(); i++)
	{
		FireCube::NodePtr geomNode(new FireCube::Node);
		GeometryInstance &gi = node->geometryInstances[i];
		std::map<std::string, Geometry>::iterator iter = geometryLibrary.find(gi.url);
		if (iter == geometryLibrary.end())
			continue;
		Geometry &geom = iter->second;
		geomNode->SetName(geom.name);
		ret->AddChild(geomNode);
		Mesh &mesh = geom.mesh;               
		
		// Flip vertices, normals, tangents and bitangents if needed
		std::vector<vec3> vertices = mesh.vertices;
		for (unsigned int j = 0; j < vertices.size(); j++)
		{
			if (upDirection == X_UP)
			{
				std::swap(vertices[j].x, vertices[j].y);
				vertices[j].x *= -1;
			}
			else if (upDirection == Z_UP)
			{
				std::swap(vertices[j].y, vertices[j].z);
				vertices[j].z *= -1;
			}
		}        
		std::vector<vec3> normals = mesh.normals;
		for (unsigned int j = 0; j < normals.size(); j++)
		{            
			if (upDirection == X_UP)
			{
				std::swap(normals[j].x, normals[j].y);
				normals[j].x *= -1;
			}
			else if (upDirection == Z_UP)
			{
				std::swap(normals[j].y, normals[j].z);
				normals[j].z *= -1;
			}
			normals[j].Normalize();
		}        
		std::vector<vec3> tangents = mesh.tangents;
		for (unsigned int j = 0; j < tangents.size(); j++)
		{            
			if (upDirection == X_UP)
			{
				std::swap(tangents[j].x, tangents[j].y);
				tangents[j].x *= -1;
			}
			else if (upDirection == Z_UP)
			{
				std::swap(tangents[j].y, tangents[j].z);
				tangents[j].z *= -1;
			}
			tangents[j].Normalize();
		}        
		std::vector<vec3> bitangents = mesh.binormals;
		for (unsigned int j = 0; j < bitangents.size(); j++)
		{            
			if (upDirection == X_UP)
			{
				std::swap(bitangents[j].x, bitangents[j].y);
				bitangents[j].x *= -1;
			}
			else if (upDirection == Z_UP)
			{
				std::swap(bitangents[j].y, bitangents[j].z);
				bitangents[j].z *= -1;
			}
			bitangents[j].Normalize();
		}		

		// Iterate over every sub mesh and create a Geometry and a Node for it
		unsigned int vertexIndex = 0;
		for (unsigned int j = 0; j < mesh.subMeshes.size(); j++)
		{
			FireCube::GeometryNodePtr surfaceNode(new FireCube::GeometryNode());
			std::ostringstream surfaceNodeName;
			surfaceNodeName << geomNode->GetName() << "-surface-" << j;
			surfaceNode->SetName(surfaceNodeName.str());
			FireCube::GeometryPtr geometry(new FireCube::Geometry);
			SubMesh &subMesh = mesh.subMeshes[j];

			// Use the material instance to resolve the material name of this sub mesh
			std::map<std::string, MaterialInstance>::iterator iter = gi.materialInstance.find(subMesh.material);
			MaterialInstance *materialInstance = nullptr;
			if (iter != gi.materialInstance.end())
				materialInstance = &iter->second;
			Material &mat = materialLibrary[materialInstance->materialName];
			Effect &effect = effectLibrary[mat.effect];
			// Add the newly created node as a child of the geometry node
			geomNode->AddChild(surfaceNode);
			surfaceNode->SetGeometry(geometry);
			geometry->GetVertices() = vertices;
			geometry->GetNormals() = normals;
			geometry->GetTangents() = tangents;
			geometry->GetBitangents() = bitangents;
			FireCube::MaterialPtr fmat;
			std::map<std::string, FireCube::MaterialPtr>:: iterator matIter = generatedMaterials.find(subMesh.material);
			if (matIter != generatedMaterials.end())
			{
				fmat = matIter->second;
			}
			else
			{
				fmat = FireCube::MaterialPtr(new FireCube::Material);
				generatedMaterials[subMesh.material] = fmat;
				fmat->SetAmbientColor(vec3(effect.ambientColor.x, effect.ambientColor.y, effect.ambientColor.z));
				fmat->SetDiffuseColor(vec3(effect.diffuseColor.x, effect.diffuseColor.y, effect.diffuseColor.z));
				fmat->SetSpecularColor(vec3(effect.specularColor.x, effect.specularColor.y, effect.specularColor.z));
				fmat->SetShininess(effect.shininess);
				fmat->SetName(subMesh.material);            
				if (effect.diffuseSampler.name != "")
				{
					fmat->SetDiffuseTexture(Renderer::GetTexturePool().Create(GetTextureFileNameFromSampler(effect, effect.diffuseSampler)));
				}
			}
			geometry->SetMaterial(fmat);
			
			if (materialInstance)
			{
				// Resolve the set number of the texture coordinates associated with each sampler
				ApplyMaterialInstanceSemanticMapping(effect.ambientSampler, *materialInstance);
				ApplyMaterialInstanceSemanticMapping(effect.diffuseSampler, *materialInstance);
				ApplyMaterialInstanceSemanticMapping(effect.specularSampler, *materialInstance);
			}

			if (effect.diffuseSampler.name != "")
			{				
				unsigned int map = 0;                
				if (effect.diffuseSampler.uvId != -1)
					map = effect.diffuseSampler.uvId;
				else
				{
					// If the set number was not specified by the material instance try to get it from the texture
					// coordinates name of the sampler
					std::string::iterator s = effect.diffuseSampler.uvCoords.begin();
					while (s != effect.diffuseSampler.uvCoords.end() && (*s < '0' || *s > '9'))
						s++;
					map = 0;
					while (s != effect.diffuseSampler.uvCoords.end() && (*s >= '0' && *s <= '9'))
					{
						map = map * 10 + (*s - '0');
						s++;
					}
				}
				geometry->GetDiffuseUV() = mesh.texcoords[map];
				if (options.flipU || options.flipV)
				{
					for (unsigned int k = 0; k < geometry->GetDiffuseUV().size(); k++)
					{
						if (options.flipU)
							geometry->GetDiffuseUV()[k].x = 1 - geometry->GetDiffuseUV()[k].x;
						if (options.flipV)
							geometry->GetDiffuseUV()[k].y = 1 - geometry->GetDiffuseUV()[k].y;
					}
				}				
			}
			
			// Create the faces of this geometry
			if (subMesh.primitiveType == PRIMITIVE_TRIANGLES)
			{
				for (int p = 0; p < subMesh.numPrimtives; p++)
				{
					Face f;
					f.v[0] = subMesh.indices[p * 3 + 0];
					f.v[1] = subMesh.indices[p * 3 + 1];
					f.v[2] = subMesh.indices[p * 3 + 2];                    
					f.normal = Cross(vertices[f.v[1]] - vertices[f.v[0]], vertices[f.v[2]] - vertices[f.v[0]]);
					f.normal.Normalize();
					geometry->GetFaces().push_back(f);
				}
			}
			else if (subMesh.primitiveType == PRIMITIVE_POLYLIST)
			{
				for (int p = 0; p < subMesh.numPrimtives; p++)
				{
					if (subMesh.vcount[p] != 3)
						continue;
					Face f;
					f.v[0] = subMesh.indices[p * 3 + 0];
					f.v[1] = subMesh.indices[p * 3 + 1];
					f.v[2] = subMesh.indices[p * 3 + 2];
					f.normal = Cross(vertices[f.v[1]] - vertices[f.v[0]], vertices[f.v[2]] - vertices[f.v[0]]);
					f.normal.Normalize();
					geometry->GetFaces().push_back(f);
				}
			}
			if (geometry->GetTangents().size() == 0)
				geometry->CalculateTangents();
			geometry->CopyFacesToIndices();
			geometry->SetPrimitiveType(TRIANGLES);
			geometry->SetPrimitiveCount(geometry->GetFaces().size());
			geometry->SetVertexCount(geometry->GetFaces().size() * 3);
			geometry->CalculateBoundingBox();
			geometry->UpdateBuffers();
		}    
	}
	
	// Recursively generate a scene graph for this node's children
	for (unsigned int i = 0; i < node->children.size(); i++)
	{
		FireCube::NodePtr c = GenerateSceneGraph(node->children[i]);
		ret->AddChild(c);
	}

	return ret;
}

FireCube::NodePtr ColladaLoader::GenerateSceneGraph()
{
	FireCube::NodePtr ret = GenerateSceneGraph(root);    
	return ret;
}

*/

void ColladaLoader::GenerateGeometries(Renderer *renderer, Node *node, mat4 parentWorldTransform)
{
	// This function walks the scene graph in the collada file and generates a list of geometries to be used by the engine

	// Calculate the node's transformation.
	mat4 transformation = CalculateTranformation(node->transformations);
	vec3 translation;
	mat3 rotation;
	vec3 scale;
	transformation.Decompose(scale, translation, rotation);
	mat4 worldTransform = parentWorldTransform * transformation;
	
	// Iterate over all geometry instances in this node
	// In Collada terms, each mesh can have a group of triangles with different materials.
	// We translate it to the engine's world by creating a geometry for each group of triangles.
	for (unsigned int i = 0; i < node->geometryInstances.size(); i++)
	{		
		GeometryInstance &gi = node->geometryInstances[i];
		std::map<std::string, Geometry>::iterator iter = geometryLibrary.find(gi.url);
		if (iter == geometryLibrary.end())
			continue;
		Geometry &geom = iter->second;
		//geomNode->SetName(geom.name);		
		Mesh &mesh = geom.mesh;               

		// Flip vertices, normals, tangents and bitangents if needed
		std::vector<vec3> vertices = mesh.vertices;
		for (unsigned int j = 0; j < vertices.size(); j++)
		{
			if (upDirection == X_UP)
			{
				std::swap(vertices[j].x, vertices[j].y);
				vertices[j].x *= -1;
			}
			else if (upDirection == Z_UP)
			{
				std::swap(vertices[j].y, vertices[j].z);
				vertices[j].z *= -1;
			}
			vertices[j] = worldTransform * vertices[j];
		}        
		std::vector<vec3> normals = mesh.normals;
		for (unsigned int j = 0; j < normals.size(); j++)
		{            
			if (upDirection == X_UP)
			{
				std::swap(normals[j].x, normals[j].y);
				normals[j].x *= -1;
			}
			else if (upDirection == Z_UP)
			{
				std::swap(normals[j].y, normals[j].z);
				normals[j].z *= -1;
			}
			normals[j].TransformNormal(worldTransform);
			normals[j].Normalize();
		}        
		std::vector<vec3> tangents = mesh.tangents;
		for (unsigned int j = 0; j < tangents.size(); j++)
		{            
			if (upDirection == X_UP)
			{
				std::swap(tangents[j].x, tangents[j].y);
				tangents[j].x *= -1;
			}
			else if (upDirection == Z_UP)
			{
				std::swap(tangents[j].y, tangents[j].z);
				tangents[j].z *= -1;
			}
			tangents[j].TransformNormal(worldTransform);
			tangents[j].Normalize();
		}        
		std::vector<vec3> bitangents = mesh.binormals;
		for (unsigned int j = 0; j < bitangents.size(); j++)
		{            
			if (upDirection == X_UP)
			{
				std::swap(bitangents[j].x, bitangents[j].y);
				bitangents[j].x *= -1;
			}
			else if (upDirection == Z_UP)
			{
				std::swap(bitangents[j].y, bitangents[j].z);
				bitangents[j].z *= -1;
			}
			bitangents[j].TransformNormal(worldTransform);
			bitangents[j].Normalize();
		}		

		// Iterate over every sub mesh and create a Geometry for it
		unsigned int vertexIndex = 0;
		for (unsigned int j = 0; j < mesh.subMeshes.size(); j++)
		{						
			FireCube::Geometry *geometry = new FireCube::Geometry(renderer);
			generatedGeometries.push_back(geometry);
			SubMesh &subMesh = mesh.subMeshes[j];

			// Use the material instance to resolve the material name of this sub mesh
			std::map<std::string, MaterialInstance>::iterator iter = gi.materialInstance.find(subMesh.material);
			MaterialInstance *materialInstance = nullptr;
			if (iter != gi.materialInstance.end())
				materialInstance = &iter->second;
			Material &mat = materialLibrary[materialInstance->materialName];
			Effect &effect = effectLibrary[mat.effect];
			// Add the newly created node as a child of the geometry node			
			
			FireCube::Material *fmat;
			std::map<std::string, FireCube::Material *>:: iterator matIter = allMaterials.find(subMesh.material);
			if (matIter != allMaterials.end())
			{
				fmat = matIter->second;
			}
			else
			{
				fmat = new FireCube::Material(engine);
				allMaterials[subMesh.material] = fmat;
				fmat->SetParameter(PARAM_MATERIAL_AMBIENT, vec4(effect.ambientColor.x, effect.ambientColor.y, effect.ambientColor.z, 1.0f));
				fmat->SetParameter(PARAM_MATERIAL_DIFFUSE, vec4(effect.diffuseColor.x, effect.diffuseColor.y, effect.diffuseColor.z, 1.0f));
				fmat->SetParameter(PARAM_MATERIAL_SPECULAR, vec4(effect.specularColor.x, effect.specularColor.y, effect.specularColor.z, 1.0f));
				fmat->SetParameter(PARAM_MATERIAL_SHININESS, effect.shininess);
				fmat->SetName(subMesh.material);            
				if (effect.diffuseSampler.name != "")
				{		
					fmat->SetTechnique(engine->GetResourceCache()->GetResource<Technique>("Techniques/DiffuseMap.xml"));
					fmat->SetTexture(TEXTURE_UNIT_DIFFUSE, engine->GetResourceCache()->GetResource<Texture>(GetTextureFileNameFromSampler(effect, effect.diffuseSampler)));
				}
				else
					fmat->SetTechnique(engine->GetResourceCache()->GetResource<Technique>("Techniques/NoTexture.xml"));
			}
			generatedMaterials.push_back(fmat);			

			if (materialInstance)
			{
				// Resolve the set number of the texture coordinates associated with each sampler
				ApplyMaterialInstanceSemanticMapping(effect.ambientSampler, *materialInstance);
				ApplyMaterialInstanceSemanticMapping(effect.diffuseSampler, *materialInstance);
				ApplyMaterialInstanceSemanticMapping(effect.specularSampler, *materialInstance);
			}
			std::vector<vec2> uv;
			if (effect.diffuseSampler.name != "")
			{				
				unsigned int map = 0;                
				if (effect.diffuseSampler.uvId != -1)
					map = effect.diffuseSampler.uvId;
				else
				{
					// If the set number was not specified by the material instance try to get it from the texture
					// coordinates name of the sampler
					std::string::iterator s = effect.diffuseSampler.uvCoords.begin();
					while (s != effect.diffuseSampler.uvCoords.end() && (*s < '0' || *s > '9'))
						s++;
					map = 0;
					while (s != effect.diffuseSampler.uvCoords.end() && (*s >= '0' && *s <= '9'))
					{
						map = map * 10 + (*s - '0');
						s++;
					}
				}
				uv = mesh.texcoords[map];
				if (options.flipU || options.flipV)
				{
					for (unsigned int k = 0; k < uv.size(); k++)
					{
						if (options.flipU)
							uv[k].x = 1 - uv[k].x;
						if (options.flipV)
							uv[k].y = 1 - uv[k].y;
					}
				}				
			}

			// Create the faces of this geometry
			std::vector<unsigned int> indexData;
			if (subMesh.primitiveType == PRIMITIVE_TRIANGLES)
			{
				for (int p = 0; p < subMesh.numPrimtives; p++)
				{					
					indexData.push_back(subMesh.indices[p * 3 + 0]);
					indexData.push_back(subMesh.indices[p * 3 + 1]);
					indexData.push_back(subMesh.indices[p * 3 + 2]);
				}
			}
			else if (subMesh.primitiveType == PRIMITIVE_POLYLIST)
			{
				for (int p = 0; p < subMesh.numPrimtives; p++)
				{
					if (subMesh.vcount[p] != 3)
						continue;					
					indexData.push_back(subMesh.indices[p * 3 + 0]);
					indexData.push_back(subMesh.indices[p * 3 + 1]);
					indexData.push_back(subMesh.indices[p * 3 + 2]);
				}
			}

			VertexBuffer *vertexBuffer = new VertexBuffer(renderer);
			IndexBuffer *indexBuffer = new IndexBuffer(renderer);
			vertexBuffer->SetShadowed(true);
			indexBuffer->SetShadowed(true);
			geometry->SetVertexBuffer(vertexBuffer);
			geometry->SetIndexBuffer(indexBuffer);

			/*if (normals.empty())
				CalculateNormals(normals, vertices, indexData);*/

			/*if (geometry->GetTangents().size() == 0)
				geometry->CalculateTangents();*/

			VertexAttribute vertexAttributes = VertexAttribute::POSITION | VertexAttribute::NORMAL;
			unsigned int vertexSize = 6;
			if (uv.empty() == false)
			{
				vertexAttributes |= VertexAttribute::TEXCOORD0;
				vertexSize += 2;
			}
			if (tangents.empty() == false)
			{
				vertexAttributes |= VertexAttribute::TANGENT;
				vertexSize += 3;
			}
			
			std::vector<float> vertexData(vertexSize * vertices.size());
			for (unsigned int k = 0; k < vertices.size(); ++k)
			{
				*((vec3 *) &vertexData[k * vertexSize + 0]) = vertices[k];
				*((vec3 *) &vertexData[k * vertexSize + 3]) = normals[k];
				unsigned int currentOffset = 6;
				if ((vertexAttributes & VertexAttribute::TEXCOORD0) == VertexAttribute::TEXCOORD0)
				{
					*((vec2 *) &vertexData[k * vertexSize + currentOffset]) = uv[k];
					currentOffset += 2;
				}
				if ((vertexAttributes & VertexAttribute::TANGENT) == VertexAttribute::TANGENT)
				{
					*((vec3 *) &vertexData[k * vertexSize + currentOffset]) = tangents[k];
					currentOffset += 3;
				}				
				boundingBox.Expand(vertices[k]);
			}			

			vertexBuffer->LoadData(&vertexData[0], vertices.size(), vertexAttributes, STATIC);			
			indexBuffer->LoadData(&indexData[0], indexData.size(), STATIC);
			
			geometry->SetPrimitiveType(TRIANGLES);
			geometry->SetPrimitiveCount(indexData.size() / 3);
			geometry->Update();
		}    
	}

	// Recursively generate geometries for this node's children
	for (unsigned int i = 0; i < node->children.size(); i++)
	{
		GenerateGeometries(renderer, node->children[i], worldTransform);		
	}	
}

BoundingBox ColladaLoader::GetBoundingBox() const
{
	return boundingBox;
}

void ColladaLoader::GenerateGeometries(Renderer *renderer)
{
	generatedGeometries.clear();
	GenerateGeometries(renderer, root, mat4::IDENTITY);	
}

const std::vector<FireCube::Geometry *> &ColladaLoader::GetGeneratedGeometries()
{
	return generatedGeometries;
}

TiXmlElement *ColladaLoader::GetChildElement(TiXmlNode *node, const std::string &elmName)
{
	TiXmlNode *n = node->FirstChild(elmName);
	if (n && n->Type() == TiXmlNode::ELEMENT)
		return n->ToElement();
	return nullptr;
}

ColladaLoader::InputType ColladaLoader::SemanticToInputType(const std::string &semantic)
{
	if (semantic == "POSITION")
		return INPUT_POSITION;
	else if (semantic == "VERTEX")
		return INPUT_VERTEX;
	else if (semantic == "NORMAL")
		return INPUT_NORMAL;
	else if (semantic == "TEXCOORD")
		return INPUT_TEXCOORD;
	else if (semantic == "COLOR")
		return INPUT_COLOR;
	else if (semantic == "TEXBINORMAL")
		return INPUT_BINORMAL;
	else if (semantic == "TEXTANGENT")
		return INPUT_TANGENT;
	else
		return INPUT_UNKNOWN;
}
