#include <FireCube.h>
using namespace std;
using namespace FireCube;
#include "Frustum.h"

float Plane::GetDist(vec3 p)
{
	return normal.Dot(p)+d;
}

void Frustum::ExtractFrustum()
{
	float proj[16];
	float modl[16];
	float clip[16];
	float t;

	/* Get the current PROJECTION matrix from OpenGL */
	glGetFloatv( GL_PROJECTION_MATRIX, proj );

	/* Get the current MODELVIEW matrix from OpenGL */
	glGetFloatv( GL_MODELVIEW_MATRIX, modl );

	/* Combine the two matrices (multiply projection by modelview) */
	clip[ 0] = modl[ 0] * proj[ 0] + modl[ 1] * proj[ 4] + modl[ 2] * proj[ 8] + modl[ 3] * proj[12];
	clip[ 1] = modl[ 0] * proj[ 1] + modl[ 1] * proj[ 5] + modl[ 2] * proj[ 9] + modl[ 3] * proj[13];
	clip[ 2] = modl[ 0] * proj[ 2] + modl[ 1] * proj[ 6] + modl[ 2] * proj[10] + modl[ 3] * proj[14];
	clip[ 3] = modl[ 0] * proj[ 3] + modl[ 1] * proj[ 7] + modl[ 2] * proj[11] + modl[ 3] * proj[15];

	clip[ 4] = modl[ 4] * proj[ 0] + modl[ 5] * proj[ 4] + modl[ 6] * proj[ 8] + modl[ 7] * proj[12];
	clip[ 5] = modl[ 4] * proj[ 1] + modl[ 5] * proj[ 5] + modl[ 6] * proj[ 9] + modl[ 7] * proj[13];
	clip[ 6] = modl[ 4] * proj[ 2] + modl[ 5] * proj[ 6] + modl[ 6] * proj[10] + modl[ 7] * proj[14];
	clip[ 7] = modl[ 4] * proj[ 3] + modl[ 5] * proj[ 7] + modl[ 6] * proj[11] + modl[ 7] * proj[15];

	clip[ 8] = modl[ 8] * proj[ 0] + modl[ 9] * proj[ 4] + modl[10] * proj[ 8] + modl[11] * proj[12];
	clip[ 9] = modl[ 8] * proj[ 1] + modl[ 9] * proj[ 5] + modl[10] * proj[ 9] + modl[11] * proj[13];
	clip[10] = modl[ 8] * proj[ 2] + modl[ 9] * proj[ 6] + modl[10] * proj[10] + modl[11] * proj[14];
	clip[11] = modl[ 8] * proj[ 3] + modl[ 9] * proj[ 7] + modl[10] * proj[11] + modl[11] * proj[15];

	clip[12] = modl[12] * proj[ 0] + modl[13] * proj[ 4] + modl[14] * proj[ 8] + modl[15] * proj[12];
	clip[13] = modl[12] * proj[ 1] + modl[13] * proj[ 5] + modl[14] * proj[ 9] + modl[15] * proj[13];
	clip[14] = modl[12] * proj[ 2] + modl[13] * proj[ 6] + modl[14] * proj[10] + modl[15] * proj[14];
	clip[15] = modl[12] * proj[ 3] + modl[13] * proj[ 7] + modl[14] * proj[11] + modl[15] * proj[15];

	/* Extract the numbers for the RIGHT plane */
	plane[0].normal.x = clip[ 3] - clip[ 0];
	plane[0].normal.y = clip[ 7] - clip[ 4];
	plane[0].normal.z = clip[11] - clip[ 8];
	plane[0].d = clip[15] - clip[12];

	t=plane[0].normal.Length();
	plane[0].normal/=t;
	plane[0].d/=t;

	/* Extract the numbers for the LEFT plane */
	plane[1].normal.x = clip[ 3] + clip[ 0];
	plane[1].normal.y = clip[ 7] + clip[ 4];
	plane[1].normal.z = clip[11] + clip[ 8];
	plane[1].d = clip[15] + clip[12];

	/* Normalize the result */
	t=plane[1].normal.Length();
	plane[1].normal/=t;
	plane[1].d/=t;

	/* Extract the BOTTOM plane */
	plane[2].normal.x = clip[ 3] + clip[ 1];
	plane[2].normal.y = clip[ 7] + clip[ 5];
	plane[2].normal.z = clip[11] + clip[ 9];
	plane[2].d = clip[15] + clip[13];

	/* Normalize the result */
	t=plane[2].normal.Length();
	plane[2].normal/=t;
	plane[2].d/=t;

	/* Extract the TOP plane */
	plane[3].normal.x = clip[ 3] - clip[ 1];
	plane[3].normal.y = clip[ 7] - clip[ 5];
	plane[3].normal.z = clip[11] - clip[ 9];
	plane[3].d = clip[15] - clip[13];

	/* Normalize the result */
	t=plane[3].normal.Length();
	plane[3].normal/=t;
	plane[3].d/=t;


	/* Extract the FAR plane */
	plane[4].normal.x = clip[ 3] - clip[ 2];
	plane[4].normal.y = clip[ 7] - clip[ 6];
	plane[4].normal.z = clip[11] - clip[10];
	plane[4].d = clip[15] - clip[14];

	/* Normalize the result */
	t=plane[4].normal.Length();
	plane[4].normal/=t;
	plane[4].d/=t;

	/* Extract the NEAR plane */
	plane[5].normal.x = clip[ 3] + clip[ 2];
	plane[5].normal.y = clip[ 7] + clip[ 6];
	plane[5].normal.z = clip[11] + clip[10];
	plane[5].d = clip[15] + clip[14];

	/* Normalize the result */
	t=plane[5].normal.Length();
	plane[5].normal/=t;
	plane[5].d/=t;	
}
bool Frustum::CubeInFrustum(vec3 pos,float size)
{
	int p;

	for(p=0;p<6;p++)
	{
		if( plane[p].normal.x * (pos.x - size) + plane[p].normal.y * (pos.y - size) + plane[p].normal.z * (pos.z - size) + plane[p].d > 0 )
			continue;
		if( plane[p].normal.x * (pos.x + size) + plane[p].normal.y * (pos.y - size) + plane[p].normal.z * (pos.z - size) + plane[p].d > 0 )
			continue;
		if( plane[p].normal.x * (pos.x - size) + plane[p].normal.y * (pos.y + size) + plane[p].normal.z * (pos.z - size) + plane[p].d > 0 )
			continue;
		if( plane[p].normal.x * (pos.x + size) + plane[p].normal.y * (pos.y + size) + plane[p].normal.z * (pos.z - size) + plane[p].d > 0 )
			continue;
		if( plane[p].normal.x * (pos.x - size) + plane[p].normal.y * (pos.y - size) + plane[p].normal.z * (pos.z + size) + plane[p].d > 0 )
			continue;
		if( plane[p].normal.x * (pos.x + size) + plane[p].normal.y * (pos.y - size) + plane[p].normal.z * (pos.z + size) + plane[p].d > 0 )
			continue;
		if( plane[p].normal.x * (pos.x - size) + plane[p].normal.y * (pos.y + size) + plane[p].normal.z * (pos.z + size) + plane[p].d > 0 )
			continue;
		if( plane[p].normal.x * (pos.x + size) + plane[p].normal.y * (pos.y + size) + plane[p].normal.z * (pos.z + size) + plane[p].d > 0 )
			continue;
		return false;
	}
	return true;
}


