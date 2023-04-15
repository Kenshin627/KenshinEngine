#version 330 core

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec4 aColor;

uniform mat4 u_ViewProjectionMatrix;

out vec4 vColor;
void main()
{
	vColor = aColor;
	gl_Position = u_ViewProjectionMatrix * vec4(aPosition, 1.0);
}