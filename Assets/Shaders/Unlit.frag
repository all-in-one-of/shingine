#version 410 

uniform vec4 _UnlitColor;

in vec4 _WorldPosition;
in vec3 _Normal;

out vec4 _OutColor;

void main()
{
	_OutColor = _UnlitColor;
}
