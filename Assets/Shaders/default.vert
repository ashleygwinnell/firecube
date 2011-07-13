#ifdef POINT_LIGHTING
	varying vec3 lightDir;
	varying vec3 eyeVec;
	uniform vec3 lightPosition;
#endif

#ifdef DIRECTIONAL_LIGHTING	
	varying vec3 lightDir;
	varying vec3 eyeVec;
	uniform vec3 directionalLightDir;
#endif

attribute vec3 atrTangent;
attribute vec3 atrBitangent;
#if !defined(NORMAL_MAPPING)
	varying vec3 normal;
#endif
void  main()
{
	gl_TexCoord[0] = gl_MultiTexCoord0;
	#ifdef POINT_LIGHTING	
		vec3 vVertex = vec3(gl_ModelViewMatrix * gl_Vertex);
		lightDir= vec3(lightPosition - vVertex);
		eyeVec = -vVertex;
	#endif
	#ifdef DIRECTIONAL_LIGHTING
		lightDir = directionalLightDir;
		eyeVec = -vec3(gl_ModelViewMatrix * gl_Vertex);		
	#endif
	#ifdef NORMAL_MAPPING
		mat3 tbn = mat3(normalize(atrTangent), normalize(atrBitangent), normalize(gl_Normal));
		eyeVec = eyeVec * gl_NormalMatrix * tbn;
		lightDir = lightDir * gl_NormalMatrix * tbn;
	#endif
	#if !defined(NORMAL_MAPPING)
		normal = gl_NormalMatrix * gl_Normal;
	#endif
	gl_Position = ftransform();
}