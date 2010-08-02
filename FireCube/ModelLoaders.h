#ifndef MODELLOADERS_H
#define MODELLOADERS_H

#pragma warning(push)
#pragma warning(disable:4251)

#define MAIN3DS       0x4D4D

//>------ Main Chunks

#define EDIT3DS       0x3D3D  // this is the start of the editor config

//>------ sub defines of EDIT3DS

#define EDIT_MATERIAL 0xAFFF
#define EDIT_OBJECT   0x4000

//>------ sub defines of EDIT_OBJECT
#define OBJ_TRIMESH   0x4100
#define OBJ_LIGHT     0x4600
#define OBJ_CAMERA    0x4700

//>------ sub defines of OBJ_CAMERA
#define CAM_UNKNWN01  0x4710 
#define CAM_UNKNWN02  0x4720 

//>------ sub defines of OBJ_LIGHT
#define LIT_OFF       0x4620
#define LIT_SPOT      0x4610
#define LIT_UNKNWN01  0x465A

//>------ sub defines of OBJ_TRIMESH
#define TRI_VERTEXLIST   0x4110
#define TRI_FACELIST    0x4120
#define TRI_TEXCOORDLIST 0x4140
#define TRI_SMOOTH    0x4150
#define TRI_MATRIX     0x4160
#define TRI_VISIBLE   0x4165
#define TRI_MATERIAL  0x4130

#define MAT_NAME 0xA000
#define MAT_AMBIENT 0xA010
#define MAT_DIFFUSE 0xA020
#define MAT_SPECULAR 0xA030
#define MAT_TEXMAP 0xA200
#define MAT_MAPNAME 0xA300
#define MAT_SHININESS 0xA040

//>>------  these define the different color chunk types
#define COL_RGB_F  0x0010
#define COL_RGB_B  0x0011
#define PERCENTAGE_B 0x0030	
#define PERCENTAGE_F 0x0031
namespace FireCube
{
class M3dsLoader
{
public:
	class Mesh
	{
	public:
		Material material;
		vector<Face> face;
	};
	class Object
	{
	public:
		string name;
		vector<vec3> vertex;
		vector<Face> face;
		vector<vec2> uv;
		vector<Mesh> mesh;
	};

