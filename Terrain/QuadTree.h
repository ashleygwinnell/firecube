#ifndef QUADTREE_H
#define QUADTREE_H

class QuadTree
{
public:
    class Node;
    typedef boost::shared_ptr<Node> NodePtr;
    class Node
    {
    public:
        typedef vector<vec2> FaceList;
        typedef boost::shared_ptr<FaceList> FaceListPtr;
        vec2 min, max;
        FaceListPtr face;
        vector<DWORD> indices;
        NodePtr child[4];
    };
    QuadTree();
    void Initialize();
    void Init(vec2 size, vec2 verticesSize);
    void Build(float minSize, DWORD maxNumerOfFaces);
    DWORD Render(Frustum &frustum);
    void RenderLines();
    void Save(const string &filename);
    void Load(const string &filename);
private:
    void Load(const vector<unsigned char> &buffer, DWORD &currentIndex, NodePtr node);
    void Save(NodePtr node, ofstream &file);
    void Build(NodePtr node, float minSize, DWORD maxNumerOfFaces);
    void BuildIndices(NodePtr node);
    void Render(NodePtr node, Frustum &frustum);
    void RenderLines(NodePtr node);
    NodePtr root;
    vec2 size;
    vector<DWORD> indicesToRender;
    Buffer indexBuffer;
    vec2 aspect;
    DWORD currentIndex;
    Program plainColor;
};

#endif