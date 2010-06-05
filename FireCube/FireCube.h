#ifndef FIRECUBE_H
#define FIRECUBE_H

#ifdef FIRECUBE_EXPORTS
#define FIRECUBE_API __declspec(dllexport)
#else
#define FIRECUBE_API __declspec(dllimport)
#pragma comment(lib, "FireCube.lib")
#endif
/* CPPDOC_BEGIN_EXCLUDE */
typedef struct lua_State lua_State;
/* CPPDOC_END_EXCLUDE */
namespace FireCube
{	
	#include "Logger.h"
	#include "ResourceManager.h"
	#include "Timer.h"
	#include "MyMath.h"	
	#include "BoundingBox.h"
	#include "Texture.h"		
	#include "Buffer.h"
	#include "Shaders.h"
	#include "Mesh.h"	
	#include "FrameBuffer.h"
	#include "Image.h"
	#include "Font.h"
	#include "Renderer.h"			
	#include "utils.h"	
	#include "Application.h"
};

#endif