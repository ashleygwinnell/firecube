layout (location = 0) in vec3 atrPosition;
layout (location = 1) in vec3 atrNormal;
layout (location = 2) in vec2 atrTexCoord;
layout (location = 3) in vec3 atrTangent;

#ifdef SKINNING
layout (location = 5) in vec4 atrBlendWeights;
layout (location = 6) in ivec4 atrSkinIndices;

const int MAX_SKIN_MATRICES = 100;
uniform mat4 skinMatrices[MAX_SKIN_MATRICES];
#endif

#if !defined(NORMAL_MAPPING)
	smooth out vec3 normal;
#else
	smooth out mat3 tbn;
#endif
smooth out vec2 texcoord;
uniform mat4 modelMatrix;
uniform mat4 viewProjectionMatrix;
uniform mat3 normalMatrix;
uniform vec3 cameraPos;
uniform vec3 uOffset;
uniform vec3 vOffset;
void  main()
{
	#ifdef SKINNING
		mat4 boneTransform = skinMatrices[atrSkinIndices[0]] * atrBlendWeights[0];
		boneTransform += skinMatrices[atrSkinIndices[1]] * atrBlendWeights[1];
		boneTransform += skinMatrices[atrSkinIndices[2]] * atrBlendWeights[2];
		boneTransform += skinMatrices[atrSkinIndices[3]] * atrBlendWeights[3];

		vec3 worldSpacePos = vec3(modelMatrix * boneTransform * vec4(atrPosition, 1.0));
		vec3 norm = vec3(boneTransform * vec4(atrNormal, 0.0));
	#else
		vec3 worldSpacePos = vec3(modelMatrix * vec4(atrPosition, 1.0));
		vec3 norm = atrNormal;
	#endif
	texcoord = vec2(dot(atrTexCoord, uOffset.xy) + uOffset.z, dot(atrTexCoord, vOffset.xy) + vOffset.z);
	#ifdef NORMAL_MAPPING
		vec3 bitangent = cross(atrTangent, norm);
		tbn = mat3(normalize(normalMatrix * atrTangent), normalize(normalMatrix * bitangent), normalize(normalMatrix * norm));		
	#else
		normal = normalMatrix * norm;		
	#endif
	gl_Position = viewProjectionMatrix * vec4(worldSpacePos, 1.0);
}