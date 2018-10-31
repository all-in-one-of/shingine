#version 410 

in vec4 _WorldPosition;
in vec3 _Normal;
in vec2 _TexCoord;

uniform sampler2D MainTex;
out vec4 _OutColor;

void main()
{
	_OutColor = vec4(texture(MainTex, _TexCoord).xyz, 1.0);
}
