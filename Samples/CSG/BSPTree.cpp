#include <FireCube.h>
using namespace std;
using namespace FireCube;

#include "BSPTree.h"

void SplitFaces(GeometryPtr geometry, Plane &plane, vector<Face> &faces, vector<Face> &coplanarFront, vector<Face> &coplanarBack, vector<Face> &front, vector<Face> &back)
{
	const float epsilon = 1e-5f;
	for (auto face = begin(faces); face != end(faces); face++)
	{
		unsigned int i[3] = {face->v[0], face->v[1], face->v[2]};
		vec3 &v0 = geometry->GetVertices()[i[0]];
		vec3 &v1 = geometry->GetVertices()[i[1]];
		vec3 &v2 = geometry->GetVertices()[i[2]];
		vec3 &normal = face->normal;

		// Remove degenerate triangles. A small triangle close to a plane might be considered coplanar although it is not.
		if (((v0-v2).Length2() < epsilon * epsilon) || ((v0-v1).Length2() < epsilon * epsilon) || ((v1-v2).Length2() < epsilon * epsilon)) 
			continue;

		float d0 = plane.GetDistance(v0);
		float d1 = plane.GetDistance(v1);
		float d2 = plane.GetDistance(v2);

		// 0 - coplanar
		// 1 - front
		// 2 - back
		int faceType = 0;
		int type[3];
		
		type[0] = d0 < -epsilon ? 2 : (d0 > epsilon ? 1 : 0);
		type[1] = d1 < -epsilon ? 2 : (d1 > epsilon ? 1 : 0);
		type[2] = d2 < -epsilon ? 2 : (d2 > epsilon ? 1 : 0);
		faceType = type[0] | type[1] | type[2];
		
		if (faceType == 0)
		{
			if (plane.GetNormal().Dot(normal) > 0)
				coplanarFront.push_back(*face);
			else
				coplanarBack.push_back(*face);
		}
		else if (faceType == 1)
			front.push_back(*face);
		else if (faceType == 2)
			back.push_back(*face);
		else
		{
			// Split
			unsigned int index[3];
			// Case where one vertex(index[0]) is on one side and two on the other
			if (type[0] != 0 && type[1] != 0 && type[2] != 0)
			{				
				if (type[1] == type[2] && type[0] != type[1])
				{
					index[0] = 0;
					index[1] = 1;
					index[2] = 2;
				}
				else if (type[2] == type[0] && type[1] != type[0])
				{
					index[0] = 1;
					index[1] = 2;
					index[2] = 0;
				}
				else if (type[1] == type[0] && type[2] != type[0])
				{
					index[0] = 2;
					index[1] = 0;
					index[2] = 1;
				}
				float t3 = -plane.GetDistance(geometry->GetVertices()[i[index[0]]]) /  plane.GetNormal().Dot(geometry->GetVertices()[i[index[1]]]-geometry->GetVertices()[i[index[0]]]);
				float t4 = -plane.GetDistance(geometry->GetVertices()[i[index[0]]]) /  plane.GetNormal().Dot(geometry->GetVertices()[i[index[2]]]-geometry->GetVertices()[i[index[0]]]);
				int i3 = geometry->GetVertices().size();
				int i4 = geometry->GetVertices().size() + 1;

				geometry->GetVertices().push_back((1 - t3) * geometry->GetVertices()[i[index[0]]] + t3 *geometry->GetVertices()[i[index[1]]]);
				geometry->GetVertices().push_back((1 - t4) * geometry->GetVertices()[i[index[0]]] + t4 *geometry->GetVertices()[i[index[2]]]);
				if (!geometry->GetNormals().empty())
				{
					geometry->GetNormals().push_back(((1 - t3) * geometry->GetNormals()[i[index[0]]] + t3 *geometry->GetNormals()[i[index[1]]]).Normalize());
					geometry->GetNormals().push_back(((1 - t4) * geometry->GetNormals()[i[index[0]]] + t4 *geometry->GetNormals()[i[index[2]]]).Normalize());
				}
				if (!geometry->GetDiffuseUV().empty())
				{
					geometry->GetDiffuseUV().push_back((1 - t3) * geometry->GetDiffuseUV()[i[index[0]]] + t3 *geometry->GetDiffuseUV()[i[index[1]]]);				
					geometry->GetDiffuseUV().push_back((1 - t4) * geometry->GetDiffuseUV()[i[index[0]]] + t4 *geometry->GetDiffuseUV()[i[index[2]]]);					
				}
				if (!geometry->GetTangents().empty())
				{
					geometry->GetTangents().push_back(((1 - t3) * geometry->GetTangents()[i[index[0]]] + t3 *geometry->GetTangents()[i[index[1]]]).Normalize());
					geometry->GetTangents().push_back(((1 - t4) * geometry->GetTangents()[i[index[0]]] + t4 *geometry->GetTangents()[i[index[2]]]).Normalize());
				}
				if (!geometry->GetBitangents().empty())
				{
					geometry->GetBitangents().push_back(((1 - t3) * geometry->GetBitangents()[i[index[0]]] + t3 *geometry->GetBitangents()[i[index[1]]]).Normalize());
					geometry->GetBitangents().push_back(((1 - t4) * geometry->GetBitangents()[i[index[0]]] + t4 *geometry->GetBitangents()[i[index[2]]]).Normalize());
				}								

				Face f3, f4, f5;				
				f3.v[0] = i[index[0]];
				f3.v[1] = i3;
				f3.v[2] = i4;
				f3.normal = normal;

				f4.v[0] = i3;
				f4.v[1] = i[index[1]];
				f4.v[2] = i[index[2]];
				f4.normal = normal;

				f5.v[0] = i3;
				f5.v[1] = i[index[2]];
				f5.v[2] = i4;
				f5.normal = normal;

				if (type[index[0]] == 1)
				{
					front.push_back(f3);
					back.push_back(f4);
					back.push_back(f5);
				}
				else
				{
					back.push_back(f3);
					front.push_back(f4);
					front.push_back(f5);
				}
			}
			else
			{
				// Case where one vertex is on the plane and two on two different sides
				// assume index[0] is on plane
				if (type[0] == 0)
				{
					index[0] = 0;
					index[1] = 1;
					index[2] = 2;
				}
				else if (type[1] == 0)
				{
					index[0] = 1;
					index[1] = 2;
					index[2] = 0;
				}
				else if (type[2] == 0)
				{
					index[0] = 2;
					index[1] = 0;
					index[2] = 1;
				}
				float t3 = -plane.GetDistance(geometry->GetVertices()[i[index[1]]]) /  plane.GetNormal().Dot(geometry->GetVertices()[i[index[2]]]-geometry->GetVertices()[i[index[1]]]);				
				int i3 = geometry->GetVertices().size();				

				geometry->GetVertices().push_back((1 - t3) * geometry->GetVertices()[i[index[1]]] + t3 *geometry->GetVertices()[i[index[2]]]);								
				
				if (!geometry->GetNormals().empty())				
					geometry->GetNormals().push_back(((1 - t3) * geometry->GetNormals()[i[index[1]]] + t3 *geometry->GetNormals()[i[index[2]]]).Normalize());
				
				if (!geometry->GetDiffuseUV().empty())
					geometry->GetDiffuseUV().push_back((1 - t3) * geometry->GetDiffuseUV()[i[index[1]]] + t3 *geometry->GetDiffuseUV()[i[index[2]]]);

				if (!geometry->GetTangents().empty())
					geometry->GetTangents().push_back(((1 - t3) * geometry->GetTangents()[i[index[1]]] + t3 *geometry->GetTangents()[i[index[2]]]).Normalize());

				if (!geometry->GetBitangents().empty())
					geometry->GetBitangents().push_back(((1 - t3) * geometry->GetBitangents()[i[index[1]]] + t3 *geometry->GetBitangents()[i[index[2]]]).Normalize());
				
				Face f3, f4;	
				f3.v[0] = i[index[0]];
				f3.v[1] = i[index[1]];
				f3.v[2] = i3;
				f3.normal = normal;

				f4.v[0] = i[index[0]];
				f4.v[1] = i3;
				f4.v[2] = i[index[2]];
				f4.normal = normal;				

				if (type[index[1]] == 1)
				{
					front.push_back(f3);
					back.push_back(f4);					
				}
				else
				{
					back.push_back(f3);
					front.push_back(f4);					
				}
			}			
		}
	}	
}

