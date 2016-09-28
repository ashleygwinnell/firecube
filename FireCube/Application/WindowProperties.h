#pragma once

namespace FireCube
{

class WindowProperties
{
public:
	WindowProperties();
	WindowProperties(int width, int height, int multisample, bool fullscreen, bool maximized, bool resizable);

	WindowProperties &Width(int width);
	WindowProperties &Height(int height);
	WindowProperties &Multisample(int multisample);
	WindowProperties &Fullscreen(bool fullscreen);
	WindowProperties &Maximized(bool maximized);
	WindowProperties &Resizable(bool resizable);

	int width;
	int height;
	int multisample;
	bool fullscreen;
	bool maximized;
	bool resizable;
};

}