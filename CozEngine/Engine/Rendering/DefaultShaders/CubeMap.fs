#version 330 core
out vec4 FragColor;

in vec3 TexCoords;

uniform samplerCube SkyboxTexture;

void main()
{    
    FragColor = texture(SkyboxTexture, TexCoords);
}