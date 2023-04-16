#type vertex
#version 330 core

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec2 aTexcoord;

uniform mat4 u_ViewProjectionMatrix;
uniform mat4 u_ModelMatrix;

out vec2 vTexCoord;

void main()
{
	vTexCoord = aTexcoord;
	gl_Position = u_ViewProjectionMatrix * u_ModelMatrix * vec4(aPosition, 1.0);
}

#type fragment
#version 330 core
out vec4 fragColor;

in vec2 vTexCoord;
uniform sampler2D sampler;

void main()
{
	fragColor = texture(sampler, vTexCoord);
}