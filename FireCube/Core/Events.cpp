#include "Events.h"

using namespace FireCube;

Event<float> Events::Update;
Event<float, const MappedInput &> Events::HandleInput;
Event<CharacterController *, CollisionShape *> Events::CharacterControllerCollision;