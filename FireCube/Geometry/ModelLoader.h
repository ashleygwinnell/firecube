#pragma once

#include <string>
#include <vector>
#include <memory>
#include "Math/BoundingBox.h"
#include "Core/Object.h"

namespace FireCube
{

class Renderer;
class Geometry;
class Material;
class Node;
typedef std::shared_ptr<Node> NodePtr;
class Viewport;

/**
* A class describing various parameters to apply to models when loaded from a file
*/
class FIRECUBE_API ModelLoadingOptions
{
public:
	ModelLoadingOptions();
	
	/**
	* Boolean flag specifying whether to flip u texture coordinate.
	*/
	bool flipU;

	/**
	* Boolean flag specifying whether to flip v texture coordinate.
	*/
	bool flipV;
};

class ModelLoader : public Object
{
public:
	ModelLoader(Engine *engine);
	virtual bool Load(const std::string &filename, ModelLoadingOptions options = ModelLoadingOptions()) = 0;	
	virtual void GenerateGeometries(Renderer *renderer) = 0;
	virtual void GenerateScene(Renderer *renderer, Node *root) = 0;
	virtual const std::vector<Geometry *> &GetGeneratedGeometries() = 0;
	virtual NodePtr GetGeneratedScene() = 0;
	virtual const std::vector<Material *> &GetGeneratedMaterials() = 0;
	virtual BoundingBox GetBoundingBox() const = 0;
};

}