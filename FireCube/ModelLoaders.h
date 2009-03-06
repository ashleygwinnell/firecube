#ifndef MODELLOADERS_H
#define MODELLOADERS_H

#pragma warning(push)
#pragma warning(disable:4251)

#define PRIMARY 0x4d4d
#define EDIT3DS 0x3d3d
#define KEYF3DS 0xb000

#define NAMED_OBJECT 0x4000
#define OBJ_MESH 0x4100
#define MESH_VERTICES 0x4110
#define MESH_FACES 0x4120
#define MESH_MATERIAL 0x4130
#define MESH_TEX_VERT 0x4140

#define MATERIAL 0xafff
#define MAT_NAME 0xa000
#define MAT_AMBIENT 0xa010
#define MAT_DIFFUSE 0xa020
#define MAT_SPECULAR 0xa030
#define MAT_SHININESS 0xa040
#define MAT_TEXMAP 0xa200
#define MAT_TEXFLNM 0xa300
#define MAT_SHIN2PCT 0xa041
#define MAT_SHIN3PCT 0xa042

class FIRECUBE_API M3dsLoader
{
public:
	M3dsLoader(ModelResource *model);
	bool Load(const string &filename);
private:
	DWORD ProcessChunk(char *buffer);
	ModelResource *model;
};

#pragma warning(pop)
#endif