BSPTree::BSPTree() : updated(false)
{

}

void BSPTree::FromGeometry(GeometryPtr geometry)
{
	if (geometry->GetFaces().empty())
		return;

	this->geometry = geometry->Clone();	
	vector<Face> faces = this->geometry->GetFaces();
	vertexUsage.resize(this->geometry->GetVertices().size());
	
	for (auto i = begin(vertexUsage); i != end(vertexUsage); i++)
	{
		*i = 0;
	}
	
	for (auto i = begin(faces); i !=end(faces); i++)
	{		
		vec3 &v0 = geometry->GetVertices()[i->v[0]];
		vec3 &v1 = geometry->GetVertices()[i->v[1]];
		vec3 &v2 = geometry->GetVertices()[i->v[2]];
		
		vertexUsage[i->v[0]]++;
		vertexUsage[i->v[1]]++;
		vertexUsage[i->v[2]]++;
	}

	root = NodePtr(new Node);
	BuildTree(root, faces);
	updated = true;
}

void BSPTree::BuildTree(NodePtr node, vector<Face> &faces)
{
	unsigned int index = ChooseSplittingPlane(faces);
	node->plane = Plane(faces[index].normal, -geometry->GetVertices()[faces[index].v[0]].Dot(faces[index].normal));

	vector<Face> front, back;
	SplitFaces(geometry, node->plane, faces, node->faces, node->faces, front, back);

	if (!front.empty())
	{
		if (!node->front)
			node->front = NodePtr(new Node);

		BuildTree(node->front, front);
	}

	if (!back.empty())
	{
		if (!node->back)
			node->back = NodePtr(new Node);

		BuildTree(node->back, back);
	}		
}

