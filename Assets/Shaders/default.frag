#version 410 

in vec4 _WorldPosition;
in vec3 _Normal;

out vec4 _OutColor;

void main()
{
	_OutColor = vec4(abs(_Normal), 1.0);
}
