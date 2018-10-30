#version 410 

in vec3 _Color;
in vec4 _WorldPosition;
in vec3 _Normal;
in vec2 _UV;

uniform sampler2D MainTex;
out vec4 _OutColor;

void main()
{
	_OutColor = vec4(/* normalize(_Color) * */
		texture(MainTex, _UV ).xyz, 1.0);
}
