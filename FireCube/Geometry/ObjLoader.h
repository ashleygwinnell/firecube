#pragma once

#pragma warning(push)
#pragma warning(disable:4251)

#include <vector>
#include <string>
#include <map>

#include "Math/MyMath.h"
#include "Scene/Node.h"
#include "ModelLoader.h"

namespace FireCube
{
	class ObjLoader : public ModelLoader
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
			std::map<std::string, std::vector<Face>> materialFaces;
			std::vector<Face> facesWithoutMaterial;
		};
		class Material
		{
		public:
			Material();
			vec3 ambientColor;
			vec3 diffuseColor;
			vec3 specularColor;
			float shininess;
			std::string diffuseTextureName;
			std::string normalTextureName;
		};
		class MapKey
		{
		public:
			MapKey();
			MapKey(unsigned int vv, unsigned int vt, unsigned int vn);
			unsigned int v, t, n;
			bool operator < (const MapKey &other) const;
		};
		ObjLoader(Engine *engine);
		virtual bool Load(const std::string &filename, ModelLoadingOptions options = ModelLoadingOptions());		
		virtual void GenerateGeometries(Renderer *renderer);
		virtual void GenerateScene(Renderer *renderer);
		virtual const std::vector<Geometry *> &GetGeneratedGeometries();
		virtual NodePtr GetGeneratedScene();
		virtual const std::vector<FireCube::Material *> &GetGeneratedMaterials();
		virtual BoundingBox GetBoundingBox() const;

	private:
		void CalculateNormals(std::vector<vec3> &normals, std::vector<vec3> &vertices, std::vector<unsigned int> &indices);
		std::string ExtractDirectory(const std::string &filename);
		void ParseVertexLine(const std::string &line);
		void ParseTexCoordLine(const std::string &line);
		void ParseNormalLine(const std::string &line);
		void ParseFaceLine(const std::string &line);
		void ParseObjectLine(const std::string &line);
		void ParseUseMtlLine(const std::string &line);
		void ParseMtlLibLine(const std::string &line);	
		void ParseFaceEntry(const std::string &entry, unsigned int &v, unsigned int &t, unsigned int &n, bool &hasTextureCoordinates, bool &hasNormal);

		void ParseMaterialFile(const std::string &filename);
		void ParseNewMtlLine(const std::string &line);
		void ParseAmbientColorLine(const std::string &line);
		void ParseDiffuseColorLine(const std::string &line);
		void ParseSpecularColorLine(const std::string &line);
		void ParseDiffuseTextureMap(const std::string &line);
		void ParseNormalTextureMap(const std::string &line);
		void ParseShininessLine(const std::string &line);
		std::vector<vec3> vertices;
		std::vector<vec2> texCoords;
		std::vector<vec3> normals;
		std::map<std::string, Object> objects;
		std::vector<Face> *currentFaces;
		Object *currentObject;
		std::string lastMaterial;
		std::map<std::string, Material> materials;
		Material *currentMaterial;
		std::string baseDir;
		ModelLoadingOptions options;
		std::vector<Geometry *> generatedGeometries;
		std::vector<FireCube::Material *> generatedMaterials;
		BoundingBox boundingBox;
	};
}

#pragma warning(pop)
