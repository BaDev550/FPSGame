#version 330 core

layout(location = 0) in vec3 a_Position; 
layout(location = 1) in vec3 a_Normal;   

uniform mat4 u_Model;
uniform mat4 u_ViewProjection;
uniform float u_OutlineThickness;

out vec3 fragPosition; 

void main()
{
    vec3 expandedPosition = a_Position + normalize(a_Normal) * u_OutlineThickness;

    fragPosition = expandedPosition;
    gl_Position = u_ViewProjection * u_Model * vec4(expandedPosition, 1.0f);
}
