#include "MathUtils.h"
#include "Physics/CollisionShape.h"
#include "Physics/PhysicsWorld.h"

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

bool MathUtils::IntersectRayTriangle(const vec3 &origin, const vec3 &direction, const vec3 &vert0, const vec3 &vert1, const vec3 &vert2, float &u, float &v, float &t)
{
	vec3 edge1;
	vec3 edge2;
	vec3 tvec;
	vec3 pvec;
	vec3 qvec;
	float inv_det;
	float det;

	edge1 = vert1 - vert0;
	edge2 = vert2 - vert0;


	pvec.Cross(direction, edge2);


	det = edge1.Dot(pvec);

	if ((det > -0.001f) && (det < 0.001f))
	{
		return false;
	}
	inv_det = 1 / det;

	tvec = origin - vert0;

	u = tvec.Dot(pvec) * inv_det;
	if ((u < 0) || (u > 1))
	{
		return false;

	}
	qvec.Cross(tvec, edge1);

	v = direction.Dot(qvec) * inv_det;
	if ((v < 0) || (u + v > 1))
	{
		return false;
	}

	t = edge2.Dot(qvec) * inv_det;

	return true;
}

vec3 MathUtils::GetTrianglePoint(const vec3 &p0, const vec3 &p1, const vec3 &p2, float u, float v)
{
	// In case the triangle is far away from the origin but is small in size, the elements of 'a' will have large magnitudes,
	// and the elements of (b-a) and (c-a) will be much smaller quantities. Therefore be extra careful with the
	// parentheses and first sum the small floats together before adding it to the large one.
	return p0 + ((p1 - p0) * u + (p2 - p0) * v);
}

void MathUtils::ClosestPointLineLine(const vec3 &v0, const vec3 &v10, const vec3 &v2, const vec3 &v32, float &d, float &d2)
{
	vec3 v02 = v0 - v2;
	float d0232 = v02.Dot(v32);
	float d3210 = v32.Dot(v10);
	float d3232 = v32.Dot(v32);
	
	float d0210 = v02.Dot(v10);
	float d1010 = v10.Dot(v10);
	float denom = d1010*d3232 - d3210*d3210;
	if (denom != 0.f)
		d = (d0232*d3210 - d0210*d3232) / denom;
	else
		d = 0.f;
	d2 = (d0232 + d * d3210) / d3232;
}

vec3 MathUtils::ClosestPointLineSegment(const vec3 &point, const vec3 &s0, const vec3 &s1, float &d)
{
	vec3 dir = s1 - s0;
	d = std::min(std::max(Dot(point - s0, dir) / dir.Length2(), 0.0f), 1.0f);
	return s0 + d * dir;
}

vec3 MathUtils::ClosestPointLineSegmentLineSegment(const vec3 &s0, const vec3 &s1, const vec3 &l0, const vec3 &l1, float &d, float &d2)
{
	vec3 dir = s1 - s0;
	ClosestPointLineLine(s0, dir, l0, l1 - l0, d, d2);
	if (d >= 0.f && d <= 1.f && d2 >= 0.f && d2 <= 1.f)
		return s0 + d * dir;
	else if (d >= 0.f && d <= 1.f) // Only d2 is out of bounds.
	{
		vec3 p;
		if (d2 < 0.f)
		{
			d2 = 0.f;
			p = l0;
		}
		else
		{
			d2 = 1.f;
			p = l1;
		}
		return ClosestPointLineSegment(p, s0, s1, d);
	}
	else if (d2 >= 0.f && d2 <= 1.f) // Only d is out of bounds.
	{
		vec3 p;
		if (d < 0.f)
		{
			d = 0.f;
			p = s0;
		}
		else
		{
			d = 1.f;
			p = s1;
		}

		ClosestPointLineSegment(p, l0, l1, d2);		
		return p;
	}
	else // Both u and u2 are out of bounds.
	{
		vec3 p;
		if (d < 0.f)
		{
			p = s0;
			d = 0.f;
		}
		else
		{
			p = s1;
			d = 1.f;
		}

		vec3 p2;
		if (d2 < 0.f)
		{
			p2 = l0;
			d2 = 0.f;
		}
		else
		{
			p2 = l1;
			d2 = 1.f;
		}

		float T, T2;
		vec3 closestPoint = ClosestPointLineSegment(p2, s0, s1, T);
		vec3 closestPoint2 = ClosestPointLineSegment(p, l0, l1, T2);

		if ((closestPoint - p2).Length2() <= (closestPoint2 - p).Length2())
		{
			d = T;
			return closestPoint;
		}
		else
		{
			d2 = T2;
			return p;
		}
	}
}

