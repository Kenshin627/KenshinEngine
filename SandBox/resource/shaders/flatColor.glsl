#type vertex
#version 330 core

layout (location = 0) in vec3 aPosition;

uniform mat4 u_ViewProjectionMatrix;
uniform mat4 u_ModelMatrix;

void main()
{
	gl_Position = u_ViewProjectionMatrix * u_ModelMatrix * vec4(aPosition, 1.0);
}

#type fragment
#version 330 core

out vec4 fragColor;
uniform vec3 u_Color;

void main()
{
	fragColor = vec4(u_Color, 1.0);
}