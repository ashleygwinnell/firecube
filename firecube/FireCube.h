#ifndef FIRECUBE_H
#define FIRECUBE_H

#ifdef FIRECUBE_EXPORTS
#define FIRECUBE_API __declspec(dllexport)
#else
#define FIRECUBE_API __declspec(dllimport)
#pragma comment(lib, "FireCube.lib")
#endif

namespace FireCube
{
	#include "Timer.h"
	#include "MyMath.h"
	#include "Application.h"	
	#include "Renderer.h"
	#include "Mesh.h"
	#include "utils.h"
};

#endif