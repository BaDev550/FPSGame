#version 330 core

in vec2 TexCoord;
in vec3 FragPos;
in vec3 Normal;
out vec4 FragColor;

struct Material {
    sampler2D texture_diffuse;
    sampler2D texture_specular;
    sampler2D texture_normal;
    sampler2D texture_height;
    bool Highlight;
    float shininess;
    float metalness;
    float roughness;
};

uniform Material u_Material;
uniform vec3 u_LightPos;
uniform vec3 u_ViewPos;

vec3 FresnelSchlick(float cosTheta, vec3 F0) {
    return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}

float DistributionGGX(vec3 normal, vec3 halfVec, float roughness) {
    float a = roughness * roughness;
    float a2 = a * a;
    float NdotH = max(dot(normal, halfVec), 0.0);
    float NdotH2 = NdotH * NdotH;
    float nom = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = 3.14159 * denom * denom;
    return nom / denom;
}

float GeometrySchlickGGX(float NdotV, float roughness) {
    float k = (roughness + 1.0) * (roughness + 1.0) / 8.0;
    float nom = NdotV;
    float denom = NdotV * (1.0 - k) + k;
    return nom / denom;
}

float GeometrySmith(vec3 normal, vec3 viewDir, vec3 lightDir, float roughness) {
    float NdotV = max(dot(normal, viewDir), 0.0);
    float NdotL = max(dot(normal, lightDir), 0.0);
    float ggx2 = GeometrySchlickGGX(NdotV, roughness);
    float ggx1 = GeometrySchlickGGX(NdotL, roughness);
    return ggx1 * ggx2;
}

void main() {
    // Retrieve material textures
    vec4 diffuseColor = texture(u_Material.texture_diffuse, TexCoord);
    vec4 normalColor = texture(u_Material.texture_normal, TexCoord);
    vec4 heightMap = texture(u_Material.texture_height, TexCoord);

    // Calculate normal and height information
    vec3 normal = normalize(normalColor.rgb * 2.0 - 1.0);  // Normal mapping
    float height = heightMap.r;  // Can be used for parallax mapping if needed
    
    // Light and view directions
    vec3 lightDir = normalize(u_LightPos - FragPos);
    vec3 viewDir = normalize(u_ViewPos - FragPos);
    vec3 halfVec = normalize(lightDir + viewDir);

    // Calculate the diffuse term (Lambertian reflection)
    float diff = max(dot(normal, lightDir), 0.0);

    // Calculate the Fresnel-Schlick term
    float cosTheta = max(dot(viewDir, normal), 0.0);
    vec3 F0 = vec3(0.04);  // Non-metallic F0 value
    F0 = mix(F0, diffuseColor.rgb, u_Material.metalness);  // Metalness affects F0 value
    vec3 F = FresnelSchlick(cosTheta, F0);

    // Cook-Torrance BRDF: Compute the D (distribution function) and G (geometry term)
    float D = DistributionGGX(normal, halfVec, u_Material.roughness);
    float G = GeometrySmith(normal, viewDir, lightDir, u_Material.roughness);

    // Compute the specular reflection using the Cook-Torrance model
    vec3 specular = (D * G * F) / (4.0 * max(dot(normal, viewDir), 0.0) * max(dot(normal, lightDir), 0.0) + 0.001);

    // Combine the results
    vec3 diffuse = diffuseColor.rgb * diff;  // Lambertian diffuse reflection
    vec3 color = diffuse + specular;  // Final color

    // Highlight effect (if needed)
    if (u_Material.Highlight) {
        color = mix(color, vec3(1.0, 1.0, 0.0), 0.2);  // Add yellow highlight
    }

    // Final output color
    FragColor = vec4(color, 1.0);
}