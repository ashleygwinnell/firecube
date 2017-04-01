#include "lua.hpp"
#include "sol.hpp"
#include "Scripting/LuaBindings.h"
#include "Audio/SoundEmitter.h"
#include "Audio/Sound.h"

using namespace FireCube;

void LuaBindings::InitAudio(sol::state &luaState)
{
	luaState.new_usertype<SoundEmitter>("SoundEmitter",
		"Play", (void(SoundEmitter::*)()) &SoundEmitter::Play,
		"sound", sol::property(&SoundEmitter::GetSound, &SoundEmitter::SetSound),
		"panning", sol::property(&SoundEmitter::GetPanning, &SoundEmitter::SetPanning),
		"gain", sol::property(&SoundEmitter::GetGain, &SoundEmitter::SetGain),
		"looped", sol::property(&SoundEmitter::GetLooped, &SoundEmitter::SetLooped),
		sol::base_classes, sol::bases<Component, Object, RefCounted>());
	
	luaState.new_usertype<Sound>("Sound",
		sol::base_classes, sol::bases<Resource, Object, RefCounted>());				
}