	M3dsLoader();
	bool Load(const string &filename);
	Node GenerateSceneGraph();
private:
	void ReadMainChunk();
	void ReadEdit3dsChunk(DWORD length);
	void ReadObjectChunk(DWORD length);
	void ReadTriMeshChunk(DWORD length);
	void ReadVerticesListChunk();
	void ReadFacesListChunk(DWORD length);
	void ReadMaterialFaceList();
	void ReadTexCoordListChunk();
	void ReadObjectMatrixChunk();
	void ReadMaterialListChunk(DWORD length);
	void ReadMaterialNameChunk();
	void ReadMaterialColorChunk(DWORD length,vec4 &color);
	void ReadMaterialTexMapChunk(DWORD length,Texture &texture);
	void ReadMaterialShininessChunk(DWORD length,float &shininess);
	string ReadMapNameChunk();
	vec4 ReadColorFChunk();
	vec4 ReadColorBChunk();
	float ReadPercentageBChunk();
	float ReadPercentageFChunk();
	Material GetMaterialByName(const string &name);
	vector<char> buffer;
	Material curMaterial;
	vector<Material> materials;	
	vector<pair<pair<DWORD,DWORD>,string>> meshMaterial;
	vector<pair<DWORD,mat4>> objectMatrix;
	vector<Object> object;
	char *curPos;
};

class ColladaLoader
{
public:
	enum UpDirection
	{
		X_UP,Y_UP,Z_UP
	};
	enum ParamType
	{
		PARAM_SAMPLER_2D,PARAM_SURFACE
	};
	enum ShadingType
	{
		SHADING_PHONG,SHADING_CONSTANT,SHADING_LAMBERT,SHADING_BLINN
	};
	enum InputType
	{
		INPUT_POSITION,INPUT_VERTEX,INPUT_NORMAL,INPUT_TEXCOORD,INPUT_COLOR,INPUT_UNKNOWN
	};
	enum PrimitiveType
	{
		PRIMITIVE_TRIANGLES,PRIMITIVE_POLYLIST
	};
	enum TransformationType
	{
		TRANSFORM_LOOKAT,TRANSFORM_ROTATE,TRANSFORM_TRANSLATE,TRANSFORM_MATRIX,TRANSFORM_SCALE
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
		int count,offset,stride;
		vector<string> params;
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
	class SemanticMapping
	{
	public:
		string materialName;
		map <string,InputSemanticEntry> inputMap;
	};
	class GeometryInstance
	{
	public:
		string url;
		map<string,SemanticMapping> materials;
	};
	class Node
	{
	public:
		string name,id,sid;
		vector<Node*> children;
		vector<NodeInstance> nodeInstances;
		vector<GeometryInstance> geometryInstances;
		vector<Transform> transformations;
		Node *parent;		
	};
	class Geometry
	{
	public:
		Mesh mesh;
	};
	class Effect
	{
	public:
		Effect() : ambientColor(1.0f,1.0f,1.0f,1.0f), diffuseColor(1.0f,1.0f,1.0f,1.0f), specularColor(0.0f,0.0f,0.0f,0.0f),shininess(128.0f)
		{			
		}
		ShadingType shadingType;
		vec4 ambientColor,diffuseColor,specularColor;
		Sampler ambientSampler,diffuseSampler,specularSampler;
		float shininess;
		map<string,EffectParam> effectParams;
	};
	ColladaLoader(const string &filename);
	~ColladaLoader();
	bool Load();
	TiXmlElement *GetChildElement(TiXmlNode *node,const string &elmName);
	void ReadLibraries(TiXmlNode *parent);
	void ReadAsset(TiXmlNode *parent);
	void ReadImageLibrary(TiXmlNode *parent);
	void ReadImage(TiXmlNode *parent,Image &img);
	void ReadMaterialLibrary(TiXmlNode *parent);
	void ReadMaterial(TiXmlNode *parent,Material &mat);
	void ReadEffectLibrary(TiXmlNode *parent);
	void ReadEffect(TiXmlNode *parent,Effect &effect);
	void ReadEffectProfileCommon(TiXmlNode *parent,Effect &effect);
	void ReadEffectParam(TiXmlNode *parent,EffectParam &effectParam);
	void ReadEffectColor(TiXmlNode *parent,vec4 &color,Sampler &sampler);
	void ReadEffectFloat(TiXmlNode *parent,float &value);
	void ReadGeometryLibrary(TiXmlNode *parent);
	void ReadGeometry(TiXmlNode *parent,Geometry &geometry);
	void ReadMesh(TiXmlNode *parent,Mesh &mesh);
	void ReadSource(TiXmlNode *parent,Source &source);
	void ReadDataArray(TiXmlNode *parent,DataArray &dataArray);
	void ReadAccessor(TiXmlNode *parent,Accessor &accessor);
	void ReadVertexData(TiXmlNode *parent,Mesh &mesh);
	void ReadInputChannel(TiXmlNode *parent,vector<InputChannel> &inputChannels);
	void ReadTriangles(TiXmlNode *parent, Mesh &mesh);
	void ReadPolylist(TiXmlNode *parent, Mesh &mesh);
	void ReadVCount(TiXmlNode *parent,SubMesh &subMesh);
	void ReadPrimitives(TiXmlNode *parent,Mesh &mesh,SubMesh &subMesh,vector<InputChannel> primInputChannels,int count);
	void GetDataFromChannel(InputChannel &ic,int index,Mesh &mesh);
	void ReadSceneLibrary(TiXmlNode *parent);
	void ReadSceneNode(TiXmlNode *parent,Node *node);
	void ReadNodeGeometry(TiXmlNode *parent,GeometryInstance &gi,Node *node);
	void ReadMaterialVertexInputBinding(TiXmlNode *parent,SemanticMapping &sm);
	void ReadScene(TiXmlNode *parent);
	void ReadTransformation(TiXmlNode *parent,Node *node);
	mat4 CalculateTranformation(vector<Transform> &transformations);
	vec3 GetTranslation(vector<Transform> &transformations);
	vec3 GetRotation(vector<Transform> &transformations);
	vec3 GetScale(vector<Transform> &transformations);
	mat4 GetTransformMatrix(vector<Transform> &transformations);
	void DeleteNodes(Node *node);
	void ApplySemanticMapping(Sampler &sampler,SemanticMapping &table);
	FireCube::Node GenerateSceneGraph(Node *node);
	FireCube::Node GenerateSceneGraph();
	InputType SemanticToInputType(const string &semantic);
	template <typename Type> 
	Type& ResolveLibraryReference( std::map<std::string, Type>& pLibrary, const std::string& pURL)
	{
		typename std::map<std::string, Type>::iterator it = pLibrary.find( pURL);		
		return it->second;
	}
	string GetTextureFileNameFromSampler(Effect &effect,Sampler &sampler);
	string FixFileName(string &filename);
private:
	TiXmlDocument xmlDocument;

	map<string,Image> imageLibrary;
	map<string,Material> materialLibrary;
	map<string,Effect> effectLibrary;
	map<string,Geometry> geometryLibrary;
	map<string,Source> sources;
	map<string,Node*> nodeLibrary;
	map<unsigned int,unsigned int> tempMap;
	Node *root;
	float unit;
	UpDirection upDirection;
};
}
#pragma warning(pop)
#endif