bool Frustum::BoxInFrustum(vec3 pos,vec3 size)
{	
	vec3 start=pos-size;
	vec3 end=pos+size;
	for(int i = 0; i < 6; i++ )
	{		
		if(plane[i].normal.x * start.x  + plane[i].normal.y * start.y  + plane[i].normal.z * start.z  + plane[i].d > 0)  continue;
		if(plane[i].normal.x * end.x + plane[i].normal.y * start.y  + plane[i].normal.z * start.z  + plane[i].d > 0)  continue;
		if(plane[i].normal.x * start.x  + plane[i].normal.y * end.y + plane[i].normal.z * start.z  + plane[i].d > 0)  continue;
		if(plane[i].normal.x * end.x + plane[i].normal.y * end.y + plane[i].normal.z * start.z  + plane[i].d > 0)  continue;
		if(plane[i].normal.x * start.x  + plane[i].normal.y * start.y  + plane[i].normal.z * end.z + plane[i].d > 0)  continue;
		if(plane[i].normal.x * end.x + plane[i].normal.y * start.y  + plane[i].normal.z * end.z + plane[i].d > 0)  continue;
		if(plane[i].normal.x * start.x  + plane[i].normal.y * end.y + plane[i].normal.z * end.z + plane[i].d > 0)  continue;
		if(plane[i].normal.x * end.x + plane[i].normal.y * end.y + plane[i].normal.z * end.z + plane[i].d > 0)  continue;

		return false;
	}

	return true;
}
