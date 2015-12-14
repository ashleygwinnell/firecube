layout (location = 0) in vec3 atrPosition;

smooth out vec3 texcoord;
uniform mat4 viewProjectionMatrix;
uniform vec3 cameraPos;

void  main()
{	
	vec3 worldSpacePos = atrPosition + cameraPos;
	texcoord = atrPosition;
	gl_Position = (viewProjectionMatrix * vec4(worldSpacePos, 1.0)).xyww;	
}