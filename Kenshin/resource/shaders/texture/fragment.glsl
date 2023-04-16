#version 330 core
out vec4 fragColor;

in vec2 vTexCoord;
uniform sampler2D sampler;

void main()
{
	fragColor = texture(sampler, vTexCoord);
//	fragColor = vec4(vTexCoord, 0.0, 1.0);
}