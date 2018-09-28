#pragma once

#include <vector>
#include "Geometry/CollisionQuery.h"
#include "Rendering/RenderingTypes.h"
#include "Core/Memory.h"
#include "Scene/Node.h"
#include "Scene/Light.h"
#include "Core/Component.h"

namespace FireCube
{

class Engine;

class LuaBindings
{
public:
	static void Init(sol::state &luaState, Engine *engine);
	static void InitMath(sol::state &luaState);
	static void InitScene(sol::state &luaState);
	static void InitUtils(sol::state &luaState);
	static void InitRendering(sol::state &luaState);
	static void InitCore(sol::state &luaState, Engine *engine);
	static void InitScripting(sol::state &luaState);
	static void InitGeometry(sol::state &luaState);
	static void InitPhysics(sol::state &luaState);
	static void InitAudio(sol::state &luaState);
	static void InitUI(sol::state &luaState);
};

}