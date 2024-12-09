// VERTEX SHADER

#version 330


// Matrices
uniform mat4 matrixProjection;
uniform mat4 matrixView;
uniform mat4 matrixModelView;


// Materials
uniform vec3 materialAmbient;
uniform vec3 materialDiffuse;
uniform vec3 materialSpecular;
uniform float shininess;


in vec3 aVertex;
in vec3 aNormal;


out vec4 color;
vec4 position;
vec3 normal;

// Light declarations
struct AMBIENT
{
	vec3 color;
};
uniform AMBIENT lightAmbient;

struct DIRECTIONAL
{
	vec3 direction;
	vec3 diffuse;
};
uniform DIRECTIONAL lightDir;

struct POINT
{
	vec3 position;
	vec3 diffuse;
	vec3 specular;
};
uniform POINT lightPoint;


//Light Function
vec4 AmbientLight(AMBIENT light)
{
	// Calculate Ambient Light
	return vec4(materialAmbient * light.color, 1);
} 

vec4 DirectionalLight(DIRECTIONAL light)
{
	// Calculate Directional Light
	vec4 color = vec4(0, 0, 0, 0);
	vec3 L = normalize(mat3(matrixView) * light.direction);
	float NdotL = dot(normal, L);
	color += vec4(materialDiffuse * light.diffuse, 1) * max(NdotL, 0);
	return color;
}

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

void main(void)
{

	// calculate position
	position = matrixModelView * vec4(aVertex, 1.0);
	gl_Position = matrixProjection * position;

	//calculate normal
	normal = normalize(mat3(matrixModelView) * aNormal);

	// calculate light
	color = vec4(0, 0, 0, 1);
	color += AmbientLight(lightAmbient);
	color += DirectionalLight(lightDir);
	color += PointLight(lightPoint);

}