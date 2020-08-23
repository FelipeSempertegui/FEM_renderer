#shader vertex
#version 330 core

layout(location = 0) in vec3 globalPosition;
layout(location = 1) in vec3 vertexColor;
layout(location = 2) in vec3 aNormal;

out vec3 fragPosition;
out vec3 objectColor;
out vec3 Normal;

uniform mat4 V;
uniform mat4 M;
uniform mat4 P;

void main() {
	fragPosition = vec3(M * vec4(globalPosition, 1.0));
	gl_Position = P * V * vec4(fragPosition, 1.0);
	// Original formula: Normal = inverse(transpose(mat3(M))) * aNormal; But my M matrix is an identity, therefore
	Normal = aNormal;
	objectColor = vertexColor;
}

//====================================================================
#shader fragment
#version 330 core

in vec3 objectColor;
in vec3 Normal;
in vec3 fragPosition;

uniform vec3 lightColorAmbi;
uniform vec3 lightColorDiff;
uniform vec3 lightColorSpec;
uniform int shininess;

uniform vec3 lightPos; // in world space - position of the light
uniform vec3 viewPos;  // in world space - position of the camera

out vec4 fragmentColor;

void main() {
	//Ambient light
	vec3 ambient = lightColorAmbi;

	// Diffuse light
	// Original formula: vec3 norm = normalize(Normal); mine is already normalized, therefore, norm = Normal;
	vec3 lightDir = normalize(lightPos - fragPosition);
	float diffuseStrenght = max(dot(Normal, lightDir), 0.0);
	//vec3 diffuse = diffuseStrenght * lightColor;
	vec3 diffuse = diffuseStrenght * lightColorDiff;

	// Specular light
	vec3 viewDir = normalize(viewPos - fragPosition);
	vec3 reflectDir = reflect(-lightDir, Normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess); //132 - espelhado, 8 - "opaco"
	vec3 specular = spec * lightColorSpec;

	// combine them
	vec3 result = (ambient + diffuse) * objectColor + specular;
	fragmentColor = vec4(result, 1.0);
}