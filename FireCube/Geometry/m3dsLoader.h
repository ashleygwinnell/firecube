#ifndef M3DS_LOADER_H
#define M3DS_LOADER_H

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
			MaterialPtr material;
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
		bool Load(const string &filename, ModelLoadingOptions options);
		NodePtr GenerateSceneGraph();
	private:
		void ReadMainChunk();
		void ReadEdit3dsChunk(unsigned int length);
		void ReadObjectChunk(unsigned int length);
		void ReadTriMeshChunk(unsigned int length);
		void ReadVerticesListChunk();
		void ReadFacesListChunk(unsigned int length);
		void ReadMaterialFaceList();
		void ReadTexCoordListChunk();
		void ReadObjectMatrixChunk();
		void ReadMaterialListChunk(unsigned int length);
		void ReadMaterialNameChunk();
		vec3 ReadMaterialColorChunk(unsigned int length);
		TexturePtr ReadMaterialTexMapChunk(unsigned int length);
		float ReadMaterialShininessChunk(unsigned int length);
		string ReadMapNameChunk();
		vec3 ReadColorFChunk();
		vec3 ReadColorBChunk();
		float ReadPercentageBChunk();
		float ReadPercentageFChunk();
		MaterialPtr GetMaterialByName(const string &name);
		vector<char> buffer;
		MaterialPtr curMaterial;
		vector<MaterialPtr> materials;
		vector<pair<pair<unsigned int, unsigned int>, string>> meshMaterial;
		vector<pair<unsigned int, mat4>> objectMatrix;
		vector<Object> object;
		char *curPos;
		ModelLoadingOptions options;
	};
}

#pragma warning(pop)
#endif