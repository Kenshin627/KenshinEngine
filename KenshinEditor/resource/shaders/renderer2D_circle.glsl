#type vertex
#version 450 core

layout (location = 0) in vec3  a_WorldPosition;
layout (location = 1) in vec3  a_LocalPosition;
layout (location = 2) in vec4  a_Color;
layout (location = 3) in float a_Thinness;
layout (location = 4) in float a_Fade;
layout (location = 5) in int   a_EntityId;

layout(std140, binding = 0) uniform Camera
{
	mat4 u_ViewProjection;
};

struct VertexOutput
{
	vec3 LocalPosition;
	vec4 Color;
	float Thinness;
	float Fade;
};

layout (location = 0) out VertexOutput Output;
layout (location = 4) out flat int v_EntityID;

void main()
{
	Output.LocalPosition = a_LocalPosition;
	Output.Color = a_Color;
	Output.Thinness = a_Thinness;
	Output.Fade = a_Fade;

	v_EntityID = a_EntityId;
	
	gl_Position = u_ViewProjection * vec4(a_WorldPosition, 1.0);
}

#type fragment
#version 450 core

struct VertexOutput
{
	vec3 LocalPosition;
	vec4 Color;
	float Thinness;
	float Fade;
};

layout (location = 0) in VertexOutput vs_In;
layout (location = 4) in flat int v_EntityID;



layout (location = 0) out vec4 fragColor;
layout (location = 1) out int pickData;

void main()
{
	float dist = 1.0 - sqrt(dot(vs_In.LocalPosition, vs_In.LocalPosition));
	float circle = smoothstep(0.0, vs_In.Fade, dist);
	circle      *= smoothstep(vs_In.Thinness + vs_In.Fade, vs_In.Thinness, dist);
	if(circle <= 0.0)
	{
		discard;
	}

	fragColor = vs_In.Color;
	fragColor.a *= circle;

	pickData = v_EntityID;
}