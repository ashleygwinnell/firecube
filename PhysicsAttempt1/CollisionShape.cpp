#include <string>
#include <vector>
#include <sstream>
#include <iostream>
#include <map>
#include <fstream>
#include <cmath>
#include <boost/shared_array.hpp>
#include <boost/weak_ptr.hpp>
#include <windows.h>
using namespace std;
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <gl/gl.h>
#include <FireCube.h>
using namespace FireCube;
#include "app.h"
#include "Grid3D.h"
#include "CollisionShape.h"
#include "utils.h"

CollisionShape::CollisionShape() : radius(0), interpolateMesh(true)
{

}
void CollisionShape::FromModel(Model model,int sizex,int sizey,int sizez,float extraSize)
{
	vertex=model->object[0].vertex;
	face=model->object[0].face;
	edge.resize(face.size()*3);
	for (unsigned int i=0;i<face.size();i++)
	{
		edge[i*3+0].v[0]=face[i].v[0];
		edge[i*3+0].v[1]=face[i].v[1];
		edge[i*3+1].v[0]=face[i].v[0];
		edge[i*3+1].v[1]=face[i].v[2];
		edge[i*3+2].v[0]=face[i].v[1];
		edge[i*3+2].v[1]=face[i].v[2];
	}
	min.Set(1000000,1000000,1000000);
	max=-min;
	this->sizex=sizex;
	this->sizey=sizey;
	this->sizez=sizez;
	int num=vertex.size();
	for (int i=0;i<num;i++)
	{
		min.x = vertex[i].x < min.x ? vertex[i].x : min.x;
		max.x = vertex[i].x > max.x ? vertex[i].x : max.x;
		min.y = vertex[i].y < min.y ? vertex[i].y : min.y;
		max.y = vertex[i].y > max.y ? vertex[i].y : max.y;
		min.z = vertex[i].z < min.z ? vertex[i].z : min.z;
		max.z = vertex[i].z > max.z ? vertex[i].z : max.z;
	}
	vec3 mid =0.5f * (min + max);	

	min.x = mid.x + extraSize * (min.x - mid.x);
	min.y = mid.y + extraSize * (min.y - mid.y);
	min.z = mid.z + extraSize * (min.z - mid.z);
	max.x = mid.x + extraSize * (max.x - mid.x);
	max.y = mid.y + extraSize * (max.y - mid.y);
	max.z = mid.z + extraSize * (max.z - mid.z);

	dv.x=(max.x-min.x) / (sizex-1);
	dv.y=(max.y-min.y) / (sizey-1);
	dv.z=(max.z-min.z) / (sizez-1);
	
	vec3 corners[8];
	corners[0] = vec3(max.x, max.y, max.z);
	corners[1] = vec3(max.x, max.y, min.z);
	corners[2] = vec3(max.x, min.y, max.z);
	corners[3] = vec3(max.x, min.y, min.z);
	corners[4] = vec3(min.x, max.y, max.z);
	corners[5] = vec3(min.x, max.y, min.z);
	corners[6] = vec3(min.x, min.y, max.z);
	corners[7] = vec3(min.x, min.y, min.z);
	radius = corners[0].Length();
	for (int i = 1 ; i < 8 ; ++i)
	{
		if (corners[i].Length() > radius)
			radius = corners[i].Length();
	}

	volume.resize(sizex,sizey,sizez);
	const int numOutsideDirs=10;
	vec3 outsideDirs[numOutsideDirs];
	for (int i=0;i<numOutsideDirs;i++)
	{
		outsideDirs[i]=vec3(RangedRandom(-1.0f,1.0f),RangedRandom(-1.0f,1.0f),RangedRandom(-1.0f,1.0f));
		outsideDirs[i].Normalize();
		outsideDirs[i]*=radius*2;
	}
	for (int x=0;x<sizex;x++)
	{
		for (int y=0;y<sizey;y++)
		{
			for (int z=0;z<sizez;z++)
			{
				PointInfo &pointInfo=volume(x,y,z);
				vec3 pos=vec3(min.x + dv.x*x,min.y + dv.y*y,min.z+dv.z*z);
				vec3 pointOnTriangle;
				int triangleIndex;
				FindClosestTriangle(pos,vertex,face,triangleIndex,pointOnTriangle);
				pointInfo.dir=pointOnTriangle-pos;
				pointInfo.dist=pointInfo.dir.Length();
				if (pointInfo.dist<0.0001f)
				{
					pointInfo.dist=0.0001f;
					pointInfo.dir=Cross(vertex[face[triangleIndex].v[1]]-vertex[face[triangleIndex].v[0]],vertex[face[triangleIndex].v[2]]-vertex[face[triangleIndex].v[0]]);
					pointInfo.dir.Normalize();
				}
				else
				{
					pointInfo.dir/=pointInfo.dist;

					int l;
					int numIn = 0;
					int numOut = 0;
					for (l = 0 ; l < numOutsideDirs ; l++)
					{
						
						int hits = CountTriangleHits(pos,outsideDirs[l],vertex,face);
						if (hits % 2)
							++numIn;
						else
							++numOut;
					}
					if (numOut > numIn)
					{
						// outside
						pointInfo.dist = -pointInfo.dist;
						pointInfo.dir = -pointInfo.dir;
					}
				}
			}
		}
	}
}

