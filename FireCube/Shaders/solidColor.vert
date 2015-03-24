layout (location = 0) in vec3 atrPosition;
layout (location = 4) in vec4 atrColor;

uniform mat4 viewProjectionMatrix;
smooth out vec3 color;
void  main()
{
      color = atrColor.rgb;
      gl_Position = viewProjectionMatrix * vec4(atrPosition, 1.0);
}