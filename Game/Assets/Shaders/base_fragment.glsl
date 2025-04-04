#version 330 core

in vec2 TexCoord;
out vec4 FragColor;

struct Material {
    sampler2D texture_diffuse;
    sampler2D texture_specular;
    sampler2D texture_normal;
    sampler2D texture_height;
    float shininess;
};

uniform Material u_Material;
uniform bool u_bWireframe;
void main() {
    if (u_bWireframe)
        FragColor = vec4(0.0, 0.0, 0.0, 1.0);
    else
        FragColor = texture(u_Material.texture_diffuse, TexCoord);
}
