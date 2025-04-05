#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTexCoord;
layout(location = 2) in vec3 aNormal;
layout(location = 3) in vec3 aTangent;
layout(location = 4) in vec3 aBitangent;

uniform mat4 u_ViewProjection;
uniform mat4 u_Model;

out vec2 TexCoord;
out vec3 Normal;

void main(){
    TexCoord = aTexCoord;
	Normal = mat3(transpose(inverse(u_Model))) * aNormal;
	gl_Position = u_ViewProjection * u_Model * vec4(aPos, 1.0f);
}