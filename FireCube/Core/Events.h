#include "Core/EventSystem.h"

namespace FireCube
{

class MappedInput;
class CollisionShape;
class CharacterController;
class Resource;

namespace Events
{

extern Event<float> Update;
extern Event<float> PostRender;
extern Event<float, const MappedInput &> HandleInput;
extern Event<CharacterController *, CollisionShape *> CharacterControllerCollision;
extern Event<Resource *> ResourceReloaded;

}

}