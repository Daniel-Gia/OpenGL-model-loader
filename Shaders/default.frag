#version 330 core

// Outputs colors in RGBA
out vec4 FragColor;

// Imports the data from the Vertex Shader
in vec3 currentPos;
in vec3 Normal;
in vec3 color;
in vec2 texCoord;

// Gets the Texture
uniform sampler2D baseColor0;
uniform sampler2D metallicRoughness0;

// Checks if the model has a base color texture and metallic roughness texture
uniform bool hasBaseColorTex;
uniform bool hasMetallicRoughnessTex;

// Gets the position of the camera from the main function
uniform vec3 camPos;

struct LightResult {
    float ambient;
    float diffuse;
    float specular;
    vec4 color;
};

LightResult directionalLight(vec3 normal, vec3 viewDirection, float roughness) {
	vec3 lightInput = vec3(-1.0f, -1.0f, 0.0f); // Directional light coming from
	vec4 lightColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);

	// Ambient lighting
	float ambient = 0.40f;

	// Diffuse lighting
	vec3 lightDirection = normalize(-lightInput);
	float diffuse = max(dot(normal, lightDirection), 0.0f);

	// Specular lighting
	float specularLight = 0.5f;
	vec3 reflectionDirection = reflect(-lightDirection, normal);
    float shininess = mix(128.0f, 2.0f, roughness);
	float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), shininess);
	float specular = specAmount * specularLight;

    return LightResult(ambient, diffuse, specular, lightColor);
}

void main() {
    vec3 normal = normalize(Normal);
    vec3 viewDirection = normalize(camPos - currentPos);

    vec4 baseColorTexture = hasBaseColorTex ? texture(baseColor0, texCoord) : vec4(color, 1.0f);

    float roughness = 0.8f;
    float metallic = 0.0f;

    if (hasMetallicRoughnessTex) {
        vec4 metallicRoughnessTexture = texture(metallicRoughness0, texCoord);
        roughness = metallicRoughnessTexture.g;
        metallic = metallicRoughnessTexture.b;
    }

    vec4 diffuseColor = baseColorTexture * (1.0f - metallic);
    vec4 specularColor = mix(vec4(1.0f), baseColorTexture, metallic);

    vec4 totalDiffuseAmbient = vec4(0.0f);
    vec4 totalSpecular = vec4(0.0f);

    LightResult dirLight = directionalLight(normal, viewDirection, roughness);

    totalDiffuseAmbient += dirLight.color * (dirLight.diffuse + dirLight.ambient);
    totalSpecular += dirLight.color * dirLight.specular;

	FragColor = (diffuseColor * totalDiffuseAmbient) + (specularColor * totalSpecular);
}
