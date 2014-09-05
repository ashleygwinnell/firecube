#pragma once

#pragma warning(push)
#pragma warning(disable:4251)

#include <string>
#include <vector>
#include <map>

#include "Math/Math.h"
#include "Scene/Node.h"
#include "tinyxml.h"
#include "Geometry/ModelLoader.h"

namespace FireCube
{	
	class ColladaLoader : public ModelLoader
	{
	public:
		ColladaLoader(Engine *engine);
		~ColladaLoader();
		virtual bool Load(const std::string &filename, ModelLoadingOptions options = ModelLoadingOptions());		
		virtual void GenerateGeometries(Renderer *renderer);
		virtual void GenerateScene(Renderer *renderer, Node *root);
		virtual const std::vector<Geometry *> &GetGeneratedGeometries();
		virtual Node *GetGeneratedScene();
		virtual const std::vector<Material *> &GetGeneratedMaterials();
		virtual BoundingBox GetBoundingBox() const;
	private:
		enum class UpDirection
		{
			X, Y, Z
		};
		enum class ParamType
		{
			SAMPLER_2D, SURFACE
		};
		enum class ShadingType
		{
			PHONG, CONSTANT, LAMBERT, BLINN
		};
		enum class InputType
		{
			POSITION, VERTEX, NORMAL, TEXCOORD, COLOR, TANGENT, BINORMAL, UNKNOWN
		};
		enum class PrimitiveType
		{
			TRIANGLES, POLYLIST
		};
		enum class TransformationType
		{
			LOOKAT, ROTATE, TRANSLATE, MATRIX, SCALE
		};
		class Transform
		{
		public:
			TransformationType type;
			float v[16];
		};
		class Sampler
		{
		public:
			std::string name;
			std::string uvCoords;
			int uvId;
		};
		class Image
		{
		public:
			std::string initFrom;
		};
		class Material
		{
		public:
			std::string effect;
		};
		class EffectParam
		{
		public:
			ParamType type;
			std::string reference;
		};
		class DataArray
		{
		public:
			std::vector<float> floatData;
		};
		class Accessor
		{
		public:
			int count, offset, stride;			
			int componentsOffset[4];			
		};
		class Source
		{
		public:
			Accessor accessor;
			DataArray dataArray;
		};
		class SubMesh
		{
		public:
			PrimitiveType primitiveType;
			std::string material;
			int numPrimtives;
			std::vector<unsigned int> indices;
			std::vector<unsigned int> vcount;
		};
		class InputChannel
		{
		public:
			InputChannel() : source(NULL)
			{

			}
			InputType type;
			std::string sourceName;
			Source *source;
			int offset;
			int index;
		};
		class Mesh
		{
		public:
			std::vector<InputChannel> inputChannels;
			std::vector <SubMesh> subMeshes;
			std::string vertexId;
			std::vector<vec3> vertices;
			std::vector<vec3> normals;
			std::vector<vec3> tangents;
			std::vector<vec3> binormals;
			std::vector<vec2> texcoords[4];
		};
		class NodeInstance
		{
		public:
			std::string url;
		};
		class InputSemanticEntry
		{
		public:
			int set;
			InputType type;
		};
		class MaterialInstance
		{
		public:
			std::string materialName;
			std::map<std::string, InputSemanticEntry> inputMap;
		};
		class GeometryInstance
		{
		public:
			std::string url;
			std::map<std::string, MaterialInstance> materialInstance;
		};
		class Node
		{
		public:
			std::string name, id, sid;
			std::vector<Node*> children;
			std::vector<NodeInstance> nodeInstances;
			std::vector<GeometryInstance> geometryInstances;
			std::vector<Transform> transformations;
			Node *parent;
		};
		class Geometry
		{
		public:
			std::string name;
			Mesh mesh;
		};
		class Effect
		{
		public:
			Effect() : ambientColor(1.0f, 1.0f, 1.0f, 1.0f), diffuseColor(1.0f, 1.0f, 1.0f, 1.0f), specularColor(0.0f, 0.0f, 0.0f, 0.0f), shininess(128.0f)
			{
			}
			ShadingType shadingType;
			vec4 ambientColor, diffuseColor, specularColor;
			Sampler ambientSampler, diffuseSampler, specularSampler;
			float shininess;
			std::map<std::string, EffectParam> effectParams;
		};		
		TiXmlElement *GetChildElement(TiXmlNode *node, const std::string &elmName);
		void ReadLibraries(TiXmlNode *parent);
		void ReadAsset(TiXmlNode *parent);
		void ReadImageLibrary(TiXmlNode *parent);
		void ReadImage(TiXmlNode *parent, Image &img);
		void ReadMaterialLibrary(TiXmlNode *parent);
		void ReadMaterial(TiXmlNode *parent, Material &mat);
		void ReadEffectLibrary(TiXmlNode *parent);
		void ReadEffect(TiXmlNode *parent, Effect &effect);
		void ReadEffectProfileCommon(TiXmlNode *parent, Effect &effect);
		void ReadEffectParam(TiXmlNode *parent, EffectParam &effectParam);
		void ReadEffectColor(TiXmlNode *parent, vec4 &color, Sampler &sampler);
		void ReadEffectFloat(TiXmlNode *parent, float &value);
		void ReadGeometryLibrary(TiXmlNode *parent);
		void ReadGeometry(TiXmlNode *parent, Geometry &geometry);
		void ReadMesh(TiXmlNode *parent, Mesh &mesh);
		void ReadSource(TiXmlNode *parent, Source &source);
		void ReadDataArray(TiXmlNode *parent, DataArray &dataArray);
		void ReadAccessor(TiXmlNode *parent, Accessor &accessor);
		void ReadVertexData(TiXmlNode *parent, Mesh &mesh);
		void ReadInputChannel(TiXmlNode *parent, std::vector<InputChannel> &inputChannels);
		void ReadTriangles(TiXmlNode *parent, Mesh &mesh);
		void ReadPolylist(TiXmlNode *parent, Mesh &mesh);
		void ReadVCount(TiXmlNode *parent, SubMesh &subMesh);
		void ReadPrimitives(TiXmlNode *parent, Mesh &mesh, SubMesh &subMesh, std::vector<InputChannel> primInputChannels, int count);
		void GetDataFromChannel(InputChannel &ic, int index, Mesh &mesh);
		void ReadSceneLibrary(TiXmlNode *parent);
		void ReadSceneNode(TiXmlNode *parent, Node *node);
		void ReadNodeGeometryInstance(TiXmlNode *parent, GeometryInstance &gi);
		void ReadMaterialVertexInputBinding(TiXmlNode *parent, MaterialInstance &materialInstance);
		void ReadScene(TiXmlNode *parent);
		void ReadTransformation(TiXmlNode *parent, Node *node);
		mat4 CalculateTranformation(std::vector<Transform> &transformations);
		vec3 GetTranslation(std::vector<Transform> &transformations);
		mat4 GetRotation(std::vector<Transform> &transformations);
		vec3 GetScale(std::vector<Transform> &transformations);
		mat4 GetTransformMatrix(std::vector<Transform> &transformations);
		void DeleteNodes(Node *node);
		void ApplyMaterialInstanceSemanticMapping(Sampler &sampler, MaterialInstance &materialInstance);
		//FireCube::NodePtr GenerateSceneGraph(Node *node);		
		void GenerateGeometries(Renderer *renderer, Node *node, mat4 parentWorldTransform);		

		InputType SemanticToInputType(const std::string &semantic);
		template <typename Type>
		Type& ResolveLibraryReference(std::map<std::string, Type> &pLibrary, const std::string &pURL)
		{
			typename std::map<std::string, Type>::iterator it = pLibrary.find( pURL);
			return it->second;
		}
		std::string GetTextureFileNameFromSampler(Effect &effect, Sampler &sampler);
		std::string FixFileName(std::string &filename);
	
		TiXmlDocument xmlDocument;

		std::map<std::string, Image> imageLibrary;
		std::map<std::string, Material> materialLibrary;
		std::map<std::string, Effect> effectLibrary;
		std::map<std::string, Geometry> geometryLibrary;
		std::map<std::string, Source> sources;
		std::map<std::string, Node*> nodeLibrary;
		std::map<unsigned int, unsigned int> tempMap;
		std::map<std::string, FireCube::Material *> allMaterials;
		std::vector<FireCube::Material *> generatedMaterials;
		Node *root;
		float unit;
		UpDirection upDirection;
		ModelLoadingOptions options;
		std::vector<FireCube::Geometry *> generatedGeometries;
		BoundingBox boundingBox;
	};
}

#pragma warning(pop)
