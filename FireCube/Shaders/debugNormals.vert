attribute vec3 atrTangent;
attribute vec3 atrBitangent;
#if !defined(NORMAL_MAPPING)
	varying vec3 normal;
#else
	varying mat3 tbn;
#endif
void  main()
{
	gl_TexCoord[0] = gl_MultiTexCoord0;	
	#ifdef NORMAL_MAPPING
		tbn = mat3(normalize(atrTangent), normalize(atrBitangent), normalize(gl_Normal));		
	#endif
	#if !defined(NORMAL_MAPPING)
		normal = gl_NormalMatrix * gl_Normal;
	#endif
	gl_Position = ftransform();
}