vec3 GetPoint(const vec3 &s0, const vec3 &s1, float t)
{
	return (1.0f - t) * s0 + t * s1;
}

inline float TriArea2D(float x1, float y1, float x2, float y2, float x3, float y3)
{
	return (x1 - x2)*(y2 - y3) - (x2 - x3)*(y1 - y2);
}

vec3 BarycentricUVW(const vec3 &point, const vec3 &p0, const vec3 &p1, const vec3 &p2)
{
	// Implementation from Christer Ericson's Real-Time Collision Detection, pp. 51-52.

	// Unnormalized triangle normal.
	vec3 m = Cross(p1 - p0, p2 - p0);

	// Nominators and one-over-denominator for u and v ratios.
	float nu, nv, ood;

	// Absolute components for determining projection plane.
	float x = std::abs(m.x);
	float y = std::abs(m.y);
	float z = std::abs(m.z);

	if (x >= y && x >= z)
	{
		// Project to the yz plane.
		nu = TriArea2D(point.y, point.z, p1.y, p1.z, p2.y, p2.z); // Area of PBC in yz-plane.
		nv = TriArea2D(point.y, point.z, p2.y, p2.z, p0.y, p0.z); // Area OF PCA in yz-plane.
		ood = 1.f / m.x; // 1 / (2*area of ABC in yz plane)
	}
	else if (y >= z) // Note: The book has a redundant 'if (y >= x)' comparison
	{
		// y is largest, project to the xz-plane.
		nu = TriArea2D(point.x, point.z, p1.x, p1.z, p2.x, p2.z);
		nv = TriArea2D(point.x, point.z, p2.x, p2.z, p0.x, p0.z);
		ood = 1.f / -m.y;
	}
	else // z is largest, project to the xy-plane.
	{
		nu = TriArea2D(point.x, point.y, p1.x, p1.y, p2.x, p2.y);
		nv = TriArea2D(point.x, point.y, p2.x, p2.y, p0.x, p0.y);
		ood = 1.f / m.z;
	}
	float u = nu * ood;
	float v = nv * ood;
	float w = 1.f - u - v;
	return vec3(u, v, w);
}

vec2 BarycentricUV(const vec3 &point, const vec3 &p0, const vec3 &p1, const vec3 &p2)
{
	vec3 uvw = BarycentricUVW(point, p0, p1, p2);
	return vec2(uvw.y, uvw.z);
}

vec3 MathUtils::ClosestPointToTriangleEdge(const vec3 &s0, const vec3 &s1, const vec3 &p0, const vec3 &p1, const vec3 &p2, float &outU, float &outV, float &outD)
{
	///@todo Optimize!
	// The line is parallel to the triangle.
	float unused1, unused2, unused3;
	float d1, d2, d3;
			
	vec3 pt1 = ClosestPointLineSegmentLineSegment(p0, p1, s0, s1, unused1, d1);
	vec3 pt2 = ClosestPointLineSegmentLineSegment(p1, p2, s0, s1, unused2, d2);
	vec3 pt3 = ClosestPointLineSegmentLineSegment(p2, p0, s0, s1, unused3, d3);		
		
	float dist1 = (pt1 - GetPoint(s0, s1, d1)).Length2();
	float dist2 = (pt2 - GetPoint(s0, s1, d2)).Length2();
	float dist3 = (pt3 - GetPoint(s0, s1, d3)).Length2();
	if (dist1 <= dist2 && dist1 <= dist3)
	{
		outU = BarycentricUV(pt1, p0, p1, p2).x;
		outV = BarycentricUV(pt1, p0, p1, p2).y;
		outD = d1;
		return pt1;
	}
	else if (dist2 <= dist3)
	{
		outU = BarycentricUV(pt2, p0, p1, p2).x;
		outV = BarycentricUV(pt2, p0, p1, p2).y;
		outD = d2;
		return pt2;
	}
	else
	{
		outU = BarycentricUV(pt3, p0, p1, p2).x;
		outV = BarycentricUV(pt3, p0, p1, p2).y;
		outD = d3;
		return pt3;
	}
}

