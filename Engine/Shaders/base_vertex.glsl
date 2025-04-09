#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTexCoord;
layout(location = 2) in vec3 aNormal;
layout(location = 3) in vec3 aTangent;
layout(location = 4) in vec3 aBitangent;

uniform mat4 u_ViewProjection;
uniform mat4 u_Model;
uniform mat4 u_LightSpaceMatrix;

out vec2 TexCoord;
out vec3 FragPos;
out vec3 Normal;
out vec3 Tangent; 
out vec3 Bitangent;
out vec4 FragPosLightSpace;

void main(){
    FragPos = vec3(u_Model * vec4(aPos, 1.0));
    TexCoord = aTexCoord;
    Normal = mat3(transpose(inverse(u_Model))) * aNormal;
    Tangent = mat3(transpose(inverse(u_Model))) * aTangent;
    Bitangent = mat3(transpose(inverse(u_Model))) * aBitangent;
    FragPosLightSpace = u_LightSpaceMatrix * u_Model * vec4(aPos, 1.0);

	gl_Position = u_ViewProjection * vec4(FragPos, 1.0f);
}