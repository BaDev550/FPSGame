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
uniform bool u_bTextureCoords;

void main() {
    if (u_bWireframe)
        FragColor = vec4(0.0, 0.0, 0.0, 1.0);
    else if(u_bTextureCoords)
        FragColor = vec4(TexCoord, 0.0f, 1.0f);
    else
        FragColor = texture(u_Material.texture_diffuse, TexCoord);
}
