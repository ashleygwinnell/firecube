#include <map>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;
#include <boost/weak_ptr.hpp>
#include <boost/smart_ptr.hpp>
#include <Windows.h>
#include <gl/GL.h>
#include <FireCube.h>
using namespace FireCube;

extern "C" {
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}
#include <luabind/luabind.hpp>
#include <luabind/operator.hpp>
#include <luabind/object.hpp>
class Test2
{
public:
	Test2()
	{
		vl=0;
	}
	void SetVl(int k) {vl=k;}
	int GetVl() {return vl;}
	int vl;
};
class Test
{
public:
	Test()
	{
		v=0;		
	}
	void SetV(int k) {v=k;}
	int GetV() {return v;}
	int v;
};
void SetV(boost::shared_ptr<Test> &t,int vv)
{
	t->v=vv;
}
void DoIt(boost::shared_ptr<Test> &t,boost::shared_ptr<Test2> vv)
{
	t->v=vv->vl;
}

int GetV(boost::shared_ptr<Test> &t)
{
	return t->v;
}

void main()
{	
	Application::InitializeLua();
	luabind::module(Application::GetLuaState())
	[
		luabind::class_<Test,boost::shared_ptr<Test> >("Test")
			.def(luabind::constructor<>())
			.def("SetV",&SetV)
			.def("GetV",&GetV)
			.def("DoIt",&DoIt),
		luabind::class_<Test2,boost::shared_ptr<Test2> >("Test2")
			.def(luabind::constructor<>())
			.def("SetVl",&Test2::SetVl)
			.def("GetVl",&Test2::GetVl)
	];
	Application::ExecuteFile("script.lua");	
	Application::CloseLua();
}