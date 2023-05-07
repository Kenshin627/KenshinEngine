#type vertex
#version 450 core

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec4 aColor;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in float aTilingFactor;
layout (location = 4) in float aTexIndex;
layout (location = 5) in int aEntityId;

layout(std140, binding = 0) uniform Camera
{
	mat4 u_ViewProjection;
};

struct VertexOutput
{
	vec4 Color;
	vec2 TexCoord;
	float TilingFactor;
};

layout (location = 0) out VertexOutput Output;
layout (location = 3) out flat float v_Texindex;
layout (location = 4) out flat int v_EntityID;

void main()
{
	Output.Color = aColor;
	Output.TexCoord = aTexCoord;
	Output.TilingFactor = aTilingFactor;
	v_Texindex = aTexIndex;
	v_EntityID = aEntityId;
	
	gl_Position = u_ViewProjection * vec4(aPosition, 1.0);
}

#type fragment
#version 450 core

struct VertexOutput
{
	vec4 Color;
	vec2 TexCoord;
	float TilingFactor;
};

layout (location = 0) in VertexOutput vs_In;
layout (location = 3) in flat float v_Texindex;
layout (location = 4) in flat int v_EntityID;

layout (binding = 0) uniform sampler2D u_Textures[32];

layout (location = 0) out vec4 fragColor;
layout (location = 1) out int pickData;

void main()
{
	vec4 texColor = vs_In.Color;

	switch(int(v_Texindex))
	{
		case  0: texColor *= texture(u_Textures[ 0], vs_In.TexCoord * vs_In.TilingFactor); break;
		case  1: texColor *= texture(u_Textures[ 1], vs_In.TexCoord * vs_In.TilingFactor); break;
		case  2: texColor *= texture(u_Textures[ 2], vs_In.TexCoord * vs_In.TilingFactor); break;
		case  3: texColor *= texture(u_Textures[ 3], vs_In.TexCoord * vs_In.TilingFactor); break;
		case  4: texColor *= texture(u_Textures[ 4], vs_In.TexCoord * vs_In.TilingFactor); break;
		case  5: texColor *= texture(u_Textures[ 5], vs_In.TexCoord * vs_In.TilingFactor); break;
		case  6: texColor *= texture(u_Textures[ 6], vs_In.TexCoord * vs_In.TilingFactor); break;
		case  7: texColor *= texture(u_Textures[ 7], vs_In.TexCoord * vs_In.TilingFactor); break;
		case  8: texColor *= texture(u_Textures[ 8], vs_In.TexCoord * vs_In.TilingFactor); break;
		case  9: texColor *= texture(u_Textures[ 9], vs_In.TexCoord * vs_In.TilingFactor); break;
		case 10: texColor *= texture(u_Textures[10], vs_In.TexCoord * vs_In.TilingFactor); break;
		case 11: texColor *= texture(u_Textures[11], vs_In.TexCoord * vs_In.TilingFactor); break;
		case 12: texColor *= texture(u_Textures[12], vs_In.TexCoord * vs_In.TilingFactor); break;
		case 13: texColor *= texture(u_Textures[13], vs_In.TexCoord * vs_In.TilingFactor); break;
		case 14: texColor *= texture(u_Textures[14], vs_In.TexCoord * vs_In.TilingFactor); break;
		case 15: texColor *= texture(u_Textures[15], vs_In.TexCoord * vs_In.TilingFactor); break;
		case 16: texColor *= texture(u_Textures[16], vs_In.TexCoord * vs_In.TilingFactor); break;
		case 17: texColor *= texture(u_Textures[17], vs_In.TexCoord * vs_In.TilingFactor); break;
		case 18: texColor *= texture(u_Textures[18], vs_In.TexCoord * vs_In.TilingFactor); break;
		case 19: texColor *= texture(u_Textures[19], vs_In.TexCoord * vs_In.TilingFactor); break;
		case 20: texColor *= texture(u_Textures[20], vs_In.TexCoord * vs_In.TilingFactor); break;
		case 21: texColor *= texture(u_Textures[21], vs_In.TexCoord * vs_In.TilingFactor); break;
		case 22: texColor *= texture(u_Textures[22], vs_In.TexCoord * vs_In.TilingFactor); break;
		case 23: texColor *= texture(u_Textures[23], vs_In.TexCoord * vs_In.TilingFactor); break;
		case 24: texColor *= texture(u_Textures[24], vs_In.TexCoord * vs_In.TilingFactor); break;
		case 25: texColor *= texture(u_Textures[25], vs_In.TexCoord * vs_In.TilingFactor); break;
		case 26: texColor *= texture(u_Textures[26], vs_In.TexCoord * vs_In.TilingFactor); break;
		case 27: texColor *= texture(u_Textures[27], vs_In.TexCoord * vs_In.TilingFactor); break;
		case 28: texColor *= texture(u_Textures[28], vs_In.TexCoord * vs_In.TilingFactor); break;
		case 29: texColor *= texture(u_Textures[29], vs_In.TexCoord * vs_In.TilingFactor); break;
		case 30: texColor *= texture(u_Textures[30], vs_In.TexCoord * vs_In.TilingFactor); break;
		case 31: texColor *= texture(u_Textures[31], vs_In.TexCoord * vs_In.TilingFactor); break;
	}
	fragColor = texColor;
	pickData = v_EntityID;
}