// FRAGMENT SHADER

#version 330


	uniform sampler2D texture0;
	in vec2 texCoord0;

	in vec4 color;
	out vec4 outColor;

// Materials

	uniform vec3 materialAmbient;
	uniform vec3 materialDiffuse;
	uniform vec3 materialSpecular;
	uniform float shininess;


// View Matrix
	uniform mat4 matrixView;

	in vec4 position;
	in vec3 normal;

//function for point light
struct POINT
{
	vec3 position;
	vec3 diffuse;
	vec3 specular;
};
uniform POINT lightPoint;

vec4 PointLight (POINT light)
{
	// Calculate Point Light 
	vec4 color = vec4(0, 0, 0, 0);
	
	//get light Position
	vec4 lightPos = vec4 (light.position, 1);
	vec4 L = normalize(matrixView * lightPos - position);

	float NdotL = dot(normal, L.xyz); 
	color += vec4(materialDiffuse * light.diffuse, 1) * max(NdotL, 0.0); 

	vec3 V = normalize(-position.xyz);
	vec3 R = reflect(-L.xyz, normal);
	float RdotV = dot(R, V);
	color += vec4(materialSpecular * light.specular * pow(max(RdotV, 0), shininess), 1);

	return color;
}
uniform POINT lightPoint1, lightPoint2;


void main(void) 
{
 
  outColor = color;
  outColor += PointLight(lightPoint1);
  outColor += PointLight(lightPoint2);
  outColor *= texture(texture0, texCoord0);
 
}