vec3 MathUtils::LineSegmentTriangleClosestPoint(const vec3 &s0, const vec3 &s1, const vec3 &p0, const vec3 &p1, const vec3 &p2, vec3 &pointOnSegment)
{
	float u, v;
	float t;
	bool intersects = IntersectRayTriangle(s0, s1 - s0, p0, p1, p2, u, v, t);	
	if (intersects && t >= 0.0f && t <= 1.0f)
	{		
		pointOnSegment = GetPoint(s0, s1, t);
		return GetTrianglePoint(p0, p1, p2, u, v);
	}

	float u1, v1, d1;
	vec3 pt1 = ClosestPointToTriangleEdge(s0, s1, p0, p1, p2, u1, v1, d1);

	vec3 pt2 = ClosestPointTriangle(s0, p0, p1, p2, u1, v1);
	vec3 pt3 = ClosestPointTriangle(s1, p0, p1, p2, u1, v1);

	float D1 = (pt1 - GetPoint(s0, s1, d1)).Length2();
	float D2 = (pt2 - s0).Length2();
	float D3 = (pt3 - s1).Length2();

	if (D1 <= D2 && D1 <= D3)
	{		
		pointOnSegment = GetPoint(s0, s1, d1);
		return pt1;
	}
	else if (D2 <= D3)
	{	
		pointOnSegment = s0;
		return pt2;
	}
	else
	{		
		pointOnSegment = s1;
		return pt3;
	}
}

vec3 MathUtils::ClosestPointTriangle(const vec3 &p, const vec3 &a, const vec3 &b, const vec3 &c, float& s, float& t)
{
	// Check if P in vertex region outside A
	const vec3 ab = b - a;
	const vec3 ac = c - a;
	const vec3 ap = p - a;
	const float d1 = ab.Dot(ap);
	const float d2 = ac.Dot(ap);
	if (d1 <= 0.0f && d2 <= 0.0f)
	{
		s = 0.0f;
		t = 0.0f;
		return a;	// Barycentric coords 1,0,0
	}

	// Check if P in vertex region outside B
	const vec3 bp = p - b;
	const float d3 = ab.Dot(bp);
	const float d4 = ac.Dot(bp);
	if (d3 >= 0.0f && d4 <= d3)
	{
		s = 1.0f;
		t = 0.0f;
		return b;	// Barycentric coords 0,1,0
	}

	// Check if P in edge region of AB, if so return projection of P onto AB
	const float vc = d1*d4 - d3*d2;
	if (vc <= 0.0f && d1 >= 0.0f && d3 <= 0.0f)
	{
		const float v = d1 / (d1 - d3);
		s = v;
		t = 0.0f;
		return a + v * ab;	// barycentric coords (1-v, v, 0)
	}

	// Check if P in vertex region outside C
	const vec3 cp = p - c;
	const float d5 = ab.Dot(cp);
	const float d6 = ac.Dot(cp);
	if (d6 >= 0.0f && d5 <= d6)
	{
		s = 0.0f;
		t = 1.0f;
		return c;	// Barycentric coords 0,0,1
	}

	// Check if P in edge region of AC, if so return projection of P onto AC
	const float vb = d5*d2 - d1*d6;
	if (vb <= 0.0f && d2 >= 0.0f && d6 <= 0.0f)
	{
		const float w = d2 / (d2 - d6);
		s = 0.0f;
		t = w;
		return a + w * ac;	// barycentric coords (1-w, 0, w)
	}

	// Check if P in edge region of BC, if so return projection of P onto BC
	const float va = d3*d6 - d5*d4;
	if (va <= 0.0f && (d4 - d3) >= 0.0f && (d5 - d6) >= 0.0f)
	{
		const float w = (d4 - d3) / ((d4 - d3) + (d5 - d6));
		s = 1.0f - w;
		t = w;
		return b + w * (c - b);	// barycentric coords (0, 1-w, w)
	}

	// P inside face region. Compute Q through its barycentric coords (u,v,w)
	const float denom = 1.0f / (va + vb + vc);
	const float v = vb * denom;
	const float w = vc * denom;
	s = v;
	t = w;
	return a + ab*v + ac*w;
}