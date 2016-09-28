#include "WindowProperties.h"

using namespace FireCube;

WindowProperties::WindowProperties() : width(800), height(600), multisample(0), fullscreen(false), maximized(false), resizable(true)
{

}

WindowProperties::WindowProperties(int width, int height, int multisample, bool fullscreen, bool maximized, bool resizable) : width(width), height(height), multisample(multisample),
	fullscreen(fullscreen), maximized(maximized), resizable(resizable)
{

}

WindowProperties &WindowProperties::Width(int width)
{
	this->width = width;
	return *this;
}

WindowProperties &WindowProperties::Height(int height)
{
	this->height = height;
	return *this;
}

WindowProperties &WindowProperties::Multisample(int multisample)
{
	this->multisample = multisample;
	return *this;
}

WindowProperties &WindowProperties::Fullscreen(bool fullscreen)
{
	this->fullscreen = fullscreen;
	return *this;
}

WindowProperties &WindowProperties::Maximized(bool maximized)
{
	this->maximized = maximized;
	return *this;
}

WindowProperties &WindowProperties::Resizable(bool resizable)
{
	this->resizable = resizable;
	return *this;
}
