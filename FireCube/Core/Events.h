#include <set>

#include "Core/EventSystem.h"

namespace FireCube
{

class MappedInput;
class CollisionShape;
class CharacterController;
class Resource;
class Shader;

namespace Events
{

extern Event<float> PreUpdate;
extern Event<float> Update;
extern Event<float> PostRender;
extern Event<float> PreRender;
extern Event<float, const MappedInput &> HandleInput;
extern Event<CharacterController *, CollisionShape *> CharacterControllerCollision;
extern Event<Resource *> ResourceReloaded;
extern Event<const std::set<Shader *> &> ShaderReloaded;
extern Event<CollisionShape *, CollisionShape *> CollisionShapeEnterCollision;
extern Event<CollisionShape *, CollisionShape *> CollisionShapeLeaveCollision;

}

}