vector<Face> BSPTree::ClipFaces(NodePtr node, GeometryPtr pGeometry, vector<Face> &faces)
{
	vector<Face> front, back;
	SplitFaces(pGeometry, node->plane, faces, front, back, front, back);
	
	if (node->front)
	{
		front = ClipFaces(node->front, pGeometry, front);
	}

	if (node->back)
	{
		back = ClipFaces(node->back, pGeometry, back);
	}
	else
		back.clear();

	front.insert(front.end(), back.begin(), back.end());

	return front;		
}

void BSPTree::ClipTo(NodePtr node, BSPTree &bsp)
{
	node->faces = ClipFaces(bsp.root, geometry, node->faces);

	if (node->front)
		ClipTo(node->front, bsp);

	if (node->back)
		ClipTo(node->back, bsp);
}

void BSPTree::ClipTo(BSPTree &bsp)
{
	ClipTo(root, bsp);
}
void BSPTree::Invert(NodePtr node)
{	
	for (auto face = begin(node->faces); face != end(node->faces); face++)
	{
		face->normal *= -1;
		unsigned int temp = face->v[1];
		face->v[1] = face->v[2];
		face->v[2] = temp;
	}

	node->plane.SetNormal(node->plane.GetNormal() * -1);
	node->plane.SetDistance(-node->plane.GetDistance());
	if (node->front)
		Invert(node->front);
	if (node->back)
		Invert(node->back);

	NodePtr temp = node->front;
	node->front = node->back;
	node->back = temp;
}
void BSPTree::Invert()
{
	Invert(root);
	for (auto n = begin(geometry->GetNormals()); n != end(geometry->GetNormals()); n++)
		*n *= -1;

	for (auto n = begin(geometry->GetTangents()); n != end(geometry->GetTangents()); n++)
		*n *= -1;

	for (auto n = begin(geometry->GetBitangents()); n != end(geometry->GetBitangents()); n++)
		*n *= -1;

	
}

void BSPTree::Union(BSPTree &other, BSPTree &ret1, BSPTree &ret2)
{	
	this->Clone(ret1);
	other.Clone(ret2);	
	ret1.ClipTo(ret2);
	ret2.ClipTo(ret1);
	ret2.Invert();
	ret2.ClipTo(ret1);
	ret2.Invert();
	ret1.updated = false;
	ret2.updated = false;
}
void BSPTree::Subtract(BSPTree &other, BSPTree &ret1, BSPTree &ret2)
{
	this->Clone(ret1);
	other.Clone(ret2);	
	ret1.Invert();	
	ret1.ClipTo(ret2);
	ret2.ClipTo(ret1);
	ret2.Invert();
	ret2.ClipTo(ret1);	
	ret1.Invert();	
	ret1.updated = false;
	ret2.updated = false;
}
void BSPTree::Intersect(BSPTree &other, BSPTree &ret1, BSPTree &ret2)
{	
	this->Clone(ret1);
	other.Clone(ret2);	
	ret1.Invert();	
	ret2.ClipTo(ret1);
	ret2.Invert();
	ret1.ClipTo(ret2);
	ret2.ClipTo(ret1);
	ret1.Invert();
	ret2.Invert();
	ret1.updated = false;
	ret2.updated = false;
}
BSPTree::NodePtr BSPTree::Clone(NodePtr node)
{
	NodePtr ret(new Node);
	ret->plane = node->plane;
	ret->faces = node->faces;
	if (node->front)
		ret->front = Clone(node->front);
	if (node->back)
		ret->back = Clone(node->back);
	return ret;
}
void BSPTree::Clone(BSPTree &clone)
{
	clone.geometry = geometry->Clone();
	clone.vertexUsage = vertexUsage;
	clone.root = Clone(root);
}

GeometryPtr BSPTree::GetGeometry()
{
	if (!updated)
	{
		Update();
		updated = true;
	}
	return geometry;
}

void BSPTree::Update()
{
	geometry->GetFaces().clear();
	CopyFacesToGeometry(root);	
	geometry->CopyFacesToIndices();
	geometry->SetVertexCount(geometry->GetIndices().size());
	geometry->SetPrimitiveCount(geometry->GetIndices().size() / 3);
	geometry->CalculateBoundingBox();
	geometry->UpdateBuffers();
}

void BSPTree::CopyFacesToGeometry(NodePtr node)
{
	geometry->GetFaces().insert(end(geometry->GetFaces()), begin(node->faces), end(node->faces));
	if (node->front)
		CopyFacesToGeometry(node->front);
	if (node->back)
		CopyFacesToGeometry(node->back);
}
unsigned int BSPTree::ChooseSplittingPlane(vector<Face> &faces)
{	
	return rand() % faces.size();	
}