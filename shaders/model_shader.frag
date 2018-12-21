#version 410 core


#define NR_POINT_LIGHTS 2


out vec4 FragColor;


in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;


uniform sampler2D texture_diffuse1;


struct Material {
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
};

struct DirLight {
	vec3 direction;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct PointLight {
	vec3 position;
	float constant;
	float linear;
	float quadratic;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct SpotLight {
	vec3 position;
	vec3 direction;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float constant;
	float linear;
	float quadratic;
	float cutOff;
	float outerCutOff;
};


uniform PointLight pointLights[NR_POINT_LIGHTS];
//uniform PointLight pointLight;
uniform DirLight dirLight;
uniform Material material;
uniform SpotLight spotLight;
uniform vec3 viewPos;

// Bools to detirmine which lights are active
uniform bool useDirLight;
uniform bool usePointLight;
uniform bool useSpotLight;

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcSpotLight(SpotLight spotLight, vec3 nrom, vec3 FragPos, vec3 viewDir);


void main() {
	// properties
	vec3 norm = normalize(Normal);
	vec3 viewDir = normalize(viewPos - FragPos);

	vec3 result = vec3(0.0, 0.0, 0.0);

	// 1: Directional light 
	if (useDirLight) {
		result = CalcDirLight(dirLight, norm, viewDir);
	}
	// 2: Point light
	if (usePointLight) {
		for (int i = 0; i < NR_POINT_LIGHTS; i++) {
			result += CalcPointLight(pointLights[i], norm, FragPos, viewDir);
		}
		//result += CalcPointLight(pointLight, norm, FragPos, viewDir);
	}
	// 3: Spot light 
	if (useSpotLight) {
		result += CalcSpotLight(spotLight, norm, FragPos, viewDir);
	}
	FragColor = vec4(result, 1.0);
	//FragColor = texture(texture_diffuse1, TexCoords);
}


vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir) {
	vec3 lightDir = normalize(-light.direction);
	// Diffuse shading
	float diff = max(dot(normal, lightDir), 0.0);
	// Specular shading
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	// Combine results
	vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
	vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));
	vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
	return (ambient + diffuse + specular);
}


vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir) { 
	vec3 lightDir = normalize(light.position - fragPos);
    // Diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // Specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // Attenuation
    float distance    = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    // Combine results
    vec3 ambient  = light.ambient  * vec3(texture(material.diffuse, TexCoords));
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(material.diffuse, TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;
	return (ambient + diffuse + specular);
}


vec3 CalcSpotLight(SpotLight spotLight, vec3 norm, vec3 FragPos, vec3 viewDir) {
	float theta = dot(viewDir, normalize(-spotLight.direction));
	float epsilon = spotLight.cutOff - spotLight.outerCutOff;
	float intensity = clamp((theta - spotLight.outerCutOff) / epsilon, 0.0, 1.0);

	vec3 lightDir = normalize(spotLight.position - FragPos);
	// Diffuse shading
	float diff = max(dot(norm, lightDir), 0.0);
	// Specular shading
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	// attenuation
	float distance = length(spotLight.position - FragPos);
	float attenuation = 1.0 / (spotLight.constant + spotLight.linear * distance + spotLight.quadratic * (distance * distance));
	// Combine results 
	vec3 ambient = spotLight.ambient * vec3(texture(material.diffuse, TexCoords));
	vec3 diffuse = spotLight.diffuse * diff * vec3(texture(material.diffuse, TexCoords));
	vec3 specular = spotLight.specular * spec * vec3(texture(material.specular, TexCoords));

	return ((diffuse * intensity) + (specular * intensity) + ambient);
}
