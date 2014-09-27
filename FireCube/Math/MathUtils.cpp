#include "MathUtils.h"

using namespace FireCube;

unsigned int MathUtils::MurmurHash2 ( const void * key, int len, unsigned int seed )
{
	// 'm' and 'r' are mixing constants generated offline.
	// They're not really 'magic', they just happen to work well.

	const unsigned int m = 0x5bd1e995;
	const int r = 24;

	// Initialize the hash to a 'random' value

	unsigned int h = seed ^ len;

	// Mix 4 bytes at a time into the hash

	const unsigned char * data = (const unsigned char *)key;

	while(len >= 4)
	{
		unsigned int k = *(unsigned int *)data;

		k *= m;
		k ^= k >> r;
		k *= m;

		h *= m;
		h ^= k;

		data += 4;
		len -= 4;
	}

	// Handle the last few bytes of the input array

	switch(len)
	{
	case 3:
		h ^= data[2] << 16;
	case 2:
		h ^= data[1] << 8;
	case 1:
		h ^= data[0];
		h *= m;
	};

	// Do a few final mixes of the hash to ensure the last few
	// bytes are well-incorporated.

	h ^= h >> 13;
	h *= m;
	h ^= h >> 15;

	return h;
}

void MathUtils::CalculateNormals(std::vector<vec3> &normals, const std::vector<vec3> &vertices, const std::vector<unsigned int> &indices)
{
	normals.resize(vertices.size(), vec3(0, 0, 0));	

	for (unsigned int i = 0; i < indices.size(); i += 3)
	{
		// Calculate face normals
		vec3 v1 = vertices[indices[i + 1]] - vertices[indices[i + 0]];
		vec3 v2 = vertices[indices[i + 2]] - vertices[indices[i + 0]];
		vec3 n = Cross(v1, v2);				
		// Add this normal to the three vertex normals forming this face
		normals[indices[i + 0]] += n;
		normals[indices[i + 1]] += n;
		normals[indices[i + 2]] += n;
	}
	for (unsigned int n = 0; n < normals.size(); n++)
	{
		normals[n].Normalize();
	}	
}

void MathUtils::CalculateTangents(const std::vector<vec3> &vertices, const std::vector<vec3> &normals, const std::vector<vec2> &uv, const std::vector<unsigned int> &indices, std::vector<vec3> &tangents)
{
	if ((normals.size() == 0) || (uv.size() == 0))
		return;
	std::vector<vec3> tan(vertices.size());
	std::fill(tan.begin(), tan.end(), vec3(0, 0, 0));

	for (unsigned int i = 0; i < indices.size(); i += 3)
	{		
		const vec3 &v0 = vertices[indices[i + 0]];
		const vec3 &v1 = vertices[indices[i + 1]];
		const vec3 &v2 = vertices[indices[i + 2]];

		const vec2 &uv0 = uv[indices[i + 0]];
		const vec2 &uv1 = uv[indices[i + 1]];
		const vec2 &uv2 = uv[indices[i + 2]];

		float x1 = v1.x - v0.x;
		float x2 = v2.x - v0.x;
		float y1 = v1.y - v0.y;
		float y2 = v2.y - v0.y;
		float z1 = v1.z - v0.z;
		float z2 = v2.z - v0.z;

		float s1 = uv1.x - uv0.x;
		float s2 = uv2.x - uv0.x;
		float t1 = uv1.y - uv0.y;
		float t2 = uv2.y - uv0.y;

		float r = 1.0f / (s1 * t2 - s2 * t1);
		vec3 sdir((t2 * x1 - t1 * x2) * r, (t2 * y1 - t1 * y2) * r,
			(t2 * z1 - t1 * z2) * r);		

		tan[indices[i + 0]] += sdir;
		tan[indices[i + 1]] += sdir;
		tan[indices[i + 2]] += sdir;		
	}
	tangents.resize(vertices.size());	
	for (unsigned int i = 0; i < vertices.size(); i++)
	{
		vec3 n = normals[i];
		vec3 t1 = tan[i];
		tangents[i] = (t1 - n * Dot(n, t1)).Normalized();		
	}	
}

