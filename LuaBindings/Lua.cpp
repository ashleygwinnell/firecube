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

void InitializeLuaStd(lua_State *L);

class Dummy{};
class Dummy2{};
class Dummy3{};
class Dummy4{};
class Dummy5{};
class Dummy6{};
class Dummy7{};
void SetValue(Face &f,DWORD i,DWORD v)
{
	f.v[i]=v;
}
DWORD GetValue(Face &f,DWORD i)
{
	return f.v[i];
}
void SetValue(Edge &e,DWORD i,DWORD v)
{
	e.v[i]=v;
}
DWORD GetValue(Edge &e,DWORD i)
{
	return e.v[i];
}

void SetValue(mat4 &m,DWORD i,float v)
{
	m.m[i]=v;
}
float GetValue(mat4 &m,DWORD i)
{
	return m.m[i];
}
void SetValue(mat3 &m,DWORD i,float v)
{
	m.m[i]=v;
}
float GetValue(mat3 &m,DWORD i)
{
	return m.m[i];
}

class ApplicationWrapper : public Application,public luabind::wrap_base
{
public:
	ApplicationWrapper() : Application() 
	{

	}
	virtual void Render(float t) 
	{ 
		call<void>("Render", t); 
	}
	static void DefaultRender(Application* ptr, float t){}
	virtual void Update(float t) 
	{ 
		call<void>("Update", t); 
	}
	static void DefaultUpdate(Application* ptr, float t){}
	virtual void HandleInput(float t) 
	{ 
		call<void>("HandleInput", t); 
	}
	static void DefaultHandleInput(Application* ptr, float t){}
	virtual bool Init() 
	{ 				
		return call<bool>("Init");;
	}
	static bool DefaultInit(Application* ptr){return false;}
};
void LoadData(Buffer b,vector<vec4> &v,BufferType type)
{
	b->LoadData(&v[0],sizeof(vec4)*v.size(),type);
}
void LoadData(Buffer b,luabind::object const &t,BufferType type)
{
	vector<vec3> vv;
	if (luabind::type(t) == LUA_TTABLE)
	{
		for (luabind::iterator i(t), end; i != end; ++i)
		{
			vec3 v=luabind::object_cast<vec3>(*i);
			vv.push_back(v);
		}		
		b->LoadData(&vv[0],sizeof(vec3)*vv.size(),type);
	}	
}
void LoadData(Buffer b,vector<vec2> &v,BufferType type)
{
	b->LoadData(&v[0],sizeof(vec2)*v.size(),type);
}
void LoadData(Buffer b,vector<float> &v,BufferType type)
{
	b->LoadData(&v[0],sizeof(float)*v.size(),type);
}
void LoadData(Buffer b,vector<int> &v,BufferType type)
{
	b->LoadData(&v[0],sizeof(int)*v.size(),type);
}
void LoadData(Buffer b,vector<DWORD> &v,BufferType type)
{
	b->LoadData(&v[0],sizeof(DWORD)*v.size(),type);
}
void LoadIndexData(Buffer b,luabind::object const &t,BufferType type)
{
	vector<DWORD> vv;
	if (luabind::type(t) == LUA_TTABLE)
	{
		for (luabind::iterator i(t), end; i != end; ++i)
		{
			DWORD v=luabind::object_cast<DWORD>(*i);
			vv.push_back(v);
		}		
		b->LoadIndexData(&vv[0],vv.size(),type);
	}
	
}
void InitializeLua(lua_State *& luaState)
{
	luabind::open(luaState);

	luabind::module(luaState)
	[				
		luabind::def("LoadMesh",&LoadMesh),
		luabind::def("IsFinite",&IsFinite),
		luabind::def("RangedRandom",&RangedRandom),
		luabind::def("Cross",(float (*)(vec2, vec2))&FireCube::Cross),
		luabind::def("Dot",(float (*)(vec2, vec2))&FireCube::Dot),
		luabind::def("Cross",(vec3 (*)(vec3, vec3))&FireCube::Cross),
		luabind::def("Dot",(float (*)(vec3, vec3))&FireCube::Dot),
		luabind::def("Cross",(vec4 (*)(vec4, vec4))&FireCube::Cross),
		luabind::def("Dot",(float (*)(vec4, vec4))&FireCube::Dot),		
		luabind::class_<mat4>("mat4")
			.def(luabind::constructor<>())			
			.def(luabind::self + luabind::other<mat4>())
			.def(luabind::self - luabind::other<mat4>())
			.def(luabind::self * float())
			.def(luabind::self * luabind::other<mat4>())
			.def(luabind::self * luabind::other<vec3>())
			.def(luabind::self * luabind::other<vec4>())
			.def("GetValue",(float (*)(mat4 &,DWORD))&GetValue)
			.def("SetValue",(void (*)(mat4 &,DWORD,float))&SetValue)
			.def("Identity",&mat4::Identity)
			.def("Translate",(void(mat4::*)(vec3))&mat4::Translate)
			.def("Translate",(void(mat4::*)(float,float,float))&mat4::Translate)
			.def("Scale",&mat4::Scale)
			.def("RotateX",&mat4::RotateX)
			.def("RotateY",&mat4::RotateY)
			.def("RotateZ",&mat4::RotateZ)
			.def("Rotate",&mat4::Rotate)			
			.def("Transpose",&mat4::Transpose)
			.def("Inverse",&mat4::Inverse)
			.def("GetPos",&mat4::GetPos)
			.def("GetDir",&mat4::GetDir)
			.def("GeneratePerspective",&mat4::GeneratePerspective)
			.def("GenerateOrthographic",&mat4::GenerateOrthographic)
			.def("LookAt",&mat4::LookAt)
			.def("ToMatrix3",&mat4::ToMatrix3),
		luabind::class_<mat3>("mat3")
			.def(luabind::constructor<>())			
			.def(luabind::self + luabind::other<mat3>())
			.def(luabind::self - luabind::other<mat3>())
			.def(luabind::self * float())
			.def(luabind::self * luabind::other<mat3>())
			.def("GetValue",(float (*)(mat3 &,DWORD))&GetValue)
			.def("SetValue",(void (*)(mat3 &,DWORD,float))&SetValue)
			.def("Identity",&mat3::Identity)			
			.def("Scale",&mat3::Scale)
			.def("RotateX",&mat3::RotateX)
			.def("RotateY",&mat3::RotateY)
			.def("RotateZ",&mat3::RotateZ)
			.def("Transpose",&mat3::Transpose)
			.def("Inverse",&mat3::Inverse)
			.def("GetDir",&mat3::GetDir)
			.def("ToMatrix4",&mat3::ToMatrix4)
			.def("Orthonormalize",&mat3::Orthonormalize)
			.def("Sensible",&mat3::Sensible)
			.def("GetCol",&mat3::GetCol)
			.def("SetCol",&mat3::SetCol),
		luabind::class_<vec4>("vec4")
			.def(luabind::constructor<>())
			.def(luabind::constructor<const vec4 &>())
			.def(luabind::constructor<float,float,float,float>())
			.def(luabind::self + luabind::other<vec4>())
			.def(luabind::self - luabind::other<vec4>())
			.def(luabind::self * float())
			.def("Clear",&vec4::Clear)
			.def("Normalize",&vec4::Normalize)
			.def("Length",&vec4::Length)
			.def("Length2",&vec4::Length2)
			.def("Dot",&vec4::Dot)
			.def("Cross",&vec4::Cross)
			.def("Set",(void(vec4::*)(float,float,float,float))&vec4::Set)
			.def("Set",(void(vec4::*)(float))&vec4::Set)
			.def("ToVec2",&vec4::ToVec2)
			.def("ToVec3",&vec4::ToVec3)
			.def("SetLength",&vec4::SetLength)
			.def("RotateX",&vec4::RotateX)
			.def("RotateY",&vec4::RotateY)
			.def("RotateZ",&vec4::RotateZ)
			.def("FromAngles",&vec4::FromAngles)
			.def("Sensible",&vec4::Sensible)
			.def_readwrite("x", &vec4::x)
			.def_readwrite("y", &vec4::y)
			.def_readwrite("z", &vec4::z)
			.def_readwrite("w", &vec4::w),
		luabind::class_<vec3 >("vec3")
			.def(luabind::constructor<>())
			.def(luabind::constructor<const vec3 &>())
			.def(luabind::constructor<float,float,float>())
			.def(luabind::self + luabind::other<vec3>())
			.def(luabind::self - luabind::other<vec3>())
			.def(luabind::self * float())
			.def("Clear",&vec3::Clear)
			.def("Normalize",&vec3::Normalize)
			.def("Length",&vec3::Length)
			.def("Length2",&vec3::Length2)
			.def("Dot",&vec3::Dot)
			.def("Cross",&vec3::Cross)
			.def("Set",(void(vec3::*)(float,float,float))&vec3::Set)
			.def("Set",(void(vec3::*)(float))&vec3::Set)
			.def("ToVec2",&vec3::ToVec2)
			.def("ToVec4",&vec3::ToVec4)
			.def("SetLength",&vec3::SetLength)
			.def("RotateX",&vec3::RotateX)
			.def("RotateY",&vec3::RotateY)
			.def("RotateZ",&vec3::RotateZ)
			.def("FromAngles",&vec3::FromAngles)
			.def("TransformCoordinate",&vec3::TransformCoordinate)
			.def("TransformNormal",&vec3::TransformNormal)
			.def("Sensible",&vec3::Sensible)
			.def_readwrite("x", &vec3::x)
			.def_readwrite("y", &vec3::y)
			.def_readwrite("z", &vec3::z),
		luabind::class_<vec2 >("vec2")
			.def(luabind::constructor<>())
			.def(luabind::constructor<const vec2 &>())
			.def(luabind::constructor<float,float>())
			.def(luabind::self + luabind::other<vec2>())
			.def(luabind::self - luabind::other<vec2>())
			.def(luabind::self * float())
			.def("Clear",&vec2::Clear)
			.def("Normalize",&vec2::Normalize)
			.def("Length",&vec2::Length)
			.def("Length2",&vec2::Length2)
			.def("Dot",&vec2::Dot)
			.def("Cross",&vec2::Cross)
			.def("Set",(void(vec2::*)(float,float))&vec2::Set)
			.def("Set",(void(vec2::*)(float))&vec2::Set)
			.def("ToVec3",&vec2::ToVec3)
			.def("ToVec4",&vec2::ToVec4)
			.def("SetLength",&vec2::SetLength)
			.def("Rotate",&vec2::Rotate)
			.def("FromAngle",&vec2::FromAngle)
			.def("TransformCoordinate",&vec2::TransformCoordinate)
			.def("TransformNormal",&vec2::TransformNormal)
			.def("Sensible",&vec2::Sensible)			
			.def_readwrite("x", &vec2::x)
			.def_readwrite("y", &vec2::y),
		luabind::class_<Timer>("Timer")
			.def("Init",&Timer::Init)
			.def("Update",&Timer::Update)
			.def("Passed",&Timer::Passed),
		luabind::class_<MaterialResource,Material>("Material")
			.def(luabind::constructor<>())
			.def_readwrite("diffuseTexture",&MaterialResource::diffuseTexture)
			.def_readwrite("name",&MaterialResource::name)
			.def_readwrite("ambient",&MaterialResource::ambient)
			.def_readwrite("diffuse",&MaterialResource::diffuse)
			.def_readwrite("specular",&MaterialResource::specular)
			.def_readwrite("shininess",&MaterialResource::shininess),			
		luabind::class_<Edge>("Edge")
			.def(luabind::constructor<>())
			.def("GetValue",(DWORD (*)(Edge &,DWORD))&GetValue)
			.def("SetValue",(void (*)(Edge &,DWORD,DWORD))&SetValue),
		luabind::class_<Face>("Face")
			.def(luabind::constructor<>())
			.def(luabind::constructor<DWORD,DWORD,DWORD>())
			.def("GetValue",(DWORD (*)(Face &,DWORD))&GetValue)
			.def("SetValue",(void (*)(Face &,DWORD,DWORD))&SetValue)
			.def_readwrite("normal",&Face::normal),
		luabind::class_<Surface>("Surface")
			.def(luabind::constructor<>())
			.def_readwrite("material",&Surface::material)
			.def_readwrite("face",&Surface::face)
			.def_readwrite("indexBuffer",&Surface::indexBuffer),		
		luabind::class_<RenderParameters>("RenderParameters")
			.def(luabind::constructor<>())
			.def_readwrite("fog",&RenderParameters::fog)
			.def_readwrite("fogColor",&RenderParameters::fogColor)
			.def_readwrite("fogDensity",&RenderParameters::fogDensity)
			.def_readwrite("lighting",&RenderParameters::lighting)
			.def_readwrite("program",&RenderParameters::program),
		luabind::class_<LightResource,Light>("Light")
			.def(luabind::constructor<>())
			.def_readwrite("ambientColor",&LightResource::ambientColor)
			.def_readwrite("diffuseColor",&LightResource::diffuseColor)			
			.def_readwrite("specularColor",&LightResource::specularColor)
			.def_readwrite("type",&LightResource::type),
		luabind::class_<Node>("Node")
			.def(luabind::constructor<>())
			.def(luabind::constructor<const string &>())
			.def(luabind::constructor<Node>())			
			.def("AddChild",&Node::AddChild)
			.def("AddGeometry",&Node::AddGeometry)
			.def("AddLight",&Node::AddLight)
			.def("CreateHardNormals",&Node::CreateHardNormals)
			.def("GetChild",&Node::GetChild)
			.def("GetChildren",&Node::GetChildren)
			.def("GetGeometries",&Node::GetGeometries)
			.def("GetLighting",&Node::GetLighting)
			.def("GetLights",&Node::GetLights)
			.def("GetLocalTransformation",&Node::GetLocalTransformation)
			.def("GetMatrixTransformation",&Node::GetMatrixTransformation)
			.def("GetName",&Node::GetName)
			.def("GetProgram",&Node::GetProgram)
			.def("GetRenderParameters",&Node::GetRenderParameters)
			.def("GetRotation",&Node::GetRotation)
			.def("GetScale",&Node::GetScale)
			.def("GetTranslation",&Node::GetTranslation)
			.def("GetWorldTransformation",&Node::GetWorldTransformation)
			.def("Move",&Node::Move)
			.def("Parent",&Node::Parent)
			.def("RemoveChild",(Node(Node::*)(const string&))&Node::RemoveChild)
			.def("RemoveChild",(Node(Node::*)(Node))&Node::RemoveChild)
			.def("RemoveLight",&Node::RemoveLight)
			.def("Render",&Node::Render)
			.def("RenderBoundingBox",&Node::RenderBoundingBox)
			.def("Rotate",&Node::Rotate)
			.def("Scale",&Node::Scale)
			.def("SetLighting",&Node::SetLighting)
			.def("SetMatrixTransformation",&Node::SetMatrixTransformation)
			.def("SetName",&Node::SetName)
			.def("SetParent",&Node::SetParent)
			.def("SetProgram",&Node::SetProgram)
			.def("SetRotation",&Node::SetRotation)
			.def("SetScale",&Node::SetScale)
			.def("SetTranslation",&Node::SetTranslation)
			.def("SetWorldTransformation",&Node::SetWorldTransformation)
			.def("UpdateBoundingBox",&Node::UpdateBoundingBox),			
		luabind::class_<GeometryResource, Geometry >("Geometry")
			.def(luabind::constructor<>())			
			//.def("Load",&ModelResource::Load)
			.def("Reduce",&GeometryResource::Reduce)
			.def("CalculateNormals",&GeometryResource::CalculateNormals)
			.def("ApplyTransformation",&GeometryResource::ApplyTransformation)			
			.def("CreateHardNormals",&GeometryResource::CreateHardNormals)
			.def("UpdateBuffers",&GeometryResource::UpdateBuffers)
			.def("GetMaterialByName",&GeometryResource::GetMaterialByName)			
			.def_readwrite("material",&GeometryResource::material)						
			.def_readwrite("vertex",&GeometryResource::vertex)
			.def_readwrite("normal",&GeometryResource::normal)
			.def_readwrite("mesh",&GeometryResource::surface)
			.def_readwrite("face",&GeometryResource::face)
			.def_readwrite("surface",&GeometryResource::surface)
			.def_readwrite("diffuseUV",&GeometryResource::diffuseUV)
			.def_readwrite("vertexBuffer",&GeometryResource::vertexBuffer)
			.def_readwrite("diffuseUVBuffer",&GeometryResource::diffuseUVBuffer)
			.def_readwrite("normalBuffer",&GeometryResource::normalBuffer),
		luabind::class_<TextureResource,Texture>("Texture")			
			.def(luabind::constructor<>())
			.def("Load",&TextureResource::Load)
			.def("IsValid",&TextureResource::IsValid)
			.def("Create",&TextureResource::Create)
			.def("GenerateMipMaps",&TextureResource::GenerateMipMaps)
			.def("SetFiltering",&TextureResource::SetFiltering)
			.def_readwrite("id", &TextureResource::id)
			.def_readwrite("filename", &TextureResource::filename),
		luabind::class_<FrameBufferResource,FrameBuffer>("FrameBuffer")
			.def(luabind::constructor<>())
			.def("Create",&FrameBufferResource::Create)
			.def("AddDepthBuffer",&FrameBufferResource::AddDepthBuffer)
			.def("AddDepthBufferTexture",&FrameBufferResource::AddDepthBufferTexture)
			.def("AddRenderTarget",&FrameBufferResource::AddRenderTarget)
			.def("SetRenderTarget",&FrameBufferResource::SetRenderTarget)
			.def("GetRenderTarget",&FrameBufferResource::GetRenderTarget)
			.def("GetDepthBuffer",&FrameBufferResource::GetDepthBuffer)
			.def("IsValid",&FrameBufferResource::IsValid)
			.def("GetWidth",&FrameBufferResource::GetWidth)
			.def("GetHeight",&FrameBufferResource::GetHeight),			
		luabind::class_<Image>("Image")
			.def(luabind::constructor<>())
			.def("Load",&Image::Load)
			.def("GetWidth",&Image::GetWidth)
			.def("GetHeight",&Image::GetHeight)
			.def("GetBPP",&Image::GetBPP)
			.def("GetPixel",&Image::GetPixel)
			.def("GetPixels",&Image::GetPixels),
		luabind::class_<BufferResource,Buffer>("Buffer")			
			.def(luabind::constructor<>())
			.def("Create",&BufferResource::Create)
			.def("Destroy",&BufferResource::Destroy)
			.def("IsValid",&BufferResource::IsValid)			
			//.def("LoadData",(void (*)(Buffer, vector<vec4>&,BufferType))&::LoadData)
			.def("LoadData",(void (*)(Buffer, luabind::object const &,BufferType))&::LoadData)
			//.def("LoadData",(void (*)(Buffer, vector<vec2>&,BufferType))&::LoadData)
			//.def("LoadData",(void (*)(Buffer, vector<float>&,BufferType))&::LoadData)
			//.def("LoadData",(void (*)(Buffer, vector<int>&,BufferType))&::LoadData)
			//.def("LoadData",(void (*)(Buffer, vector<DWORD>&,BufferType))&::LoadData)
			.def("LoadIndexData",(void (*)(Buffer,luabind::object const&,BufferType))&::LoadIndexData)
			.def("SetColorStream",&BufferResource::SetColorStream)
			.def("SetIndexStream",&BufferResource::SetIndexStream)
			.def("SetNormalStream",&BufferResource::SetNormalStream)
			.def("SetTexCoordStream",&BufferResource::SetTexCoordStream)
			.def("SetVertexStream",&BufferResource::SetVertexStream),			
		luabind::class_<ProgramResource,Program>("Program")
			.def(luabind::constructor<>())
			.def("Create",(void(ProgramResource::*)(Shader,Shader))&ProgramResource::Create)
			.def("Create",(void(ProgramResource::*)())&ProgramResource::Create)
			.def("Attach",&ProgramResource::Attach)
			.def("GetInfoLog",&ProgramResource::GetInfoLog)
			.def("IsValid",&ProgramResource::IsValid)
			.def("Link",&ProgramResource::Link)
			.def("SetUniform",(void(ProgramResource::*)(const string&,bool))&ProgramResource::SetUniform)
			.def("SetUniform",(void(ProgramResource::*)(const string&,vec4))&ProgramResource::SetUniform)
			.def("SetUniform",(void(ProgramResource::*)(const string&,vec3))&ProgramResource::SetUniform)
			.def("SetUniformi",(void(ProgramResource::*)(const string&,int))&ProgramResource::SetUniform)
			.def("SetUniform",(void(ProgramResource::*)(const string&,float))&ProgramResource::SetUniform),		
		luabind::class_<ShaderResource,Shader>("Shader")			
			.def(luabind::constructor<>())
			.def("Create",&ShaderResource::Create)
			.def("Load",&ShaderResource::Load),		
		luabind::class_<FontResource,Font>("Font")			
			.def(luabind::constructor<>())
			.def("Load",(bool(FontResource::*)(const string&))&FontResource::Load)
			.def("Load",(bool(FontResource::*)(const string&,int))&FontResource::Load),
		luabind::class_<FontManager>("FontManager")
			.def(luabind::constructor<>())
			.def("Add",(Font(FontManager::*)(const string&))&FontManager::Add)
			.def("Add",(void(FontManager::*)(const string&,Font))&FontManager::Add)
			.def("Create",(Font(FontManager::*)(const string&,int))&FontManager::Create)
			.def("Create",(Font(FontManager::*)(const string&))&FontManager::Create)
			.def("Get",&FontManager::Get),
		luabind::class_<TextureManager>("TextureManager")
			.def(luabind::constructor<>())
			.def("Add",(Texture(TextureManager::*)(const string&))&TextureManager::Add)
			.def("Add",(void(TextureManager::*)(const string&,Texture))&TextureManager::Add)
			.def("Create",&TextureManager::Create)
			.def("Get",&TextureManager::Get),
		luabind::class_<ShaderManager>("ShaderManager")
			.def(luabind::constructor<>())
			.def("Add",(Shader(ShaderManager::*)(const string&))&ShaderManager::Add)
			.def("Add",(void(ShaderManager::*)(const string&,Shader))&ShaderManager::Add)
			.def("Create",&ShaderManager::Create)
			.def("Get",&ShaderManager::Get),
		luabind::class_<Event>("Event")
			.def(luabind::constructor<>())
			.def_readwrite("type",&Event::type)
			.def_readwrite("key",&Event::key)
			.def_readwrite("mouseX",&Event::mouseX)
			.def_readwrite("mouseY",&Event::mouseY),
		luabind::class_<Application,ApplicationWrapper>("Application")
			.def(luabind::constructor<>())
			.def("Initialize",(bool(Application::*)())&Application::Initialize)
			.def("Initialize",(bool(Application::*)(int,int,int,int,bool))&Application::Initialize)
			.def("InitializeNoWindow",&Application::InitializeNoWindow)
			.def("Destroy",&Application::Destroy)
			.def("Run",&Application::Run)
			.def("SetTitle",&Application::SetTitle)
			.def("GetFps",&Application::GetFps)
			.def("GetWidth",&Application::GetWidth)
			.def("GetHeight",&Application::GetHeight)
			.def("HasMoreEvents",&Application::HasMoreEvents)
			.def("GetEvent",&Application::GetEvent)
			.def("Render", &Application::Render, &ApplicationWrapper::DefaultRender)
			.def("Update", &Application::Update, &ApplicationWrapper::DefaultUpdate)
			.def("HandleInput", &Application::HandleInput, &ApplicationWrapper::DefaultHandleInput)
			.def("Init", &Application::Init, &ApplicationWrapper::DefaultInit)
			.scope
			[				
				luabind::def("AddSearchPath", &Application::AddSearchPath)
			],
		luabind::class_<Dummy>("RenderMode")
			.enum_("RenderMode")
			[
				luabind::value("POINTS", FireCube::POINTS),
				luabind::value("LINES", LINES),
				luabind::value("TRIANGLES", TRIANGLES),
				luabind::value("TRIANGLE_STRIP", TRIANGLE_STRIP),
				luabind::value("QUADS", QUADS),
				luabind::value("LINE_LOOP", LINE_LOOP),
				luabind::value("TRIANGLE_FAN", TRIANGLE_FAN)				
			],
		luabind::class_<Dummy2>("BufferType")
			.enum_("BufferType")
			[
				luabind::value("STATIC", STATIC),
				luabind::value("DYNAMIC", DYNAMIC),
				luabind::value("STREAM", STREAM)				
			],
		luabind::class_<Dummy3>("TextureFilter")
			.enum_("TextureFilter")
			[
				luabind::value("NEAREST", NEAREST),
				luabind::value("LINEAR", LINEAR),
				luabind::value("MIPMAP", MIPMAP)
			],
		luabind::class_<Dummy4>("ShaderType")
			.enum_("ShaderType")
			[
				luabind::value("VERTEX_SHADER", VERTEX_SHADER),
				luabind::value("FRAGMENT_SHADER", FRAGMENT_SHADER)			
			],
		luabind::class_<Dummy5>("EventType")
			.enum_("EventType")
			[
				luabind::value("MOUSE_MOVE", MOUSE_MOVE),
				luabind::value("MOUSE_CLICK", MOUSE_CLICK),
				luabind::value("KEY_PRESSED", KEY_PRESSED),
				luabind::value("KEY_DOWN", KEY_DOWN),
				luabind::value("KEY_UP", KEY_UP)
			],
		luabind::class_<Dummy6>("Key")
			.enum_("Key")
			[
				luabind::value("KEY_UP_ARROW", KEY_UP_ARROW),
				luabind::value("KEY_DOWN_ARROW", KEY_DOWN_ARROW),
				luabind::value("KEY_LEFT_ARROW", KEY_LEFT_ARROW),
				luabind::value("KEY_RIGHT_ARROW", KEY_RIGHT_ARROW),
				luabind::value("KEY_SPACE", KEY_SPACE)
			],
		luabind::class_<Dummy7>("LightType")
			.enum_("LightType")
			[
				luabind::value("DIRECTIONAL", DIRECTIONAL),
				luabind::value("POINT", FireCube::POINT)
			],
		luabind::namespace_("Renderer")
		[			
			luabind::def("Clear",&Renderer::Clear),
			//luabind::def("Render",(void(*)(Geometry)) &Renderer::Render),
			luabind::def("Render",(void(*)(Node)) &Renderer::Render),
			luabind::def("SetModelViewMatrix",&Renderer::SetModelViewMatrix),
			luabind::def("SetProjectionMatrix",&Renderer::SetProjectionMatrix),
			luabind::def("SetTextureMatrix",&Renderer::SetTextureMatrix),
			luabind::def("GetModelViewMatrix",&Renderer::GetModelViewMatrix),
			luabind::def("GetProjectionMatrix",&Renderer::GetProjectionMatrix),
			luabind::def("SaveModelViewMatrix",&Renderer::SaveModelViewMatrix),
			luabind::def("RestoreModelViewMatrix",&Renderer::RestoreModelViewMatrix),
			luabind::def("SaveProjectionMatrix",&Renderer::SaveProjectionMatrix),
			luabind::def("RestoreProjectionMatrix",&Renderer::RestoreProjectionMatrix),
			luabind::def("MultiplyModelViewMatrix",&Renderer::MultiplyModelViewMatrix),
			luabind::def("UseTexture",&Renderer::UseTexture),
			luabind::def("RenderText",&Renderer::RenderText),
			luabind::def("RenderIndexStream",&Renderer::RenderIndexStream),
			luabind::def("RenderStream",&Renderer::RenderStream),
			luabind::def("DisableTexCoordStream",&Renderer::DisableTexCoordStream),
			luabind::def("DisableNormalStream",&Renderer::DisableNormalStream),
			luabind::def("UseProgram",&Renderer::UseProgram),
			luabind::def("UseMaterial",&Renderer::UseMaterial),
			luabind::def("UseFrameBuffer",&Renderer::UseFrameBuffer),
			luabind::def("RestoreFrameBuffer",&Renderer::RestoreFrameBuffer),
			luabind::def("SetViewport",&Renderer::SetViewport),
			luabind::def("SetGlobalProgram",&Renderer::SetGlobalProgram),
			luabind::def("SetPerspectiveProjection",&Renderer::SetPerspectiveProjection),
			luabind::def("SetOrthographicProjection",&Renderer::SetOrthographicProjection),
			luabind::def("SetTextureManager",&Renderer::SetTextureManager),
			luabind::def("GetTextureManager",&Renderer::GetTextureManager),
			luabind::def("SetShaderManager",&Renderer::SetShaderManager),
			luabind::def("GetShaderManager",&Renderer::GetShaderManager),
			luabind::def("SetFontManager",&Renderer::SetFontManager),
			luabind::def("GetFontManager",&Renderer::GetFontManager)
		],
		luabind::namespace_("Logger")
		[
			luabind::def("Init",&Logger::Init),
			luabind::def("Write",&Logger::Write),
			luabind::def("Close",&Logger::Close)
		]
	];
	luabind::globals(luaState)["MAX_TEXTURES"]=MAX_TEXTURES;
	InitializeLuaStd(luaState);
}