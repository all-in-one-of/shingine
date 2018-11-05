#version 410 

// #define TURN_OFF_MATRICES 0

uniform mat4 _ModelMatrix;
uniform mat4 _ModelMatrixInverseTransposed;
uniform mat4 _ViewMatrix;
uniform mat4 _ProjectionMatrix;

in vec3 _PositionAttribute;
in vec3 _NormalAttribute;
in vec3 _TexCoordAttribute;

out vec4 _WorldPosition;
out vec3 _Normal;

void main()
{
    vec3 position = _PositionAttribute;
	#ifdef TURN_OFF_MATRICES
	gl_Position = vec4(position, 1.0);
	#else
	mat4 MVP = _ProjectionMatrix * _ViewMatrix * _ModelMatrix;
    gl_Position = MVP * vec4(position, 1.0);
	#endif

	_WorldPosition = _ModelMatrix * vec4(position, 1.0);
	_Normal = vec3(_ModelMatrixInverseTransposed * vec4(normalize(_NormalAttribute), 1.0f));
}