bool MathUtils::PointTri(vec3 aa, vec3 bb, vec3 cc, vec3 p)
{
	vec3 a, b, c, an, bn, cn;
	vec3 tnorm = Cross(bb - aa, cc - aa).Normalized();
	float d1, d2, d3, dot1, dot2, dot3;
	a = bb - aa;
	b = cc - bb;
	c = aa - cc;

	an.Cross(a, tnorm);
	bn.Cross(b, tnorm);
	cn.Cross(c, tnorm);

	d1 = aa.Dot(an);
	d2 = bb.Dot(bn);
	d3 = cc.Dot(cn);

	dot1 = p.Dot(an) - d1;
	dot2 = p.Dot(bn) - d2;
	dot3 = p.Dot(cn) - d3;


	if (((dot1 <= 0) && (dot2 <= 0) && (dot3 <= 0)) || ((dot1 >= 0) && (dot2 >= 0) && (dot3 >= 0))) return true;
	return false;
}


bool MathUtils::GetLowestRoot(float a, float b, float c, float maxR, float &root)
{
	float determinant = b*b - 4.0f*a*c;

	if (determinant < 0.0f)
		return false;

	float sqrtD = (float)sqrt(determinant);
	float r1 = (-b - sqrtD) / (2 * a);
	float r2 = (-b + sqrtD) / (2 * a);

	if (r1 > r2)
	{
		float temp = r2;
		r2 = r1;
		r1 = temp;
	}
	if ((r1 > 0) && (r1 < maxR))
	{
		root = r1;
		return true;
	}

	if ((r2>0) && (r2 < maxR))
	{
		root = r2;
		return true;
	}
	return false;

}

/*
void MathUtils::CalculateTangents(const std::vector<vec3> &vertices, const std::vector<vec3> &normals, const std::vector<vec2> &uv, const std::vector<unsigned int> &indices, std::vector<vec3> &tangents, std::vector<vec3> &bitangents)
{
	if ((normals.size() == 0) || (uv.size() == 0))
		return;
	std::vector<vec3> tan(vertices.size() * 2);
	std::fill(tan.begin(), tan.end(), vec3(0, 0, 0));

	for (unsigned int i = 0; i < indices.size(); i += 3)
	{		
		const vec3 &v0 = vertices[indices[i + 0]];
		const vec3 &v1 = vertices[indices[i + 1]];
		const vec3 &v2 = vertices[indices[i + 2]];

		const vec2 &uv0 = uv[indices[i + 0]];
		const vec2 &uv1 = uv[indices[i + 1]];
		const vec2 &uv2 = uv[indices[i + 2]];

		float x1 = v1.x - v0.x;
		float x2 = v2.x - v0.x;
		float y1 = v1.y - v0.y;
		float y2 = v2.y - v0.y;
		float z1 = v1.z - v0.z;
		float z2 = v2.z - v0.z;

		float s1 = uv1.x - uv0.x;
		float s2 = uv2.x - uv0.x;
		float t1 = uv1.y - uv0.y;
		float t2 = uv2.y - uv0.y;

		float r = 1.0f / (s1 * t2 - s2 * t1);
		vec3 sdir((t2 * x1 - t1 * x2) * r, (t2 * y1 - t1 * y2) * r,
			(t2 * z1 - t1 * z2) * r);
		vec3 tdir((s1 * x2 - s2 * x1) * r, (s1 * y2 - s2 * y1) * r,
			(s1 * z2 - s2 * z1) * r);

		tan[indices[i + 0]] += sdir;
		tan[indices[i + 1]] += sdir;
		tan[indices[i + 2]] += sdir;

		tan[vertices.size() + indices[i + 0]] += tdir;
		tan[vertices.size() + indices[i + 1]] += tdir;
		tan[vertices.size() + indices[i + 2]] += tdir;
	}
	tangents.resize(vertices.size());
	bitangents.resize(vertices.size());
	for (unsigned int i = 0; i < vertices.size(); i++)
	{
		vec3 n = normals[i];
		vec3 t1 = tan[i];
		vec3 t2 = tan[vertices.size() + i];

		tangents[i] = (t1 - n * Dot(n, t1)).Normalized();
		float w = (Dot(Cross(n, t1), t2) < 0.0f) ? -1.0f : 1.0f;
		bitangents[i] = (Cross(n, tangents[i]) * w).Normalized();
	}	
}
*/