#ifndef COLLADA_LOADER_H
#define COLLADA_LOADER_H

#pragma warning(push)
#pragma warning(disable:4251)

namespace FireCube
{
	class ColladaLoader
	{
	public:
		enum UpDirection
		{
			X_UP, Y_UP, Z_UP
		};
		enum ParamType
		{
			PARAM_SAMPLER_2D, PARAM_SURFACE
		};
		enum ShadingType
		{
			SHADING_PHONG, SHADING_CONSTANT, SHADING_LAMBERT, SHADING_BLINN
		};
		enum InputType
		{
			INPUT_POSITION, INPUT_VERTEX, INPUT_NORMAL, INPUT_TEXCOORD, INPUT_COLOR, INPUT_TANGENT, INPUT_BINORMAL, INPUT_UNKNOWN
		};
		enum PrimitiveType
		{
			PRIMITIVE_TRIANGLES, PRIMITIVE_POLYLIST
		};
		enum TransformationType
		{
			TRANSFORM_LOOKAT, TRANSFORM_ROTATE, TRANSFORM_TRANSLATE, TRANSFORM_MATRIX, TRANSFORM_SCALE
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
			string name;
			string uvCoords;
			int uvId;
		};
		class Image
		{
		public:
			string initFrom;
		};
		class Material
		{
		public:
			string effect;
		};
		class EffectParam
		{
		public:
			ParamType type;
			string reference;
		};
		class DataArray
		{
		public:
			vector<float> floatData;
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
			string material;
			int numPrimtives;
			vector<unsigned int> indices;
			vector<unsigned int> vcount;
		};
		class InputChannel
		{
		public:
			InputChannel() : source(NULL)
			{

			}
			InputType type;
			string sourceName;
			Source *source;
			int offset;
			int index;
		};
		class Mesh
		{
		public:
			vector<InputChannel> inputChannels;
			vector <SubMesh> subMeshes;
			string vertexId;
			vector<vec3> vertices;
			vector<vec3> normals;
			vector<vec3> tangents;
			vector<vec3> binormals;
			vector<vec2> texcoords[4];
		};
		class NodeInstance
		{
		public:
			string url;
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
			string materialName;
			map <string, InputSemanticEntry> inputMap;
		};
		class GeometryInstance
		{
		public:
			string url;
			map<string, MaterialInstance> materialInstance;
		};
		class Node
		{
		public:
			string name, id, sid;
			vector<Node*> children;
			vector<NodeInstance> nodeInstances;
			vector<GeometryInstance> geometryInstances;
			vector<Transform> transformations;
			Node *parent;
		};
		class Geometry
		{
		public:
			string name;
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
			map<string, EffectParam> effectParams;
		};
		ColladaLoader(const string &filename);
		~ColladaLoader();
		bool Load(ModelLoadingOptions options);
		TiXmlElement *GetChildElement(TiXmlNode *node, const string &elmName);
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
		void ReadInputChannel(TiXmlNode *parent, vector<InputChannel> &inputChannels);
		void ReadTriangles(TiXmlNode *parent, Mesh &mesh);
		void ReadPolylist(TiXmlNode *parent, Mesh &mesh);
		void ReadVCount(TiXmlNode *parent, SubMesh &subMesh);
		void ReadPrimitives(TiXmlNode *parent, Mesh &mesh, SubMesh &subMesh, vector<InputChannel> primInputChannels, int count);
		void GetDataFromChannel(InputChannel &ic, int index, Mesh &mesh);
		void ReadSceneLibrary(TiXmlNode *parent);
		void ReadSceneNode(TiXmlNode *parent, Node *node);
		void ReadNodeGeometryInstance(TiXmlNode *parent, GeometryInstance &gi);
		void ReadMaterialVertexInputBinding(TiXmlNode *parent, MaterialInstance &materialInstance);
		void ReadScene(TiXmlNode *parent);
		void ReadTransformation(TiXmlNode *parent, Node *node);
		mat4 CalculateTranformation(vector<Transform> &transformations);
		vec3 GetTranslation(vector<Transform> &transformations);
		mat4 GetRotation(vector<Transform> &transformations);
		vec3 GetScale(vector<Transform> &transformations);
		mat4 GetTransformMatrix(vector<Transform> &transformations);
		void DeleteNodes(Node *node);
		void ApplyMaterialInstanceSemanticMapping(Sampler &sampler, MaterialInstance &materialInstance);
		FireCube::NodePtr GenerateSceneGraph(Node *node);
		FireCube::NodePtr GenerateSceneGraph();
		InputType SemanticToInputType(const string &semantic);
		template <typename Type>
		Type& ResolveLibraryReference( std::map<std::string, Type>& pLibrary, const std::string& pURL)
		{
			typename std::map<std::string, Type>::iterator it = pLibrary.find( pURL);
			return it->second;
		}
		string GetTextureFileNameFromSampler(Effect &effect, Sampler &sampler);
		string FixFileName(string &filename);
	private:
		TiXmlDocument xmlDocument;

		map<string, Image> imageLibrary;
		map<string, Material> materialLibrary;
		map<string, Effect> effectLibrary;
		map<string, Geometry> geometryLibrary;
		map<string, Source> sources;
		map<string, Node*> nodeLibrary;
		map<unsigned int, unsigned int> tempMap;
		map<string, FireCube::MaterialPtr> generatedMaterials;
		Node *root;
		float unit;
		UpDirection upDirection;
		ModelLoadingOptions options;
	};
}

#pragma warning(pop)
#endif