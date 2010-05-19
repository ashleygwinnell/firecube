#include <string>
#include <vector>
#include <sstream>
#include <map>
#include <boost/shared_array.hpp>
#include <boost/weak_ptr.hpp>
#include <iostream>
#include <queue>
using namespace std;
#include <windows.h>
#include <gl/gl.h>
#include <FireCube.h>
using namespace FireCube;
#include "ShaderGenerator.h"

RenderState::RenderState() : directionalLighting(false), diffuseTexture(false), fog(false)
{

}
void RenderState::FromMaterial(Material mat)
{
	if (mat->diffuseTexture)
		this->diffuseTexture=true;
}
void RenderState::SetDirectionalLighting(bool enable)
{
	directionalLighting=enable;
}
void RenderState::SetFog(bool enable)
{
	fog=enable;
}
DWORD RenderState::ToInt()
{
	DWORD ret=0;
	if (directionalLighting)
		ret|=1;
	if (fog)
		ret|=2;
	if (diffuseTexture)
		ret|=4;
	return ret;
}
Program ShaderGenerator::GenerateProgram(RenderState &renderState)
{
	map<DWORD,Program>::iterator i=programs.find(renderState.ToInt());
	if (i!=programs.end())
		return i->second;
	
	Program program(new ProgramResource);
	Shader vshader(new ShaderResource);
	ostringstream vshaderCode;
	vshaderCode << "varying vec3 normal;" << endl
				<<"void  main()" << endl
				<< "{" << endl;
	if (renderState.diffuseTexture)
		vshaderCode << "gl_TexCoord[0]=gl_MultiTexCoord0;" << endl;
	vshaderCode << "normal = gl_NormalMatrix * gl_Normal;" << endl;
	vshaderCode	<< "gl_Position = ftransform();" << endl
				<< "}";
	Shader fshader(new ShaderResource);
	ostringstream fshaderCode;	
	fshaderCode << "varying vec3 normal;" << endl;
	if (renderState.directionalLighting)
		fshaderCode << "uniform vec4 lightDiffuse;" << endl;
	if (renderState.diffuseTexture)
		fshaderCode << "uniform sampler2D diffuseMap;" << endl;
	if (renderState.directionalLighting)
		fshaderCode << "uniform vec3 lightDir;" << endl;
	if (renderState.fog)
	{
		fshaderCode << "uniform float fogDensity;" << endl
					<< "uniform vec4 fogColor;" << endl;
	}
	fshaderCode << "void main()" << endl
				<< "{" << endl
				<< "vec4 color=gl_FrontMaterial.diffuse;" << endl;
	if (renderState.diffuseTexture)
		fshaderCode << "color*=texture2D(diffuseMap,gl_TexCoord[0].xy);" << endl;
	if (renderState.directionalLighting)
	{
		fshaderCode << "vec3 n=normalize(normal);" << endl
					<< "lightDir=normalize(lightDir);" << endl
					<< "float d=max(dot(n,lightDir),0.0);" << endl
					<< "color *= lightDiffuse*d;" << endl;
	}
	if (renderState.fog)
	{
		fshaderCode << "const float LOG2 = 1.442695;" << endl
					<< "float z = gl_FragCoord.z / gl_FragCoord.w;" << endl
					<< "float fogFactor = exp2( -fogDensity* fogDensity* z * z * LOG2 );" << endl
					<< "fogFactor = clamp(fogFactor, 0.0, 1.0);" << endl
					<< "color = mix(fogColor, color, fogFactor );" << endl;
	}
	fshaderCode << "gl_FragColor=color;" << endl
				<< "}" << endl;
	vshader->Create(VERTEX_SHADER,vshaderCode.str());
	fshader->Create(FRAGMENT_SHADER,fshaderCode.str());
	program->Create(vshader,fshader);
	programs[renderState.ToInt()]=program;
	return program;
}