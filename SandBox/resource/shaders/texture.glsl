#type vertex
#version 330 core

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec4 aColor;
layout (location = 2) in vec2 aTexCoord;

uniform mat4 u_ViewProjectionMatrix;

out vec4 v_Color;
out vec2 v_TexCoord;

void main()
{
	v_Color = aColor;
	v_TexCoord = aTexCoord;
	gl_Position = u_ViewProjectionMatrix * vec4(aPosition, 1.0);
}

#type fragment
#version 330 core
out vec4 fragColor;

in vec4 v_Color;
in vec2 v_TexCoord;

//uniform sampler2D sampler;
//uniform float u_TilingFactor;

void main()
{
	fragColor = v_Color;
}