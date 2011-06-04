#include <FireCube.h>
#include <fstream>
using namespace std;
using namespace FireCube;
#include "Frustum.h"
#include "QuadTree.h"

QuadTree::QuadTree()
{
}
void QuadTree::Initialize()
{
    indexBuffer = BufferPtr(new Buffer);
    indexBuffer->Create();
    plainColor = ProgramPtr(new Program);
    plainColor->Create(Renderer::GetShaderManager().Create("plainColor.vert"), Renderer::GetShaderManager().Create("plainColor.frag"));
}
void QuadTree::Init(vec2 size, vec2 verticesSize)
{

    root = NodePtr(new Node);
    root->face = Node::FaceListPtr(new Node::FaceList);
    root->boundingBox.SetMin(vec3(0.0f, 0.0f, 0.0f));
    root->boundingBox.SetMax(vec3(verticesSize.x, 50.0f, verticesSize.y));
    aspect = (size - vec2(1.0f, 1.0f)) / verticesSize;
    for (unsigned int y = 0; y < size.y - 1.0f; y++)
    {
        for (unsigned int x = 0; x < size.x - 1.0f; x++)
        {
            root->face->push_back(vec2((float)x / (size.x - 1.0f)*verticesSize.x, (float)y / (size.y - 1.0f)*verticesSize.y));
        }
    }
    this->size = size - vec2(1.0f, 1.0f);
}
void QuadTree::Build(float minSize, unsigned int maxNumberOfFaces)
{
    Build(root, minSize, maxNumberOfFaces);
    BuildIndices(root);
}
void QuadTree::Build(NodePtr node, float minSize, unsigned int maxNumberOfFaces)
{
    if ((node->boundingBox.GetWidth() <= minSize) || (node->face->size() < maxNumberOfFaces))
        return;
    vec3 midPoint = node->boundingBox.GetCenter();
    NodePtr child[4];
    child[0] = NodePtr(new Node);
    child[0]->face = Node::FaceListPtr(new Node::FaceList);
    child[1] = NodePtr(new Node);
    child[1]->face = Node::FaceListPtr(new Node::FaceList);
    child[2] = NodePtr(new Node);
    child[2]->face = Node::FaceListPtr(new Node::FaceList);
    child[3] = NodePtr(new Node);
    child[3]->face = Node::FaceListPtr(new Node::FaceList);
    child[0]->boundingBox.SetMin(node->boundingBox.GetMin());
    child[0]->boundingBox.SetMax(vec3(midPoint.x, 50.0f, midPoint.z));
    child[1]->boundingBox.SetMin(vec3(midPoint.x, 0.0f, node->boundingBox.GetMin().z));
    child[1]->boundingBox.SetMax(vec3(node->boundingBox.GetMax().x, 50.0f, midPoint.z));
    child[2]->boundingBox.SetMin(vec3(node->boundingBox.GetMin().x, 0.0f, midPoint.z));
    child[2]->boundingBox.SetMax(vec3(midPoint.x, 50.0f, node->boundingBox.GetMax().z));
    child[3]->boundingBox.SetMin(vec3(midPoint.x, 0.0f, midPoint.z));
    child[3]->boundingBox.SetMax(node->boundingBox.GetMax());
    for (unsigned int i = 0; i < node->face->size(); i++)
    {
        for (unsigned int j = 0; j < 4; j++)
        {
            vec2 f = (*node->face)[i];
            vec2 f2 = (*node->face)[i] + vec2(1.0f / aspect.x, 1.0f / aspect.y);
            if (((f.x >= child[j]->boundingBox.GetMin().x) && (f.y >= child[j]->boundingBox.GetMin().z) && (f.x <= child[j]->boundingBox.GetMax().x) && (f.y <= child[j]->boundingBox.GetMax().z) ) || ((f2.x >= child[j]->boundingBox.GetMin().x) && (f2.y >= child[j]->boundingBox.GetMin().z) && (f2.x <= child[j]->boundingBox.GetMax().x) && (f2.y <= child[j]->boundingBox.GetMax().z)))
            {
                child[j]->face->push_back(f);
            }
        }
    }
    node->face.reset();
    for (unsigned int i = 0; i < 4; i++)
    {
        if (child[i]->face->size() == 0)
            child[i].reset();
        node->child[i] = child[i];
    }
    for (unsigned int i = 0; i < 4; i++)
        if (node->child[i])
            Build(node->child[i], minSize, maxNumberOfFaces);
}
unsigned int QuadTree::Render(Frustum &frustum)
{
    currentIndex = 0;
    Render(root, frustum);
    if (currentIndex > 0)
    {
        indexBuffer->LoadIndexData(&indicesToRender[0], currentIndex, DYNAMIC);
        indexBuffer->SetIndexStream();
        Renderer::RenderIndexStream(TRIANGLES, currentIndex);
    }
    return currentIndex;
}
void QuadTree::Render(NodePtr node, Frustum &frustum)
{
    vec3 pos =  node->boundingBox.GetCenter(); //vec3( (node->min.x + node->max.x) / 2.0f, 25.0f, (node->min.y + node->max.y) / 2.0f);
    vec3 sized = node->boundingBox.GetSize() / 2.0f; //vec3( (node->max.x - node->min.x) / 2.0f, 25.0f, (node->max.y - node->min.y) / 2.0f);
    unsigned int sx = (unsigned int)this->size.x * 6;
    if (frustum.Contains(node->boundingBox))
    {
        if (!node->indices.empty())
        {
            if (indicesToRender.size() - currentIndex < node->indices.size())
                indicesToRender.resize(indicesToRender.size() + node->indices.size());
            std::copy(node->indices.begin(), node->indices.end(), indicesToRender.begin() + currentIndex);
            currentIndex += node->indices.size();
        }
        else
        {
            for (unsigned int i = 0; i < 4; i++)
                if (node->child[i])
                    Render(node->child[i], frustum);
        }
    }
}
void QuadTree::BuildIndices(NodePtr node)
{
    unsigned int sx = (unsigned int)this->size.x * 6;
    if ((node->face) && (node->face->size() > 0))
    {
        node->indices.resize(node->face->size() * 6);
        unsigned int cIndex = 0;
        for (unsigned int i = 0; i < node->face->size(); i++)
        {
            vec2 f = (*node->face)[i] * aspect;
            unsigned int x = (unsigned int)f.x;
            unsigned int y = (unsigned int)f.y;
            unsigned int index = y * sx + x * 6;
            unsigned int fi = y * sx / 6 + x;
            node->indices[cIndex++] = index + 0;
            node->indices[cIndex++] = index + 1;
            node->indices[cIndex++] = index + 2;
            node->indices[cIndex++] = index + 3;
            node->indices[cIndex++] = index + 4;
            node->indices[cIndex++] = index + 5;
        }
        node->face.reset();
    }
    else
    {
        for (unsigned int i = 0; i < 4; i++)
            if (node->child[i])
                BuildIndices(node->child[i]);
    }
}
void QuadTree::RenderLines()
{
    Renderer::UseProgram(plainColor);
    RenderLines(root);
}
void QuadTree::RenderLines(NodePtr node)
{
    BufferPtr v(new Buffer);
    v->Create();
    vector<vec3> vv;
    vv.push_back(vec3(node->boundingBox.GetMin().x, 20.0f, node->boundingBox.GetMin().z));
    vv.push_back(vec3(node->boundingBox.GetMax().x, 20.0f, node->boundingBox.GetMin().z));
    vv.push_back(vec3(node->boundingBox.GetMax().x, 20.0f, node->boundingBox.GetMax().z));
    vv.push_back(vec3(node->boundingBox.GetMin().x, 20.0f, node->boundingBox.GetMax().z));
    v->LoadData(&vv[0], sizeof(vec3)*vv.size(), DYNAMIC);
    v->SetVertexStream(3);
    Renderer::RenderStream(LINE_LOOP, vv.size());
    for (unsigned int j = 0; j < 4; j++)
        if (node->child[j])
            RenderLines(node->child[j]);
}
void QuadTree::Save(const string &filename)
{
    ofstream file(filename.c_str(), ios::binary);
    file.write((const char*)&size, sizeof(vec2));
    file.write((const char*)&aspect, sizeof(vec2));
    Save(root, file);
}
void QuadTree::Save(NodePtr node, ofstream &file)
{
    unsigned char children = 0;
    unsigned int numIndices = node->indices.size();
    if (node->child[0])
        children |= 1;
    if (node->child[1])
        children |= 2;
    if (node->child[2])
        children |= 4;
    if (node->child[3])
        children |= 8;
    file.write((const char*)&node->boundingBox.GetMin(), sizeof(vec3));
    file.write((const char*)&node->boundingBox.GetMax(), sizeof(vec3));
    file.write((const char*)&children, 1);
    file.write((const char*)&numIndices, sizeof(unsigned int));
    if (numIndices > 0)
    {
        unsigned int lastIndex = node->indices[0];
        for (unsigned int i = 1; i < node->indices.size(); i++)
        {
            if (node->indices[i] != node->indices[i - 1] + 1)
            {
                file.write((const char *)&lastIndex, sizeof(unsigned int));
                unsigned int j = node->indices[i - 1];
                file.write((const char *)&j, sizeof(unsigned int));
                lastIndex = node->indices[i];
                if (i == node->indices.size() - 1)
                {
                    file.write((const char *)&lastIndex, sizeof(unsigned int));
                    file.write((const char *)&lastIndex, sizeof(unsigned int));
                }
            }
            else
            {
                if (i == node->indices.size() - 1)
                {
                    file.write((const char *)&lastIndex, sizeof(unsigned int));
                    unsigned int j = node->indices[i];
                    file.write((const char *)&j, sizeof(unsigned int));
                }
            }
        }
    }
    for (unsigned int i = 0; i < 4; i++)
        if (node->child[i])
            Save(node->child[i], file);
}
void QuadTree::Load(const string &filename)
{
    ifstream file(filename.c_str(), ios::binary);
    file.seekg(0, ios_base::end);
    unsigned int size = file.tellg(), currentIndex = 0;
    vector<unsigned char> buffer;
    buffer.resize(size);
    file.seekg(0, ios_base::beg);
    file.read((char*)&buffer[0], size);
    this->size = *(vec2*)&buffer[currentIndex];
    currentIndex += sizeof(vec2);
    aspect = *(vec2*)&buffer[currentIndex];
    currentIndex += sizeof(vec2);
    root = NodePtr(new Node);
    Load(buffer, currentIndex, root);
}
void QuadTree::Load(const vector<unsigned char> &buffer, unsigned int &currentIndex, NodePtr node)
{
    unsigned char children;
    unsigned int numIndices;
    node->boundingBox.SetMin(*(vec3*)&buffer[currentIndex]);
    currentIndex += sizeof(vec3);
    node->boundingBox.SetMax(*(vec3*)&buffer[currentIndex]);
    currentIndex += sizeof(vec3);
    children = *(unsigned char*)&buffer[currentIndex];
    currentIndex += 1;
    numIndices = *(unsigned int*)&buffer[currentIndex];
    currentIndex += sizeof(unsigned int);
    if (numIndices > 0)
    {
        node->indices.resize(numIndices);
        unsigned int startIndex, endIndex, nodeCurrentIndex = 0;
        while (nodeCurrentIndex != numIndices)
        {

            startIndex = *(unsigned int*)&buffer[currentIndex];
            currentIndex += sizeof(unsigned int);
            endIndex = *(unsigned int*)&buffer[currentIndex];
            currentIndex += sizeof(unsigned int);
            for (unsigned int i = startIndex; i <= endIndex; i++)
                node->indices[nodeCurrentIndex++] = i;
        }
    }
    if (children & 1)
    {
        node->child[0] = NodePtr(new Node);
        Load(buffer, currentIndex, node->child[0]);
    }
    if (children & 2)
    {
        node->child[1] = NodePtr(new Node);
        Load(buffer, currentIndex, node->child[1]);
    }
    if (children & 4)
    {
        node->child[2] = NodePtr(new Node);
        Load(buffer, currentIndex, node->child[2]);
    }
    if (children & 8)
    {
        node->child[3] = NodePtr(new Node);
        Load(buffer, currentIndex, node->child[3]);
    }
}