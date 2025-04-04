#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTexCoord;
layout(location = 2) in vec3 aNormal;
layout(location = 3) in vec3 aTangent;
layout(location = 4) in vec3 aBitangent;

uniform mat4 u_ViewProjection;

out vec2 TexCoord;

void main(){
    TexCoord = aTexCoord;
	gl_Position = u_ViewProjection * vec4(aPos, 1.0f);
}