#version 410 

in vec4 _WorldPosition;
in vec3 _Normal;
in vec2 _TexCoord;

out vec4 _OutColor;

uniform sampler2D _MainTex;
uniform vec4 _CameraWorldPosition;

const int MAX_LIGHTS = 4;
const int POINT_LIGHT_TYPE = 0;
const int SPOT_LIGHT_TYPE = 1;
const int DIRECTIONAL_LIGHT_TYPE = 2;

struct Light
{
	vec4 Position;
	vec4 Direction;
	// Color, Intensity
	vec4 Color; 
	// Type, Inner Angle, Outer, Shadow Enabled	
	vec4 Params0; 
	// Constant, Linear, Quadratic, Cutoff	
	vec4 Params1; 	
};

struct Directional
{
	// Direction, Shadow enabled
	vec4 Direction; 
	// Color, Intensity
	vec4 Color; 	
};

const float SpecularPower = 90.0;

uniform Light Lights[MAX_LIGHTS];
uniform Directional DirectionalLight;

void main()
{
	vec3 normal = normalize(_Normal);
	vec3 lightColor = vec3(0.0);
	// directional light
	// Point / Spot Light calculation
	for (int x = 0; x < MAX_LIGHTS; x++)
	{
		vec3 lightToPos = Lights[x].Position.xyz - _WorldPosition.xyz;
		float distanceToLight = length(lightToPos);
		vec3 lightVector = normalize(lightToPos);
		float dotValue = dot(lightVector, normal);

		vec3 diffuseContribution = Lights[x].Color.xyz * dotValue;
		vec3 specularContribution = vec3(0.0);

		vec3 reflectionVector = reflect(-lightVector, normal);
		vec3 posToCamera = normalize(_CameraWorldPosition.xyz - _WorldPosition.xyz);

		float specularMultiplier = 1.0;
		specularContribution = vec3(pow(max(0.0, dot(posToCamera, reflectionVector)), SpecularPower) 
			* specularMultiplier);

		float attenuation = 1.0 / (Lights[x].Params1.x + Lights[x].Params1.y * distanceToLight +
			Lights[x].Params1.z * distanceToLight * distanceToLight);

		diffuseContribution *= attenuation;
		specularContribution *= attenuation;

		if (Lights[x].Params0.x == SPOT_LIGHT_TYPE)
		{
			vec3 posToLight = normalize(_WorldPosition.xyz - Lights[x].Position.xyz);
			float lightRayAngle = max(0.0, dot(posToLight, Lights[x].Direction.xyz));

			float innerCosAngle = cos(radians(Lights[x].Params0.y));
			float outerCosAngle = cos(radians(Lights[x].Params0.z));

			if (lightRayAngle < outerCosAngle)
			{
				diffuseContribution = vec3(0.0);
				specularContribution = vec3(0.0);
			}
			if (lightRayAngle < innerCosAngle)
			{
				float ratio = (lightRayAngle - outerCosAngle) / (innerCosAngle - outerCosAngle);
				diffuseContribution *= ratio;
				specularContribution *= ratio;
			}
		}
		lightColor += (diffuseContribution + specularContribution) * Lights[x].Color.w;
	}
	vec4 tex = texture(_MainTex, _TexCoord);
	
	vec4 objectColor = vec4((lightColor + vec3(.5)) * tex, 1.0);
	_OutColor = objectColor;
}
