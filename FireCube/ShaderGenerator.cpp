#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <map>
#include <queue>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
using namespace std;
#include <SDL.h>
#include <SDL_image.h>
#include <windows.h>
#include "GLee.h"
#include <GL/GLU.h>
#include "utils.h"	
#include "Logger.h"
#include "ResourceManager.h"
#include "Timer.h"
#include "MyMath.h"	
#include "BoundingBox.h"
#include "Texture.h"		
#include "Buffer.h"
#include "Shaders.h"
#include "Geometry.h"	
#include "FrameBuffer.h"
#include "Image.h"
#include "Font.h"
#include "ShaderGenerator.h"
#include "Renderer.h"				
#include "Application.h"

using namespace FireCube;
RenderState::RenderState() : directionalLighting(false), diffuseTexture(false), fog(false),pointLighting(false)
{

}
void RenderState::FromMaterial(Material mat)
{
	if (mat.GetDiffuseTexture())
		this->diffuseTexture=true;
}
void RenderState::SetDirectionalLighting(bool enable)
{
	directionalLighting=enable;
}
void RenderState::SetPointLighting(bool enable)
{
	pointLighting=enable;
}

void RenderState::SetFog(bool enable)
{
	fog=enable;
}
unsigned int RenderState::ToInt()
{
	unsigned int ret=0;
	if (directionalLighting)
		ret|=1;
	if (fog)
		ret|=2;
	if (diffuseTexture)
		ret|=4;
	if (pointLighting)
		ret|=8;
	return ret;
}
Program ShaderGenerator::GenerateProgram(RenderState &renderState)
{
	map<unsigned int,Program>::iterator i=programs.find(renderState.ToInt());
	if (i!=programs.end())
		return i->second;
	
	Program program;
	Shader vshader;
	ostringstream vshaderCode;
	if (renderState.pointLighting)
	{
		vshaderCode << "varying vec3 lightDir;" << endl
					<< "varying vec3 eyeVec;" << endl
					<< "uniform vec3 lightPosition;" << endl;
	}
	if (renderState.directionalLighting)
	{
		vshaderCode << "varying vec3 eyeVec;" << endl;					
	}
	vshaderCode << "varying vec3 normal;" << endl
				<<"void  main()" << endl
				<< "{" << endl;
	if (renderState.diffuseTexture)
		vshaderCode << "gl_TexCoord[0]=gl_MultiTexCoord0;" << endl;
	if (renderState.pointLighting)
	{
		vshaderCode << "vec3 vVertex = vec3(gl_ModelViewMatrix * gl_Vertex);" << endl
					<< "lightDir= vec3(lightPosition - vVertex);" << endl
					<< "eyeVec = -vVertex;" << endl;
	}
	if (renderState.directionalLighting)
	{
		vshaderCode << "eyeVec = -vec3(gl_ModelViewMatrix * gl_Vertex);" << endl;					
	}
	vshaderCode << "normal = gl_NormalMatrix * gl_Normal;" << endl;
	vshaderCode	<< "gl_Position = ftransform();" << endl
				<< "}";
	Shader fshader;
	ostringstream fshaderCode;	
	fshaderCode << "varying vec3 normal;" << endl;
	if (renderState.directionalLighting || renderState.pointLighting)
	{
		fshaderCode << "uniform vec4 lightAmbient;" << endl
					<< "uniform vec4 lightDiffuse;" << endl
					<< "uniform vec4 lightSpecular;" << endl;
	}
	if (renderState.diffuseTexture)
		fshaderCode << "uniform sampler2D diffuseMap;" << endl;
	if (renderState.directionalLighting)
	{
		fshaderCode << "uniform vec3 directionalLightDir;" << endl
					<< "varying vec3 eyeVec;" << endl;
	}
	if (renderState.pointLighting)
	{
		fshaderCode << "varying vec3 lightDir;" << endl
					<< "varying vec3 eyeVec;" << endl;
	}
	if (renderState.fog)
	{
		fshaderCode << "uniform float fogDensity;" << endl
					<< "uniform vec4 fogColor;" << endl;
	}
	fshaderCode << "void main()" << endl
				<< "{" << endl;
				
	if (!renderState.directionalLighting && !renderState.pointLighting)
	{
	if (renderState.diffuseTexture)
	fshaderCode << "vec4 color=gl_FrontMaterial.diffuse*texture2D(diffuseMap,gl_TexCoord[0].xy);" << endl;
	else
	fshaderCode << "vec4 color=gl_FrontMaterial.diffuse;" << endl;
	}
	else
	fshaderCode << "vec4 color=lightAmbient * gl_FrontMaterial.ambient;" << endl;
	if (renderState.directionalLighting)
	{
		fshaderCode << "vec3 n=normalize(normal);" << endl					
					<< "directionalLightDir=normalize(directionalLightDir);" << endl
					<< "float lambertTerm=dot(n,directionalLightDir);" << endl
					<< "if (lambertTerm>0.0)" << endl
					<< "{" << endl;
		if (renderState.diffuseTexture)
		fshaderCode << "color += gl_FrontMaterial.diffuse*lightDiffuse*lambertTerm*texture2D(diffuseMap,gl_TexCoord[0].xy);" << endl;
		else
		fshaderCode << "color += gl_FrontMaterial.diffuse*lightDiffuse*lambertTerm;" << endl;
		fshaderCode << "vec3 E = normalize(eyeVec);" << endl
					<< "vec3 R = reflect(-directionalLightDir, n);" << endl
					<< "float specular = max(pow( max(dot(R, E), 0.0), gl_FrontMaterial.shininess ),0.0);" << endl
					<< "color += gl_FrontMaterial.specular*lightSpecular*specular;" << endl
					<< "}" << endl;
	}
	if (renderState.pointLighting)
	{
		fshaderCode << "vec3 n=normalize(normal);" << endl
					<< "vec3 l=normalize(lightDir);" << endl				
					<< "float lambertTerm=dot(n,l);" << endl
					<< "if (lambertTerm>0.0)" << endl
					<< "{" << endl;
	if (renderState.diffuseTexture)
		fshaderCode << "color += gl_FrontMaterial.diffuse*lightDiffuse*lambertTerm*texture2D(diffuseMap,gl_TexCoord[0].xy);" << endl;
	else
		fshaderCode << "color += gl_FrontMaterial.diffuse*lightDiffuse*lambertTerm;" << endl;
		fshaderCode << "vec3 E = normalize(eyeVec);" << endl
					<< "vec3 R = reflect(-l, n);" << endl
					<< "float specular = max(pow( max(dot(R, E), 0.0), gl_FrontMaterial.shininess ),0.0);" << endl
					<< "color += gl_FrontMaterial.specular*lightSpecular*specular;" << endl
					<< "}" << endl;
					
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
	vshader.Create(VERTEX_SHADER,vshaderCode.str());
	fshader.Create(FRAGMENT_SHADER,fshaderCode.str());
	program.Create(vshader,fshader);
	programs[renderState.ToInt()]=program;
	return program;
}