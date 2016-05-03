#pragma once

#include "Utils/utils.h"

namespace FireCube
{

class Renderer;
class ResourceCache;
class InputManager;
class DebugRenderer;
class UI;
class LuaState;
class Audio;

class FIRECUBE_API Engine
{
public:
	Engine();
	
	/**
	* Sets the renderer
	* @param renderer The renderer to set
	*/
	void SetRenderer(Renderer *renderer);
	
	/**
	* Returns the renderer
	*/
	Renderer *GetRenderer() const;
	
	/**
	* Sets the resource cahce
	* @param resourceCache The resource cahce to set
	*/
	void SetResourceCache(ResourceCache *resourceCache);
	
	/**
	* Returns the resource cahce
	*/
	ResourceCache *GetResourceCache() const;
	
	/**
	* Sets the input manager
	* @param inputManager The input manager to set
	*/
	void SetInputManager(InputManager *inputManager);
	
	/**
	* Returns the input manager
	*/
	InputManager *GetInputManager() const;
	
	/**
	* Sets the debug renderer
	* @param debugRenderer The debug renderer to set
	*/
	void SetDebugRenderer(DebugRenderer *debugRenderer);
	
	/**
	* Returns the debug renderer
	*/
	DebugRenderer *GetDebugRenderer() const;
	
	/**
	* Sets the UI
	* @param ui The UI to set
	*/
	void SetUI(UI *ui);
	
	/**
	* Returns the UI
	*/
	UI *GetUI() const;
	
	/**
	* Sets the Lua state
	* @param luaState The Lua state to set
	*/
	void SetLuaState(LuaState *luaState);
	
	/**
	* Returns the Lua state
	*/
	LuaState *GetLuaState() const;
	
	/**
	* Sets the audio
	* @param audio The audio to set
	*/
	void SetAudio(Audio *audio);
	
	/**
	* Returns the audio
	*/
	Audio *GetAudio() const;

private:
	Renderer *renderer;
	ResourceCache *resourceCache;
	InputManager *inputManager;
	DebugRenderer *debugRenderer;
	UI *ui;
	LuaState *luaState;
	Audio *audio;
};

}