template<class T>
T Interpolate3d(const T & val000,
		   const T & val001,
		   const T & val010,
		   const T & val011,
		   const T & val100,
		   const T & val101,
		   const T & val110,
		   const T & val111,
		   float x, float y, float z)
{
	T vxy0 = (1.0f - y) * (val000 * (1.0f - x) + val100 * x) + 
		y * (val010 * (1.0f - x) + val110 * x);
	T vxy1 = (1.0f - y) * (val001 * (1.0f - x) + val101 * x) + 
		y * (val011 * (1.0f - x) + val111 * x);

	return (1.0f - z) * vxy0 + z * vxy1;
}


bool CollisionShape::GetPointInfo(vec3 pos, vec3 &dir, float &dist, bool accurateOutsideDist)
{
	if (!accurateOutsideDist)
	{
		if ( (pos.x < min.x) || (pos.x > max.x) ||
			(pos.y < min.y) || (pos.y > max.y) ||
			(pos.z < min.z) || (pos.z > max.z) )
		{
			return false;
		}
	}
	float fi = (pos.x - min.x) / dv.x;
	float fj = (pos.y - min.y) / dv.y;
	float fk = (pos.z - min.z) / dv.z;

	if (interpolateMesh)
	{
		// find the indices of the box surrounding the point
		int ii0 = (int) floor(fi);
		int ij0 = (int) floor(fj);
		int ik0 = (int) floor(fk);
		float x = fi - ii0;
		float y = fj - ij0;
		float z = fk - ik0;
		int ii1 = ii0 + 1;
		int ij1 = ij0 + 1;
		int ik1 = ik0 + 1;
		if (ii0 < 0) ii0 = 0; else if (ii0 >= sizex) ii0 = sizex - 1;
		if (ij0 < 0) ij0 = 0; else if (ij0 >= sizey) ij0 = sizey - 1;
		if (ik0 < 0) ik0 = 0; else if (ik0 >= sizez) ik0 = sizez - 1;
		if (ii1 < 0) ii1 = 0; else if (ii1 >= sizex) ii1 = sizex - 1;
		if (ij1 < 0) ij1 = 0; else if (ij1 >= sizey) ij1 = sizey - 1;
		if (ik1 < 0) ik1 = 0; else if (ik1 >= sizez) ik1 = sizez - 1;

		const PointInfo & datum000 = volume(ii0, ij0, ik0);
		const PointInfo & datum001 = volume(ii0, ij0, ik1);
		const PointInfo & datum010 = volume(ii0, ij1, ik0);
		const PointInfo & datum011 = volume(ii0, ij1, ik1);
		const PointInfo & datum100 = volume(ii1, ij0, ik0);
		const PointInfo & datum101 = volume(ii1, ij0, ik1);
		const PointInfo & datum110 = volume(ii1, ij1, ik0);
		const PointInfo & datum111 = volume(ii1, ij1, ik1);

		dir = Interpolate3d<vec3>(datum000.dir, 
			datum001.dir,
			datum010.dir,
			datum011.dir,
			datum100.dir,
			datum101.dir,
			datum110.dir,
			datum111.dir,
			x, y, z);
		dir.Normalize();
		dist = Interpolate3d<float>(datum000.dist, 
			datum001.dist,
			datum010.dist,
			datum011.dist,
			datum100.dist,
			datum101.dist,
			datum110.dist,
			datum111.dist,
			x, y, z);
		/*float extra_dist = Dot(					// me added
			dir,
			pos - vec3(min.x + ii0 * dv.x, 
			min.y + ij0 * dv.y,
			min.z + ik0 * dv.z) );
		dist -= extra_dist;*/

		if (dist > 0)
		{
			return true;
		}
		else
		{
			dist = -dist;			
			/*
			if ((accurate_outside_dist) ||
			(ii == 0) || (ii == (m_nx - 1)) ||
			(ij == 0) || (ij == (m_ny - 1)) ||
			(ik == 0) || (ik == (m_nz - 1)))
			{
			// query point was probably outside - add on the extra distance.
			dist += (pos - Position(m_min_x + ii * m_dx, m_min_y + ij * m_dy, m_min_z + ik * m_dz)).mag();
			}
			*/
			return false;
		}
	}	
	else
	{
		int ii = (int) floor(fi + 0.5f);
		int ij = (int) floor(fj + 0.5f);
		int ik = (int) floor(fk + 0.5f);

		if (ii < 0) ii = 0; else if (ii >= sizex) ii = sizex - 1;
		if (ij < 0) ij = 0; else if (ij >= sizey) ij = sizey - 1;
		if (ik < 0) ik = 0; else if (ik >= sizez) ik = sizez - 1;

		const PointInfo & datum = volume(ii, ij, ik);
		dir = datum.dir;
		dist = datum.dist;
		// need to account for the difference between the actual point and
		// the datum point.
		float extra_dist = Dot(
			dir,
			pos - vec3(min.x + ii * dv.x, 
			min.y + ij * dv.y,
			min.z + ik * dv.z) );
		dist -= extra_dist;

		if (dist > 0)
		{
			return true;
		}
		else
		{
			dist = -dist;
			if ((accurateOutsideDist) ||
				(ii == 0) || (ii == (sizex - 1)) ||
				(ij == 0) || (ij == (sizey - 1)) ||
				(ik == 0) || (ik == (sizez - 1)))
			{
				// query point was probably outside - add on the extra distance.
				dist += (pos - vec3(min.x + ii * dv.x, min.y + ij * dv.y, min.z + ik * dv.z)).Length();
			}
			return false;
		}
	}
}