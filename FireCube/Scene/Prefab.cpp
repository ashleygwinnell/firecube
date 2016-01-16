#include "Prefab.h"
#include "tinyxml.h"
#include "Utils/Filesystem.h"
#include "Node.h"
#include "SceneReader.h"

using namespace FireCube;

Prefab::Prefab(Engine *engine) : Resource(engine), node(nullptr)
{

}

Prefab::~Prefab()
{
	delete node;
}

bool Prefab::Load(const std::string &filename)
{
	std::string resolvedFileName = Filesystem::FindResourceByName(filename);
	if (resolvedFileName.empty())
		return false;

	TiXmlDocument xmlDocument;
	if (!xmlDocument.LoadFile(resolvedFileName))
		return false;

	TiXmlElement *e = xmlDocument.FirstChildElement("node");
	if (e == nullptr)
		return false;

	node = new Node(engine);

	SceneReader sceneReader(engine);
	sceneReader.ReadNode(e, node);

	return true;
}

const Node *Prefab::GetNode() const
{
	return node;
}
