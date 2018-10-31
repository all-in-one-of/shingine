#version 410 

// hard coded uniforms
uniform mat4 _ModelMatrix;
uniform mat4 _ModelMatrixInverseTransposed;
uniform mat4 _ViewMatrix;
uniform mat4 _ProjectionMatrix;

in vec3 _PositionAttribute;
in vec3 _NormalAttribute;
in vec3 _TexCoordAttribute;
// ...

//
out vec4 _WorldPosition;
out vec3 _Normal;
out vec2 _TexCoord;
//

void main()
{
    vec3 position = _PositionAttribute;
	mat4 MVP = _ProjectionMatrix * _ViewMatrix * _ModelMatrix;
    
    gl_Position = MVP * vec4(position, 1.0);

	_WorldPosition = _ModelMatrix * vec4(position, 1.0);
	_Normal = vec3(_ModelMatrixInverseTransposed * vec4(normalize(_NormalAttribute), 1.0f));
    _TexCoord = _TexCoordAttribute.xy;
}
