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

uniform mat4 modelMatrix;
uniform mat4 viewProjectionMatrix;
uniform vec3 cameraPos;
void  main()
{
	#ifdef SKINNING
		mat4 boneTransform = skinMatrices[atrSkinIndices[0]] * atrBlendWeights[0];
		boneTransform += skinMatrices[atrSkinIndices[1]] * atrBlendWeights[1];
		boneTransform += skinMatrices[atrSkinIndices[2]] * atrBlendWeights[2];
		boneTransform += skinMatrices[atrSkinIndices[3]] * atrBlendWeights[3];

		vec3 worldSpacePos = vec3(modelMatrix * boneTransform * vec4(atrPosition, 1.0));		
	#else
		vec3 worldSpacePos = vec3(modelMatrix * vec4(atrPosition, 1.0));		
	#endif	
	
	gl_Position = viewProjectionMatrix * vec4(worldSpacePos, 1.0);
}