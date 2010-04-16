#include <string>
#include <vector>
#include <map>
#include <queue>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
using namespace std;
#include <Windows.h>
#include "GLee.h"
#include <ft2build.h>
#include FT_FREETYPE_H
#include "FireCube.h"
using namespace FireCube;

extern "C" {
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}
#include <luabind/luabind.hpp>
#include <luabind/operator.hpp>
#include <luabind/object.hpp>

#define VECTOR_SET_NAMED(t,name) void name(vector<t> &v,vector<t>::size_type i,t &val) {v[i]=val;}
#define VECTOR_SET(t) void set_vector_##t(vector<t> &v,vector<t>::size_type i,t &val) {v[i]=val;}
#define VECTOR_GET_NAMED(t,name) t name(vector<t> &v,vector<t>::size_type i) {return v[i];}
#define VECTOR_GET(t) t get_vector_##t(vector<t> &v,vector<t>::size_type i) {return v[i];}

VECTOR_GET(vec4)
VECTOR_GET(vec3)
VECTOR_GET(vec2)
VECTOR_GET(float)
VECTOR_GET(int)
VECTOR_GET(DWORD)
VECTOR_GET(Face)
VECTOR_GET(Mesh)
VECTOR_GET(Buffer)
VECTOR_GET_NAMED(vector<vec2>,get_vector_vector_vec2)
VECTOR_GET(Object)
VECTOR_GET(Material)


VECTOR_SET(vec4)
VECTOR_SET(vec3)
VECTOR_SET(vec2)
VECTOR_SET(float)
VECTOR_SET(int)
VECTOR_SET(DWORD)
VECTOR_SET(Face)
VECTOR_SET(Mesh)
VECTOR_SET(Buffer)
VECTOR_SET_NAMED(vector<vec2>,set_vector_vector_vec2)
VECTOR_SET(Object)
VECTOR_SET(Material)

#define LUABIND_VECTOR_NAMED(t,name,setfunction,getfunction) \
	luabind::class_<vector<t> >(name)	\
	.def(luabind::constructor<>())	\
	.def(luabind::constructor<vector<t>::size_type>())	\
	.def(luabind::constructor<vector<t>::size_type,vector<t>::const_reference>())	\
	.def("push_back",(void(vector<t>::*)(vector<t>::const_reference))&vector<t>::push_back)	\
	.def("at",&##getfunction)	\
	.def("set",&##setfunction) \
	.def("clear",&vector<t>::clear)	\
	.def("reserve",&vector<t>::reserve)	\
	.def("resize",(void(vector<t>::*)(vector<t>::size_type)) &vector<t>::resize)	\
	.def("resize",(void(vector<t>::*)(vector<t>::size_type,t)) &vector<t>::resize)	\
	.def("size",&vector<t>::size)

#define LUABIND_VECTOR(t,name) \
	luabind::class_<vector<t> >(name)	\
	.def(luabind::constructor<>())	\
	.def(luabind::constructor<vector<t>::size_type>())	\
	.def(luabind::constructor<vector<t>::size_type,vector<t>::const_reference>())	\
	.def("push_back",(void(vector<t>::*)(vector<t>::const_reference))&vector<t>::push_back)	\
	.def("at",&get_vector_##t) \
	.def("set",&set_vector_##t) \
	.def("clear",&vector<t>::clear)	\
	.def("reserve",&vector<t>::reserve)	\
	.def("resize",(void(vector<t>::*)(vector<t>::size_type)) &vector<t>::resize)	\
	.def("resize",(void(vector<t>::*)(vector<t>::size_type,t)) &vector<t>::resize)	\
	.def("size",&vector<t>::size)

void InitializeLuaStd(lua_State *L)
{
	luabind::module(L)
	[
		LUABIND_VECTOR(vec4,"vector_vec4"),		
		LUABIND_VECTOR(vec3,"vector_vec3"),
		LUABIND_VECTOR(vec2,"vector_vec2"),
		LUABIND_VECTOR(float,"vector_float"),
		LUABIND_VECTOR(int,"vector_int"),
		LUABIND_VECTOR(DWORD,"vector_uint"),

		LUABIND_VECTOR(Face,"vector_Face"),
		LUABIND_VECTOR(Mesh,"vector_Mesh"),
		LUABIND_VECTOR(Buffer,"vector_Buffer"),
		LUABIND_VECTOR_NAMED(vector<vec2>,"vector_vector_vec2",set_vector_vector_vec2,get_vector_vector_vec2),
		LUABIND_VECTOR(Object,"vector_Object"),
		LUABIND_VECTOR(Material,"vector_Material")
	];
}