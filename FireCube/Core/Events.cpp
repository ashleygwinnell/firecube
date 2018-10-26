#include "Events.h"

using namespace FireCube;

Event<float> Events::Update;
Event<float> Events::PostRender;
Event<float, const MappedInput &> Events::HandleInput;
Event<CharacterController *, CollisionShape *> Events::CharacterControllerCollision;
Event<Resource *> Events::ResourceReloaded;
Event<const std::set<Shader *> &> Events::ShaderReloaded;