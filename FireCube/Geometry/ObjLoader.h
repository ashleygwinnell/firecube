#ifndef OBJ_LOADER_H
#define OBJ_LOADER_H

#pragma warning(push)
#pragma warning(disable:4251)

namespace FireCube
{
	class ObjLoader
	{
	public:
		class Face
		{
		public:
			unsigned int v[3];
			unsigned int t[3];
			unsigned int n[3];
			bool hasTextureCoordinates;
			bool hasNormal;
		};
		class Object
		{
		public:
			map<string, vector<Face>> materialFaces;
			vector<Face> facesWithoutMaterial;
		};
		class Material
		{
		public:
			Material();
			vec3 ambientColor;
			vec3 diffuseColor;
			vec3 specularColor;
			float shininess;
			string diffuseTextureName;
			string normalTextureName;
		};
		class MapKey
		{
		public:
			MapKey();
			MapKey(unsigned int vv, unsigned int vt, unsigned int vn);
			unsigned int v, t, n;
			bool operator < (const MapKey &other) const;
		};
		ObjLoader();
		void Load(const string &filename, ModelLoadingOptions options);
		NodePtr GenerateSceneGraph();
	private:
		string ExtractDirectory(const string &filename);
		void ParseVertexLine(const string &line);
		void ParseTexCoordLine(const string &line);
		void ParseNormalLine(const string &line);
		void ParseFaceLine(const string &line);
		void ParseObjectLine(const string &line);
		void ParseUseMtlLine(const string &line);
		void ParseMtlLibLine(const string &line);	
		void ParseFaceEntry(const string &entry, unsigned int &v, unsigned int &t, unsigned int &n, bool &hasTextureCoordinates, bool &hasNormal);

		void ParseMaterialFile(const string &filename);
		void ParseNewMtlLine(const string &line);
		void ParseAmbientColorLine(const string &line);
		void ParseDiffuseColorLine(const string &line);
		void ParseSpecularColorLine(const string &line);
		void ParseDiffuseTextureMap(const string &line);
		void ParseNormalTextureMap(const string &line);
		void ParseShininessLine(const string &line);
		vector<vec3> vertices;
		vector<vec2> texCoords;
		vector<vec3> normals;
		map<string, Object> objects;
		vector<Face> *currentFaces;
		Object *currentObject;
		string lastMaterial;
		map<string, Material> materials;
		Material *currentMaterial;
		string baseDir;
		ModelLoadingOptions options;
	};
}

#pragma warning(pop)
#endif