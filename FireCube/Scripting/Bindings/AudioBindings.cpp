#include "lua.hpp"
#include "LuaBridge.h"
#include "Scripting/LuaBindings.h"
#include "Audio/SoundEmitter.h"
#include "Audio/Sound.h"

using namespace FireCube;
using namespace luabridge;

void LuaBindings::InitAudio(lua_State *luaState)
{
	getGlobalNamespace(luaState)
		.deriveClass<SoundEmitter, Component>("SoundEmitter")
			.addFunction("Play", (void(SoundEmitter::*)()) &SoundEmitter::Play)
			.addProperty("sound", &SoundEmitter::GetSound, &SoundEmitter::SetSound)
			.addProperty("panning", &SoundEmitter::GetPanning, &SoundEmitter::SetPanning)
			.addProperty("gain", &SoundEmitter::GetGain, &SoundEmitter::SetGain)
			.addProperty("looped", &SoundEmitter::GetLooped, &SoundEmitter::SetLooped)
		.endClass()
		.deriveClass<Sound, Resource>("Sound")
		.endClass();
			
}

